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

#include <QtCore/qmetaobject.h>
#include <QtIVICore/qtiviserviceobject.h>

#include "qtiviabstractzonedfeature.h"
#include "qtivivehiclebackendinterface.h"

/*!
    \class QtIVIAbstractZonedFeature
    \inmodule QtIVIVehicleFunctions

    \since 5.6

    \brief The QtIVIAbstractZonedFeature is base class for all QtIVI vehicle features.

    QtIVIAbstractZonedFeature contains all feature attributes and zone handling.
*/

/*!
    \qmltype AbstractZonedFeature
    \instantiates QtIVIAbstractZonedFeature
    \inqmlmodule QtIVIVehicleFunctions 1.0
    \inherits AbstractFeature
    \brief The AbstractZonedFeature is not directly accessible. QML type provides
    base QML properties for each QML Vehicle feature like zone and error access.
*/

/*!
    Constructs a vehicle feature with a specific \a interface and \a zone.

    If \a parent is of type QtIVIAbstractZonedFeature, then the created instance
    uses parent for the backend connection. Parent is connected to the
    backend and will forward updates between child features and the backend.

    \sa QtIVIAbstractFeature
*/
QtIVIAbstractZonedFeature::QtIVIAbstractZonedFeature(const QString &interface, const QString &zone, QObject *parent)
    : QtIVIAbstractFeature(interface, parent)
    , m_zone(zone)
{
}

/*!
    Destroys vehicle feature
*/
QtIVIAbstractZonedFeature::~QtIVIAbstractZonedFeature()
{
}

/*!
    \reimp
*/
bool QtIVIAbstractZonedFeature::acceptServiceObject(QtIVIServiceObject *serviceObject)
{
    if (QtIVIAbstractZonedFeature* parentFeature = qobject_cast<QtIVIAbstractZonedFeature*>(parent()))
        return parentFeature->acceptServiceObject(serviceObject);
    else
        return serviceObject->interfaces().contains(interfaceName());
}

/*!
    \reimp
*/
void QtIVIAbstractZonedFeature::connectToServiceObject(QtIVIServiceObject *serviceObject)
{
    QtIVIVehicleBackendInterface *backend(0);
    if (QtIVIAbstractZonedFeature* parentFeature = qobject_cast<QtIVIAbstractZonedFeature*>(parent()))
        backend = parentFeature->backend();
    else
        backend = qobject_cast<QtIVIVehicleBackendInterface*>(serviceObject->interfaceInstance(interfaceName()));

    if (backend) {
        connect(backend, &QtIVIVehicleBackendInterface::errorChanged, this, &QtIVIAbstractZonedFeature::onErrorChanged);
        initializeZones();
    }
}

/*!
    \reimp
*/
void QtIVIAbstractZonedFeature::disconnectFromServiceObject(QtIVIServiceObject* so)
{
    QtIVIVehicleBackendInterface *backend = qobject_cast<QtIVIVehicleBackendInterface*>(so->interfaceInstance(interfaceName()));

    if (backend)
        disconnect(backend, 0, this, 0);
}

/*!
    \reimp
*/
void QtIVIAbstractZonedFeature::clearServiceObject()
{
}

/*!
   Returns pointer to the backend interface

   Returns parent backend if parent is QtIVIAbstractZonedFeature type.
   Returns zero if no backend connected.
*/
QtIVIVehicleBackendInterface *QtIVIAbstractZonedFeature::backend() const
{
    if (QtIVIAbstractZonedFeature* parentFeature = qobject_cast<QtIVIAbstractZonedFeature*>(parent())) {
        return parentFeature->backend();
    } else if (QtIVIServiceObject* so = serviceObject()) {
        return qobject_cast<QtIVIVehicleBackendInterface*>(so->interfaceInstance(interfaceName()));
    }
    return 0;
}


/*!
   \fn virtual QtIVIAbstractZonedFeature *QtIVIAbstractZonedFeature::createZoneFeature(const QString &zone) = 0

   Create new child feature to the given \a zone.

   Returns zero if feature can't be created for the given feature and zone.
*/

/*!
   \qmlproperty QString AbstractZonedFeature::zone

   \brief Name of the zone of this zoned feature.

   The zone can be given in the feature initialization. With this property it's
   possible to control only a single specific feature zone.

   This property is writable only before the backend is connected. When the backend is
   discovered and the component is verified to be valid, zone is not writable anymore.
   It's not recommended to change the zone after the initialization.

   \qml
   ClimateControl {
        zone: "FrontLeft"
        onAirConditioningChanged: {
            // Take action on front left A/C changes.
        }
   }
   \endqml
 */
/*!
   \property QtIVIAbstractZonedFeature::zone

   \brief Name of the zone of this zoned feature.

   The zone can be given in the feature initialization. With this property it's
   possible to control only a single specific feature zone.

   This property is writable only before the backend is connected. When the backend is
   discovered and the component is verified to be valid, zone is not writable anymore.
   It's not recommended to change the zone after the initialization.

   It's recommended to initialize the zone in the feature constructor:

   \code
   QtIVIClimateControl* climateControl = new QtIVIClimateControl("FrontLeft", this);
   climateControl->startAutoDiscovery();
   QString zone = climateControl->zone();
   \endcode
 */
QString QtIVIAbstractZonedFeature::zone() const
{
    return m_zone;
}

void QtIVIAbstractZonedFeature::setZone(const QString &zone)
{
    if (backend() || m_zone == zone)
        return;
    m_zone = zone;
    emit zoneChanged();
}

void QtIVIAbstractZonedFeature::initializeZones()
{
    if (!backend() || !zone().isEmpty())
        return;

    foreach (const QString &zone, backend()->availableZones()) {
        QtIVIAbstractZonedFeature* f = zoneAt(zone);
        if (!f) {
            if (this->zone() == zone)
                f = this;
            else
                f = createZoneFeature(zone);
            if (f) {
                m_zoneFeatures.append(f);
                m_zoneFeatureList.append(QVariant::fromValue(f));
                m_zoneFeatureMap.insert(f->zone(), QVariant::fromValue(f));
                emit availableZonesChanged(m_zoneFeatureMap.keys());
                emit zonesChanged();
            }
        }
    }
}

/*!
   \qmlproperty QStringList AbstractZonedFeature::availableZones

   List of the available zones.
 */
/*!
   \property QtIVIAbstractZonedFeature::availableZones

   List of the available zones.
 */
QStringList QtIVIAbstractZonedFeature::availableZones() const
{
    if (backend()) {
        return backend()->availableZones();
    }
    return QStringList();
}


/*!
   Returns the given \a zone instance of the feature.
*/
QtIVIAbstractZonedFeature *QtIVIAbstractZonedFeature::zoneAt(const QString &zone) const
{
    foreach (QtIVIAbstractZonedFeature *f, m_zoneFeatures)
        if (f->zone() == zone)
            return f;
    return 0;
}

/*!
   Returns all zone instances of the feature.
*/
QList<QtIVIAbstractZonedFeature*> QtIVIAbstractZonedFeature::zones() const
{
    return m_zoneFeatures;
}

/*!
   \qmlproperty QVariantMap AbstractZonedFeature::zoneAt

   Direct feature access to the given zone.

   \code
   feature.zoneAt.FrontLeft
   \endcode
 */
/*!
   \property QtIVIAbstractZonedFeature::zoneAt

   Direct feature access to the given zone.
 */
QVariantMap QtIVIAbstractZonedFeature::zoneFeatureMap() const
{
    return m_zoneFeatureMap;
}

/*!
   \qmlproperty QVariantList AbstractZonedFeature::zones

   Access to the feature zones model.

   \code
   model: feature.zones
   \endcode
 */
/*!
   \property QtIVIAbstractZonedFeature::zones

   Access to the feature zones model.
 */
QVariantList QtIVIAbstractZonedFeature::zoneFeatureList() const
{
    return m_zoneFeatureList;
}
