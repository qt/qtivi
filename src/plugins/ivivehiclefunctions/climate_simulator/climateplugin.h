/************************************************************************************************
 * Copyright (c) 2012-2015 Pelagicore AG. All rights reserved.
 *
 * This software, including documentation, is protected by copyright controlled by Pelagicore AG.
 * All rights reserved. Copying, including reproducing, storing, adapting or translating, any or
 * all of this material requires prior written consent of Pelagicore AG Corporation. This material
 * also contains confidential information which may not be disclosed to others without the prior
 * written consent of Pelagicore AG.
 ************************************************************************************************/

#ifndef CLIMATEPLUGIN_H
#define CLIMATEPLUGIN_H

#include <QtIVICore/QtIVIServiceInterface>

class ClimateControlBackend;

class ClimatePlugin : public QObject, QtIVIServiceInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "com.pelagicore.QtIVIServiceInterface" FILE "climate_simulator.json")
    Q_INTERFACES(QtIVIServiceInterface)

public:
    ClimatePlugin(QObject *parent = 0);

    QStringList interfaces() const;
    QObject* interfaceInstance(const QString& interface) const;

private:
    ClimateControlBackend* m_climate;
};

#endif // CLIMATEPLUGIN_H
