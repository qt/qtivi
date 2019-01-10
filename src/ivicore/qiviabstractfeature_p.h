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

#ifndef QIVIABSTRACTFEATURE_P_H
#define QIVIABSTRACTFEATURE_P_H

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

#include <private/qobject_p.h>
#include <private/qtiviglobal_p.h>

#include "qiviabstractfeature.h"
#include "qivifeatureinterface.h"
#include "qiviserviceobject.h"

QT_BEGIN_NAMESPACE

class Q_QTIVICORE_EXPORT QIviPropertyOverrider {
public:
    QIviPropertyOverrider() = default;
    virtual ~QIviPropertyOverrider() = default;

    virtual QVariant property(int propertyIndex) const = 0;
    virtual void setProperty(int propertyIndex, const QVariant &value) = 0;
    virtual bool isOverridden(int propertyIndex) const = 0;
private:
    Q_DISABLE_COPY(QIviPropertyOverrider)
};

class Q_QTIVICORE_EXPORT QIviAbstractFeaturePrivate : public QObjectPrivate
{
public:
    QIviAbstractFeaturePrivate(const QString &interface, QIviAbstractFeature *parent);

    static QIviAbstractFeaturePrivate *get(QIviAbstractFeature *q);

    virtual void initialize();
    virtual bool notify(const QByteArray &propertyName, const QVariant &value);

    QIviFeatureInterface *backend() const;
    template <class T> T backend() const
    {
        Q_Q(const QIviAbstractFeature);
        if (m_serviceObject)
            return m_serviceObject->interfaceInstance<T>(q->interfaceName());
        return nullptr;
    }

    void setDiscoveryResult(QIviAbstractFeature::DiscoveryResult discoveryResult);
    void onInitializationDone();

    QIviAbstractFeature * const q_ptr;
    Q_DECLARE_PUBLIC(QIviAbstractFeature)
    Q_DISABLE_COPY(QIviAbstractFeaturePrivate)

    QString m_interface;
    QIviServiceObject *m_serviceObject;
    QIviAbstractFeature::DiscoveryMode m_discoveryMode;
    QIviAbstractFeature::DiscoveryResult m_discoveryResult;
    QString m_errorMessage;
    QIviAbstractFeature::Error m_error;
    bool m_qmlCreation;
    bool m_isInitialized;
    bool m_isConnected;

    bool m_supportsPropertyOverriding;
    QIviPropertyOverrider *m_propertyOverride;
};

QT_END_NAMESPACE

#endif // QIVIABSTRACTFEATURE_P_H
