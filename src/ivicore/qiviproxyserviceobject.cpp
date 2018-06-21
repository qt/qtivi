/****************************************************************************
**
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

#include "qiviproxyserviceobject.h"
#include "qiviproxyserviceobject_p.h"

QT_BEGIN_NAMESPACE

QIviProxyServiceObjectPrivate::QIviProxyServiceObjectPrivate(QIviServiceInterface *interface)
    : m_serviceInterface(interface)
{
}

QIviProxyServiceObjectPrivate::QIviProxyServiceObjectPrivate(const QHash<QString, QIviFeatureInterface*> &interfaceMap)
    : m_serviceInterface(nullptr)
    , m_interfaceMap(interfaceMap)
{
}

/*!
    \class QIviProxyServiceObject
    \inmodule QtIviCore
    \brief QIviProxyServiceObject is a helper class to connect a Feature to already instantiated
    QIviServiceInterface classes

    In constrast to the QIviServiceObject which is created for you by the QIviServiceManager for
    every backend plugin, the QIviProxyServiceObject can be instantiated by the user and then
    manually set to the feature class.

    There are two ways to construct a QIviProxyServiceObject. The first takes a
    QIviServiceInterface pointer as input and can be used to load a backend which is derived from
    QIviServiceInterface and supposed to be loaded as a plugin, but is part of the same library and
    can be loaded directly instead. e.g. within a autotest.

    The second constructor takes a QHash<QString, QIviFeatureInterface*> and can be used to directly
    connect a feature class to the backend implementing the QIviFeatureInterface.

    \sa QIviAbstractFeature
*/

/*!
    Creates a new QIviProxyServiceObject for the given \a interface.

    This can be used to load a backend which is derived from QIviServiceInterface and supposed to
    be loaded as a plugin, but is part of the same library and can be loaded directly instead. e.g.
    within a autotest
*/
QIviProxyServiceObject::QIviProxyServiceObject(QIviServiceInterface *interface)
    : QIviServiceObject()
    , d_ptr(new QIviProxyServiceObjectPrivate(interface))
{
}

/*!
    Creates a new QIviProxyServiceObject for the given \a interfaceMap.

    This can be used to directly connect a feature class to the backend implementing the
    QIviFeatureInterface.
*/
QIviProxyServiceObject::QIviProxyServiceObject(const QHash<QString, QIviFeatureInterface*> &interfaceMap)
    : QIviServiceObject()
    , d_ptr(new QIviProxyServiceObjectPrivate(interfaceMap))
{
}

/*!
    \reimp
*/
QStringList QIviProxyServiceObject::interfaces() const
{
    Q_D(const QIviProxyServiceObject);
    if (d->m_serviceInterface)
        return d->m_serviceInterface->interfaces();

    return d->m_interfaceMap.keys();
}

/*!
    \reimp
*/
QIviFeatureInterface *QIviProxyServiceObject::interfaceInstance(const QString &interface) const
{
    Q_D(const QIviProxyServiceObject);
    if (d->m_serviceInterface)
        return d->m_serviceInterface->interfaceInstance(interface);

    return d->m_interfaceMap.value(interface);
}

QT_END_NAMESPACE
