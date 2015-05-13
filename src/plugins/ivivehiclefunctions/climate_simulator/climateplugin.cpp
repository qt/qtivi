/************************************************************************************************
 * Copyright (c) 2012-2015 Pelagicore AG. All rights reserved.
 *
 * This software, including documentation, is protected by copyright controlled by Pelagicore AG.
 * All rights reserved. Copying, including reproducing, storing, adapting or translating, any or
 * all of this material requires prior written consent of Pelagicore AG Corporation. This material
 * also contains confidential information which may not be disclosed to others without the prior
 * written consent of Pelagicore AG.
 ************************************************************************************************/

#include "climateplugin.h"
#include "climatecontrolbackend.h"

#include <QtIVIVehicleFunctions/QtIVIClimateControlBackendInterface>
#include <QStringList>

ClimatePlugin::ClimatePlugin(QObject *parent) :
    QObject(parent),
    m_climate(new ClimateControlBackend(this))
{
}

QStringList ClimatePlugin::interfaces() const
{
    QStringList list;
    list << QtIVIClimateControlBackendInterface::interfaceName;
    return list;
}

QObject *ClimatePlugin::interfaceInstance(const QString &interface) const
{
    if (interface != QtIVIClimateControlBackendInterface::interfaceName)
        return 0;

    return m_climate;
}
