/****************************************************************************
**
** Copyright (C) 2017 Pelagicore AG
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtIvi module of the Qt Toolkit.
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

#include "mediadiscoverybackend.h"
#include "mediaindexerbackend.h"
#include "mediaplayerbackend.h"
#include "mediaplugin.h"
#include "searchandbrowsebackend.h"

#include <QtIviCore/QIviSearchAndBrowseModel>
#include <QtIviMedia/QIviMediaPlayer>

#include <QStringList>
#include <QTemporaryFile>
#include <QCoreApplication>
#include <QtDebug>

MediaPlugin::MediaPlugin(QObject *parent)
    : QObject(parent)
    , m_discovery(new MediaDiscoveryBackend(this))
{
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    const QByteArray database = qgetenv("QTIVIMEDIA_SIMULATOR_DATABASE");
    if (database.isEmpty()) {
        QTemporaryFile *tempFile = new QTemporaryFile(qApp);
        tempFile->open();
        m_db.setDatabaseName(tempFile->fileName());
        qCritical() << "QTIVIMEDIA_SIMULATOR_DATABASE environment variable isn't set.\n"
                    << "Using the temporary database: " << tempFile->fileName();
    } else {
        m_db.setDatabaseName(database);
    }

    m_player = new MediaPlayerBackend(m_db, this);
    m_browse = new SearchAndBrowseBackend(m_db, this);
    m_indexer = new MediaIndexerBackend(m_db, this);

    connect(m_discovery, &MediaDiscoveryBackend::mediaDirectoryAdded,
            m_indexer, &MediaIndexerBackend::addMediaFolder);
    connect(m_discovery, &MediaDiscoveryBackend::mediaDirectoryRemoved,
            m_indexer, &MediaIndexerBackend::removeMediaFolder);
}

QStringList MediaPlugin::interfaces() const
{
    QStringList list;
    list << QIviMediaPlayer_iid;
    list << QIviSearchAndBrowseModel_iid;
    list << QIviMediaDeviceDiscovery_iid;
    list << QIviMediaIndexer_iid;
    return list;
}

QIviFeatureInterface *MediaPlugin::interfaceInstance(const QString &interface) const
{
    if (interface == QIviMediaPlayer_iid)
        return m_player;
    else if (interface == QIviSearchAndBrowseModel_iid)
        return m_browse;
    else if (interface == QIviMediaDeviceDiscovery_iid)
        return m_discovery;
    else if (interface == QIviMediaIndexer_iid)
        return m_indexer;

    return 0;
}
