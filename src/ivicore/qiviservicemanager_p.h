/****************************************************************************
**
** Copyright (C) 2019 Luxoft Sweden AB
** Copyright (C) 2018 Pelagicore AG
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

#ifndef QIVISERVICEMANAGER_P_H
#define QIVISERVICEMANAGER_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists purely as an
// implementation detail. This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <QtCore/QAbstractListModel>
#include <QtCore/QLoggingCategory>
#include <QtCore/QMap>
#include <QtCore/QPluginLoader>
#include <QtCore/QSet>
#include <QtCore/QStringList>
#include <QtCore/QVariantMap>

#include <QtIviCore/qiviservicemanager.h>
#include <private/qtiviglobal_p.h>

QT_BEGIN_NAMESPACE

class QIviServiceInterface;
class QIviServiceObject;
class QIviProxyServiceObject;

Q_DECLARE_LOGGING_CATEGORY(qLcIviServiceManagement)

struct Backend{
    QString name;
    bool debug;
    QVariantMap metaData;
    QIviServiceInterface *interface;
    QObject *interfaceObject;
    QIviProxyServiceObject *proxyServiceObject;
    QPluginLoader *loader;
};

class Q_QTIVICORE_EXPORT QIviServiceManagerPrivate : public QObject
{
    Q_OBJECT

public:
    explicit QIviServiceManagerPrivate(QIviServiceManager *parent);

    static QIviServiceManagerPrivate* get(QIviServiceManager *serviceManager);
    static bool isSimulation(const QVariantMap &metaData);

    QIviProxyServiceObject *createServiceObject(struct Backend *backend) const;
    QList<QIviServiceObject*> findServiceByInterface(const QString &interface, QIviServiceManager::SearchFlags searchFlags) const;

    void searchPlugins();
    void registerStaticBackend(QStaticPlugin plugin);
    void registerBackend(const QString &fileName, const QJsonObject &metaData);
    bool registerBackend(QObject *serviceBackendInterface, const QStringList &interfaces, QIviServiceManager::BackendType backendType);
    void addBackend(struct Backend *backend);

    void unloadAllBackends();

    QIviServiceInterface *loadServiceBackendInterface(struct Backend *backend) const;

    QList<Backend*> m_backends;
    QSet<QString> m_interfaceNames;
    QStringList m_loadedPaths;
    bool m_staticLoaded;

    QIviServiceManager * const q_ptr;
    Q_DECLARE_PUBLIC(QIviServiceManager)

Q_SIGNALS:
    void beginInsertRows(const QModelIndex &index, int start, int end);
    void endInsertRows();

};

QT_END_NAMESPACE

#endif // QIVISERVICEMANAGER_P_H

