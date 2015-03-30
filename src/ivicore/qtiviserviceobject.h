/************************************************************************************************
 * Copyright (c) 2012-2015 Pelagicore AG. All rights reserved.
 *
 * This software, including documentation, is protected by copyright controlled by Pelagicore AG.
 * All rights reserved. Copying, including reproducing, storing, adapting or translating, any or
 * all of this material requires prior written consent of Pelagicore AG Corporation. This material
 * also contains confidential information which may not be disclosed to others without the prior
 * written consent of Pelagicore AG.
 ************************************************************************************************/

#ifndef QTIVISERVICEOBJECT_H
#define QTIVISERVICEOBJECT_H

#include <QObject>

#include "qtiviserviceinterface.h"
#include "qtiviglobal.h"

class Q_QTIVICORE_EXPORT QtIVIServiceObject : public QObject, public QtIVIServiceInterface
{
    Q_OBJECT
    Q_INTERFACES(QtIVIServiceInterface)

    Q_PROPERTY(QString id READ id CONSTANT)

public:
    explicit QtIVIServiceObject(QObject *parent = 0);
    virtual ~QtIVIServiceObject();

    virtual QString id() const;

private:
};

#endif // QTIVISERVICEOBJECT_H
