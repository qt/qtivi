/****************************************************************************
**
** Copyright (C) 2015 Pelagicore AG
** Contact: http://www.qt.io/ or http://www.pelagicore.com/
**
** This file is part of the QtIVI module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL3-PELAGICORE$
** Commercial License Usage
** Licensees holding valid commercial Qt IVI licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Pelagicore. For licensing terms
** and conditions, contact us at http://www.pelagicore.com.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPLv3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl-3.0.html.
**
** $QT_END_LICENSE$
**
** SPDX-License-Identifier: LGPL-3.0
**
****************************************************************************/
#include <QtQml/qqmlextensionplugin.h>
#include <qqml.h>

#include <QtIVICore/QtIVICore>

QT_BEGIN_NAMESPACE

class QtIVICorePlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QQmlExtensionInterface/1.0")
public:
    virtual void registerTypes(const char *uri)
    {
        Q_ASSERT(QLatin1String(uri) == QLatin1String("QtIVICore"));
        Q_UNUSED(uri);

        qmlRegisterUncreatableType<QtIVIAbstractFeature>(uri, 1, 0, "AbstractFeature", "AbstractFeature is not accessible directly");
        qmlRegisterUncreatableType<QtIVIAbstractZonedFeature>(uri, 1, 0, "AbstractZonedFeature", "AbstractZonedFeature is not accessible directly");
    }
};

QT_END_NAMESPACE

#include "plugin.moc"
