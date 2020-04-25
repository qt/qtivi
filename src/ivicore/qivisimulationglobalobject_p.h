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

#ifndef QIVISIMULATIONGLOBALOBJECT_P_H
#define QIVISIMULATIONGLOBALOBJECT_P_H

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

#include <qiviqmlconversion_helper.h>
#include <QIviPagingModelInterface>

#include <QtCore/QObject>
#include <QtCore/QVariantMap>
#include <QtCore/QMetaType>
#include <QtCore/QMetaObject>
#include <QtCore/QMetaEnum>
#include <QJSValue>
#include <QQmlPropertyMap>

#include <QDebug>

QT_BEGIN_NAMESPACE

class Q_QTIVICORE_EXPORT QIviSimulationGlobalObject : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVariant simulationData READ simulationData CONSTANT)

public:
    explicit QIviSimulationGlobalObject(QObject *parent = nullptr);

    QVariant simulationData() const;
    void setSimulationData(const QVariant &simulationData);

    Q_INVOKABLE QVariantMap findData(const QVariantMap &data, const QString &interface);
    Q_INVOKABLE void initializeDefault(const QVariantMap &data, QObject *object);
    Q_INVOKABLE QVariant defaultValue(const QVariantMap &data, const QString &zone = QString());
    Q_INVOKABLE QString constraint(const QVariantMap &data, const QString &zone = QString());
    Q_INVOKABLE bool checkSettings(const QVariantMap &data, const QVariant &value, const QString &zone = QString());
    Q_INVOKABLE QVariant parseDomainValue(const QVariantMap &data, const QString &domain, const QString &zone = QString());

private:
    QGenericArgument createArgument(const QVariant &variant);
    QVariant m_simulationData;
};

QT_END_NAMESPACE

#endif // QIVISIMULATIONGLOBALOBJECT_P_H
