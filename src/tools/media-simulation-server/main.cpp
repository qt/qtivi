/****************************************************************************
**
** Copyright (C) 2019 Luxoft Sweden AB
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtIVI module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL-QTAS$
** Commercial License Usage
** Licensees holding valid commercial Qt Automotive Suite licenses may use
** this file in accordance with the commercial license agreement provided
** with the Software or, alternatively, in accordance with the terms
** contained in a written agreement between you and The Qt Company.  For
** licensing terms and conditions see https://www.qt.io/terms-conditions.
** For further information use the contact form at https://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl-3.0.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 2.0 or (at your option) the GNU General
** Public license version 3 or any later version approved by the KDE Free
** Qt Foundation. The licenses are as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL2 and LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-2.0.html and
** https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
** SPDX-License-Identifier: LGPL-3.0
**
****************************************************************************/

#include <QCoreApplication>

#include "database_helper.h"

#include "mediaplayerbackend.h"
#include "qivimediaplayerqtroadapter.h"

#include "mediaindexerbackend.h"
#include "qivimediaindexerqtroadapter.h"

#include "mediadiscoverybackend.h"
#include "qivimediadiscoveryqtroadapter.h"

#include "searchandbrowsebackend.h"
#include "qivisearchandbrowsemodelqtroadapter.h"
#include "usbdevice.h"

#include "core.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    QString dbFile = mediaDatabaseFile();
    createMediaDatabase(dbFile);

    MediaIndexerBackend *indexerBackend = new MediaIndexerBackend(createDatabaseConnection(QStringLiteral("indexer"), dbFile), qApp);
    MediaPlayerBackend *playerBackend = new MediaPlayerBackend(createDatabaseConnection(QStringLiteral("player"), dbFile), qApp);
    MediaDiscoveryBackend *discoveryBackend = new MediaDiscoveryBackend(qApp);
    SearchAndBrowseBackend *searchAndBrowseBackend = new SearchAndBrowseBackend(createDatabaseConnection(QStringLiteral("model"), dbFile), qApp);

    auto deviceMap = discoveryBackend->deviceMap();
    for (auto it = deviceMap.cbegin(); it != deviceMap.cend(); it++) {
        USBDevice *device = qobject_cast<USBDevice*>(it.value());
        if (!device)
            continue;
        indexerBackend->addMediaFolder(device->folder());
    }

    QObject::connect(indexerBackend, &MediaIndexerBackend::removeFromQueue,
                     playerBackend, &MediaPlayerBackend::remove);
    QObject::connect(discoveryBackend, &MediaDiscoveryBackend::mediaDirectoryAdded,
                     indexerBackend, &MediaIndexerBackend::addMediaFolder);
    QObject::connect(discoveryBackend, &MediaDiscoveryBackend::mediaDirectoryRemoved,
                     indexerBackend, &MediaIndexerBackend::removeMediaFolder);

    //initialize all our backends
    indexerBackend->initialize();
    playerBackend->initialize();
    discoveryBackend->initialize();
    searchAndBrowseBackend->initialize();

    //Start Remoting the backends
    Core::instance()->host()->enableRemoting<QIviMediaIndexerAddressWrapper>(new QIviMediaIndexerQtRoAdapter(indexerBackend));
    Core::instance()->host()->enableRemoting<QIviMediaPlayerAddressWrapper>(new QIviMediaPlayerQtRoAdapter(playerBackend));
    Core::instance()->host()->enableRemoting<QIviMediaDiscoveryModelAddressWrapper>(new QIviMediaDiscoveryModelQtRoAdapter(discoveryBackend));
    Core::instance()->host()->enableRemoting<QIviSearchAndBrowseModelAddressWrapper>(new QIviSearchAndBrowseModelQtRoAdapter(searchAndBrowseBackend));

    return app.exec();
}
