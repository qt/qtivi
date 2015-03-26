#include "qtiviclimatecontrol.h"

#include "qtiviclimatecontrolbackendinterface.h"




QString QtIVIClimateControlBackendInterface::interfaceName = QLatin1String("com.pelagicore.ClimateControl");




QtIVIClimateZone::QtIVIClimateZone(Zone zone, QtIVIClimateControl *parent)
    : QObject(parent)
    , m_zone(zone)
    , m_hasTargetTemperature(false)
    , m_hasSeatCooler(false)
    , m_hasSeatHeater(false)
    , m_targetTemperature(0)
    , m_seatCooler(0)
    , m_seatHeater(0)
{

}

QtIVIClimateZone::Zone QtIVIClimateZone::zone() const
{
    return m_zone;
}

bool QtIVIClimateZone::hasTargetTemperature() const
{
    return m_hasTargetTemperature;
}

int QtIVIClimateZone::targetTemperature() const
{
    if (m_hasTargetTemperature)
        return m_targetTemperature;
    else
        return 0;
}

void QtIVIClimateZone::setTargetTemperature(int t)
{
    if (m_hasTargetTemperature)
        qobject_cast<QtIVIClimateControl*>(parent())->backend()->setTargetTemperature(m_zone, t);
    else
        qWarning() << "Trying to set ClimateZone::targetTemperature without a backend.";
}

bool QtIVIClimateZone::hasSeatCooler() const
{
    return m_hasSeatCooler;
}

int QtIVIClimateZone::seatCooler() const
{
    if (m_hasSeatCooler)
        return m_seatCooler;
    else
        return 0;
}

void QtIVIClimateZone::setSeatCooler(int t)
{
    if (t < 0 || t > 10) {
        qWarning() << "Trying to set ClimateZone::seatCooler to " << t << " which is out of range (0-10).";
        return;
    }

    if (m_hasSeatCooler)
        qobject_cast<QtIVIClimateControl*>(parent())->backend()->setSeatCooler(m_zone, t);
    else
        qWarning() << "Trying to set ClimateZone::seatCooler without a backend.";
}

bool QtIVIClimateZone::hasSeatHeater() const
{
    if (m_hasSeatHeater)
        return m_hasSeatHeater;
    else
        return 0;
}

int QtIVIClimateZone::seatHeater() const
{
    return m_seatHeater;
}

void QtIVIClimateZone::setSeatHeater(int t)
{
    if (t < 0 || t > 10) {
        qWarning() << "Trying to set ClimateZone::seatHeater to " << t << " which is out of range (0-10).";
        return;
    }

    if (m_hasSeatHeater)
        qobject_cast<QtIVIClimateControl*>(parent())->backend()->setSeatHeater(m_zone, t);
    else
        qWarning() << "Trying to set ClimateZone::seatHeater without a backend.";
}

void QtIVIClimateZone::onTargetTemperatureChanged(QtIVIClimateZone::Zone z, int t)
{
    if (z == m_zone) {
        m_targetTemperature = t;
        if (m_hasTargetTemperature)
            emit targetTemperatureChanged(m_seatHeater);
    }
}

void QtIVIClimateZone::onSeatCoolerChanged(QtIVIClimateZone::Zone z, int t)
{
    if (z == m_zone) {
        m_seatCooler = t;
        if (m_hasSeatCooler)
            emit seatCoolerChanged(m_seatCooler);
    }
}

void QtIVIClimateZone::onSeatHeaterChanged(QtIVIClimateZone::Zone z, int t)
{
    if (z == m_zone) {
        m_seatHeater = t;
        if (m_hasSeatHeater)
            emit seatHeaterChanged(m_seatHeater);
    }
}

void QtIVIClimateZone::setHasTargetTemperature(bool e)
{
    if(e != m_hasTargetTemperature) {
        m_hasTargetTemperature = e;
        emit hasTargetTemperatureChanged(m_hasTargetTemperature);

        onTargetTemperatureChanged(m_zone, targetTemperature());
    }
}

void QtIVIClimateZone::setHasSeatCooler(bool e)
{
    if(e != m_hasSeatCooler) {
        m_hasSeatCooler = e;
        emit hasSeatCoolerChanged(m_hasSeatCooler);

        onSeatCoolerChanged(m_zone, seatCooler());
    }
}

void QtIVIClimateZone::setHasSeatHeater(bool e)
{
    if(e != m_hasSeatHeater) {
        m_hasSeatHeater = e;
        emit hasSeatHeaterChanged(m_hasSeatHeater);

        onSeatHeaterChanged(m_zone, seatHeater());
    }
}





QtIVIClimateControl::QtIVIClimateControl(QObject *parent)
    : QtIVIAbstractFeature(QtIVIClimateControlBackendInterface::interfaceName, true, parent)
    , m_airflowDirection(QtIVIClimateControl::BiLevel)
    , m_airConditioning(false)
    , m_heater(false)
    , m_airRecirculation(false)
    , m_steeringWheelHeater(false)
    , m_fanSpeedLevel(0)
{
    QList<QtIVIClimateZone::Zone> zones;
    zones << QtIVIClimateZone::FrontLeft << QtIVIClimateZone::FrontCenter << QtIVIClimateZone::FrontRight
          << QtIVIClimateZone::RearLeft << QtIVIClimateZone::RearCenter << QtIVIClimateZone::RearRight;
    foreach(QtIVIClimateZone::Zone z, zones)
        m_zones[z] = new QtIVIClimateZone(z, this);
}

QtIVIClimateControl::AirflowDirection QtIVIClimateControl::airflowDirection() const
{
    return m_airflowDirection;
}

bool QtIVIClimateControl::isAirConditioningEnabled() const
{
    return m_airConditioning;
}

bool QtIVIClimateControl::isHeaterEnabled() const
{
    return m_heater;
}

bool QtIVIClimateControl::isAirRecirculationEnabled() const
{
    return m_airRecirculation;
}

bool QtIVIClimateControl::isSteeringWheelHeaterEnabled() const
{
    return m_steeringWheelHeater;
}

int QtIVIClimateControl::fanSpeedLevel() const
{
    return m_fanSpeedLevel;
}

QtIVIClimateZone *QtIVIClimateControl::climateZone(QtIVIClimateZone::Zone z) const
{
    return m_zones[z];
}

QtIVIClimateZone *QtIVIClimateControl::driver() const
{
    if (backend())
        return m_zones[backend()->driverZone()];
    else {
        qWarning() << "Returning driver zone without a backend, using default driver zone.";
        return m_zones[QtIVIClimateZone::FrontLeft];
    }
}

QtIVIClimateZone *QtIVIClimateControl::frontLeftZone() const
{
    return m_zones[QtIVIClimateZone::FrontLeft];
}

QtIVIClimateZone *QtIVIClimateControl::frontCenterZone() const
{
    return m_zones[QtIVIClimateZone::FrontCenter];
}

QtIVIClimateZone *QtIVIClimateControl::frontRightZone() const
{
    return m_zones[QtIVIClimateZone::FrontRight];
}

QtIVIClimateZone *QtIVIClimateControl::rearLeftZone() const
{
    return m_zones[QtIVIClimateZone::RearLeft];
}

QtIVIClimateZone *QtIVIClimateControl::rearCenterZone() const
{
    return m_zones[QtIVIClimateZone::RearCenter];
}

QtIVIClimateZone *QtIVIClimateControl::rearRightZone() const
{
    return m_zones[QtIVIClimateZone::RearRight];
}

void QtIVIClimateControl::setAirflowDirection(QtIVIClimateControl::AirflowDirection ad)
{
    if(backend())
        backend()->setAirflowDirection(ad);
}

void QtIVIClimateControl::setAirConditioningEnabled(bool e)
{
    if(backend())
        backend()->setAirConditioningEnabled(e);
}

void QtIVIClimateControl::setHeaterEnabled(bool e)
{
    if(backend())
        backend()->setHeaterEnabled(e);
}

void QtIVIClimateControl::setAirRecirculationEnabled(bool e)
{
    if(backend())
        backend()->setAirRecirculationEnabled(e);
}

void QtIVIClimateControl::setSteeringWheelHeaterEnabled(bool e)
{
    if(backend())
        backend()->setSteeringWheelHeaterEnabled(e);
}

void QtIVIClimateControl::setFanSpeedLevel(int fsl)
{
    if(backend())
        backend()->setFanSpeedLevel(fsl);
}

bool QtIVIClimateControl::acceptServiceObject(QtIVIServiceObject *so)
{
    return (qobject_cast<QtIVIClimateControlBackendInterface*>(so->interfaceInstance(QtIVIClimateControlBackendInterface::interfaceName)) != NULL);
}

void QtIVIClimateControl::connectToServiceObject(QtIVIServiceObject *so)
{
    QtIVIClimateControlBackendInterface *backend = qobject_cast<QtIVIClimateControlBackendInterface*>(so->interfaceInstance(QtIVIClimateControlBackendInterface::interfaceName));

    QList<QtIVIClimateZone::Zone> zones;
    zones << QtIVIClimateZone::FrontLeft << QtIVIClimateZone::FrontCenter << QtIVIClimateZone::FrontRight
          << QtIVIClimateZone::RearLeft << QtIVIClimateZone::RearCenter << QtIVIClimateZone::RearRight;
    foreach(QtIVIClimateZone::Zone z, zones) {
        connect(backend, SIGNAL(targetTemperatureChanged(QtIVIClimateZone::Zone, int)), m_zones[z], SLOT(onTargetTemperatureChanged(QtIVIClimateZone::Zone,int)));
        connect(backend, SIGNAL(seatCoolerChanged(QtIVIClimateZone::Zone, int)), m_zones[z], SLOT(onSeatCoolerChanged(QtIVIClimateZone::Zone,int)));
        connect(backend, SIGNAL(seatHeaterChanged(QtIVIClimateZone::Zone, int)), m_zones[z], SLOT(onSeatHeaterChanged(QtIVIClimateZone::Zone,int)));
        m_zones[z]->onTargetTemperatureChanged(z, backend->targetTemperature(z));
        m_zones[z]->onSeatCoolerChanged(z, backend->seatCooler(z));
        m_zones[z]->onSeatHeaterChanged(z, backend->seatHeater(z));
        /* Set the has properties last to ensure that the value is available when signalling */
        m_zones[z]->setHasTargetTemperature(backend->hasTargetTemperature(z));
        m_zones[z]->setHasSeatCooler(backend->hasSeatCooler(z));
        m_zones[z]->setHasSeatHeater(backend->hasSeatHeater(z));
    }

    connect(backend, SIGNAL(airflowDirectionChanged(QtIVIClimateControl::AirflowDirection)), this, SLOT(onAirflowDirectionChanged(AirflowDirection)));
    connect(backend, SIGNAL(airConditioningEnabledChanged(bool)), this, SLOT(onAirConditioningEnabledChanged(bool)));
    connect(backend, SIGNAL(heaterEnabledChanged(bool)), this, SLOT(onHeaterEnabledChanged(bool)));
    connect(backend, SIGNAL(airRecirculationEnabledChanged(bool)), this, SLOT(onAirRecirculationEnabledChanged(bool)));
    connect(backend, SIGNAL(steeringWheelHeaterEnabledChanged(bool)), this, SLOT(onSteeringWheelHeaterEnabledChanged(bool)));
    connect(backend, SIGNAL(fanSpeedLevelChanged(int)), this, SLOT(onFanSpeedLevelChanged(int)));

    onAirflowDirectionChanged(backend->airflowDirection());
    onAirConditioningEnabledChanged(backend->airConditioningEnabled());
    onHeaterEnabledChanged(backend->heaterEnabled());
    onAirRecirculationEnabledChanged(backend->airRecirculationEnabled());
    onSteeringWheelHeaterEnabledChanged(backend->steeringWheelHeaterEnabled());
    onFanSpeedLevelChanged(backend->fanSpeedLevel());
}

void QtIVIClimateControl::disconnectFromServiceObject(QtIVIServiceObject *so)
{
    QtIVIClimateControlBackendInterface *backend = qobject_cast<QtIVIClimateControlBackendInterface*>(so->interfaceInstance(QtIVIClimateControlBackendInterface::interfaceName));

    QList<QtIVIClimateZone::Zone> zones;
    zones << QtIVIClimateZone::FrontLeft << QtIVIClimateZone::FrontCenter << QtIVIClimateZone::FrontRight
          << QtIVIClimateZone::RearLeft << QtIVIClimateZone::RearCenter << QtIVIClimateZone::RearRight;
    foreach(QtIVIClimateZone::Zone z, zones) {
        disconnect(backend, SIGNAL(targetTemperatureChanged(QtIVIClimateZone::Zone, int)), m_zones[z], SLOT(onTargetTemperatureChanged(QtIVIClimateZone::Zone,int)));
        disconnect(backend, SIGNAL(seatCoolerChanged(QtIVIClimateZone::Zone, int)), m_zones[z], SLOT(onSeatCoolerChanged(QtIVIClimateZone::Zone,int)));
        disconnect(backend, SIGNAL(seatHeaterChanged(QtIVIClimateZone::Zone, int)), m_zones[z], SLOT(onSeatHeaterChanged(QtIVIClimateZone::Zone,int)));
    }

    disconnect(backend, SIGNAL(airflowDirectionChanged(QtIVIClimateControl::AirflowDirection)), this, SLOT(onAirflowDirectionChanged(AirflowDirection)));
    disconnect(backend, SIGNAL(airConditioningEnabledChanged(bool)), this, SLOT(onAirConditioningEnabledChanged(bool)));
    disconnect(backend, SIGNAL(heaterEnabledChanged(bool)), this, SLOT(onHeaterEnabledChanged(bool)));
    disconnect(backend, SIGNAL(airRecirculationEnabledChanged(bool)), this, SLOT(onAirRecirculationEnabledChanged(bool)));
    disconnect(backend, SIGNAL(steeringWheelHeaterEnabledChanged(bool)), this, SLOT(onSteeringWheelHeaterEnabledChanged(bool)));
    disconnect(backend, SIGNAL(fanSpeedLevelChanged(int)), this, SLOT(onFanSpeedLevelChanged(int)));
}

void QtIVIClimateControl::clearServiceObject()
{
    /* Safe defaults */
    onAirflowDirectionChanged(QtIVIClimateControl::BiLevel);
    onAirConditioningEnabledChanged(false);
    onHeaterEnabledChanged(false);
    onAirRecirculationEnabledChanged(false);
    onSteeringWheelHeaterEnabledChanged(false);
    onFanSpeedLevelChanged(0);

    QList<QtIVIClimateZone::Zone> zones;
    zones << QtIVIClimateZone::FrontLeft << QtIVIClimateZone::FrontCenter << QtIVIClimateZone::FrontRight
          << QtIVIClimateZone::RearLeft << QtIVIClimateZone::RearCenter << QtIVIClimateZone::RearRight;
    foreach(QtIVIClimateZone::Zone z, zones) {
        m_zones[z]->setHasTargetTemperature(false);
        m_zones[z]->setHasSeatCooler(false);
        m_zones[z]->setHasSeatHeater(false);
    }
}

void QtIVIClimateControl::onAirflowDirectionChanged(QtIVIClimateControl::AirflowDirection airflowDirection)
{
    m_airflowDirection = airflowDirection;
    emit airflowDirectionChanged(m_airflowDirection);
}

void QtIVIClimateControl::onAirConditioningEnabledChanged(bool airConditioning)
{
    m_airConditioning = airConditioning;
    emit airConditioningEnabledChanged(m_airConditioning);
}

void QtIVIClimateControl::onHeaterEnabledChanged(bool heater)
{
    m_heater = heater;
    emit heaterEnabledChanged(m_heater);
}

void QtIVIClimateControl::onAirRecirculationEnabledChanged(bool airRecirculation)
{
    m_airRecirculation = airRecirculation;
    emit airRecirculationEnabledChanged(m_airRecirculation);
}

void QtIVIClimateControl::onSteeringWheelHeaterEnabledChanged(bool steeringWheelHeater)
{
    m_steeringWheelHeater = steeringWheelHeater;
    emit steeringWheelHeaterEnabledChanged(m_steeringWheelHeater);
}

void QtIVIClimateControl::onFanSpeedLevelChanged(int fanSpeedLevel)
{
    m_fanSpeedLevel = fanSpeedLevel;
    emit fanSpeedLevelChanged(m_fanSpeedLevel);
}

QtIVIClimateControlBackendInterface *QtIVIClimateControl::backend() const
{
    if (serviceObject())
        return qobject_cast<QtIVIClimateControlBackendInterface*>(serviceObject()->interfaceInstance(QtIVIClimateControlBackendInterface::interfaceName));
    else
        return 0;
}
