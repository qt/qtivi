/****************************************************************************
**
** Copyright (C) 2016 Pelagicore AG
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

#include "qiviserviceobject.h"

#include <QUuid>

/*!
 * \class QIviServiceObject
 * \inmodule QtIviCore
 * \brief QIviServiceObject is the connection point to a Backend Service.
 *
 * QIviServiceObject provides you with a list of interfaces the Backend implements.
 *
 * By using interfaceInstance() a QObject implementing this interface will be returned.
 * The returned interface can contain signals that need to be connected to by the Feature
 * implementing this interface.
 *
 * \sa QIviAbstractFeature
 */


/*!
 * Constructor.
 *
 * \a parent is passed on to \l QObject.
 */
QIviServiceObject::QIviServiceObject(QObject *parent)
    : QObject(parent)
{

}

/*!
 * Destructor.
 */
QIviServiceObject::~QIviServiceObject()
{

}

/*!
 * \property QIviServiceObject::id
 * \brief A unique ID for the service object instance.
 *
 * Each service object has a unique ID. When subclassing, the id()
 * function can be overloaded to control the generation of the ID.
 */

/*!
 * The id() function can be overloaded to control the generation of
 * the unique ID used by this service object.
 *
 * By default, QUuid::createUuid() is used.
 */
QString QIviServiceObject::id() const
{
    static QUuid id = QUuid::createUuid();
    return id.toString();
}

/*!
 * \class QIviServiceInterface
 * \inmodule QtIviCore
 * \brief Interface class for services.
 *
 * The QIviServiceInterface class defines the interface of services registered with QIviServiceManager.
 *
 * Commonly, service objects inherit the concrete class QIviServiceObject instead of using QIviServiceInterface directly.
 *
 * \sa QIviServiceObject
 */

/*!
 * \fn QIviServiceInterface::~QIviServiceInterface()
 *
 * Destructs the QIviServiceInterface instance.
 */

/*!
 * \fn QStringList QIviServiceInterface::interfaces() const
 *
 * Returns a list of service interface names supported by the service object instance.
 */

/*!
 * \fn QObject* QIviServiceInterface::interfaceInstance(const QString& interface) const
 *
 * Returns an object implementing the service interface requested through \a interface.
 */
