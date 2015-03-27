/************************************************************************************************
 * Copyright (c) 2012-2015 Pelagicore AG. All rights reserved.
 *
 * This software, including documentation, is protected by copyright controlled by Pelagicore AG.
 * All rights reserved. Copying, including reproducing, storing, adapting or translating, any or
 * all of this material requires prior written consent of Pelagicore AG Corporation. This material
 * also contains confidential information which may not be disclosed to others without the prior
 * written consent of Pelagicore AG.
 ************************************************************************************************/

#ifndef QTIVISERVICEINTERFACE_H
#define QTIVISERVICEINTERFACE_H

#include <QtPlugin>

class QtIVIServiceInterface {

public:

    virtual QStringList interfaces() const = 0;
    virtual QObject* interfaceInstance(const QString& interface) const = 0;
};

Q_DECLARE_INTERFACE(QtIVIServiceInterface, "com.pelagicore.QtIVIServiceInterface")
Q_DECLARE_METATYPE(QtIVIServiceInterface*)

#endif // QTIVISERVICEINTERFACE_H

