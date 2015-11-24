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

#include "qtiviserviceobject.h"

#include <QUuid>

/*!
 * \class QtIVIServiceObject
 * \inmodule QtIVICore
 * \brief QtIVIServiceObject is the connection point to a Backend Service
 *
 * QtIVIServiceObject provides you with a list of interfaces the Backend implements.
 *
 * By using interfaceInstance() a QObject implementing this interface will be returned.
 * The returned interface can contain signals which needs to be connected to by the Feature
 * implementing this interface.
 *
 * \sa QtIVIAbstractFeature
 */

/*!
 * \property QtIVIServiceObject::id
 * \brief A unique ID for the service object instance.
 *
 * Each service object has a unique ID. When sub-classing, the \l QtIVIServiceObject::id()
 * function can be overloaded to control how the ID is generad.
 */

/*!
 * Constructor.
 *
 * \a parent is passed on to \l QObject.
 */
QtIVIServiceObject::QtIVIServiceObject(QObject *parent)
    : QObject(parent)
{

}

/*!
 * Destructor.
 */
QtIVIServiceObject::~QtIVIServiceObject()
{

}

/*!
 * The id() function can be overloaded to control the generation of
 * the unique ID used by this Object.
 *
 * By default QUuid::createUuid() is used.
 */
QString QtIVIServiceObject::id() const
{
    static QUuid id = QUuid::createUuid();
    return id.toString();
}

/*!
 * \class QtIVIServiceInterface
 * \inmodule QtIVICore
 * \brief Interface class for services
 *
 * The QtIVIServiceInterface class defines the interface of services registered at the QtIVIServiceManager.
 *
 * Commonly, service objects inherit the concrete class QtIVIServiceObject instead of using QtIVIServiceInterface directly.
 *
 * \sa QtIVIServiceObject
 */

/*!
 * \fn QStringList QtIVIServiceInterface::interfaces() const
 * \inmodule QtIVICore
 *
 * Returns a list of service interface names supported by the service object instance.
 */

/*!
 * \fn QObject* QtIVIServiceInterface::interfaceInstance(const QString& interface) const
 * \inmodule QtIVICore
 *
 * Returns an object implementing the service interface requested through \a interface.
 */
