#include "climateplugin.h"
#include "climatecontrolbackend.h"

#include <QtIVIClimateControlBackendInterface>
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
