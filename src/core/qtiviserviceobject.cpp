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


