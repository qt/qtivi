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

QtIVIServiceObject::QtIVIServiceObject(QObject *parent)
    : QObject(parent)
{

}

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


