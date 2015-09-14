/************************************************************************************************
 * Copyright (c) 2012-2015 Pelagicore AG. All rights reserved.
 *
 * This software, including documentation, is protected by copyright controlled by Pelagicore AG.
 * All rights reserved. Copying, including reproducing, storing, adapting or translating, any or
 * all of this material requires prior written consent of Pelagicore AG Corporation. This material
 * also contains confidential information which may not be disclosed to others without the prior
 * written consent of Pelagicore AG.
 ************************************************************************************************/

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
