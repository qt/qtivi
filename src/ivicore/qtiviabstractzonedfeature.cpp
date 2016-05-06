/****************************************************************************
**
** Copyright (C) 2016 Pelagicore AG
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtIVI module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL-QTAS$
** Commercial License Usage
** Licensees holding valid commercial Qt Automotive Suite licenses may use
** this file in accordance with the commercial license agreement provided
** with the Software or, alternatively, in accordance with the terms
** contained in a written agreement between you and The Qt Company.  For
** licensing terms and conditions see https://www.qt.io/terms-conditions.
** For further information use the contact form at https://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl-3.0.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 2.0 or (at your option) the GNU General
** Public license version 3 or any later version approved by the KDE Free
** Qt Foundation. The licenses are as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL2 and LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-2.0.html and
** https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
** SPDX-License-Identifier: LGPL-3.0
**
****************************************************************************/

#include <QtCore/qmetaobject.h>
#include <QtIVICore/qtiviserviceobject.h>

#include "qtiviabstractzonedfeature.h"
#include "qtiviabstractzonedfeature_p.h"
#include "qtivizonedfeatureinterface.h"

QtIVIAbstractZonedFeaturePrivate::QtIVIAbstractZonedFeaturePrivate(const QString &interface, const QString &zone, QtIVIAbstractFeature *parent)
    : QtIVIAbstractFeaturePrivate(interface, parent)
    , m_zone(zone)
{}

/*!
    \class QtIVIAbstractZonedFeature
    \inmodule QtIVICore

    \since 5.6

    \brief The QtIVIAbstractZonedFeature is the base class for all QtIVI vehicle features.

    QtIVIAbstractZonedFeature contains all feature attributes and zone handling.
*/

/*!
    \qmltype AbstractZonedFeature
    \instantiates QtIVIAbstractZonedFeature
    \inqmlmodule QtIVICore 1.0
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
    : QtIVIAbstractFeature(*new QtIVIAbstractZonedFeaturePrivate(interface, zone, this), parent)
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
    else if (serviceObject)
        return serviceObject->interfaces().contains(interfaceName());
    return false;
}

/*!
    \reimp
*/
void QtIVIAbstractZonedFeature::connectToServiceObject(QtIVIServiceObject *serviceObject)
{
    QtIVIZonedFeatureInterface *backend(0);
    if (QtIVIAbstractZonedFeature* parentFeature = qobject_cast<QtIVIAbstractZonedFeature*>(parent()))
        backend = parentFeature->backend();
    else if (serviceObject)
        backend = qobject_cast<QtIVIZonedFeatureInterface*>(serviceObject->interfaceInstance(interfaceName()));

    if (backend) {
        connect(backend, &QtIVIZonedFeatureInterface::errorChanged, this, &QtIVIAbstractZonedFeature::onErrorChanged);
        initializeZones();
    }
}

/*!
    \reimp
*/
void QtIVIAbstractZonedFeature::clearServiceObject()
{
    Q_D(QtIVIAbstractZonedFeature);
    qDeleteAll(d->m_zoneFeatures);
    d->m_zoneFeatures.clear();
    d->m_zoneFeatureList.clear();
    d->m_zoneFeatureMap.clear();
}

/*!
   Returns pointer to the backend \a interface

   Returns parent backend if parent is QtIVIAbstractZonedFeature type.
   Returns zero if no backend connected.
*/
QtIVIZonedFeatureInterface *QtIVIAbstractZonedFeature::backend(const QString &interface) const
{
    QString iface = interface;
    if (iface.isEmpty())
        iface = interfaceName();

    if (QtIVIAbstractZonedFeature* parentFeature = qobject_cast<QtIVIAbstractZonedFeature*>(parent())) {
        return parentFeature->backend();
    } else if (QtIVIServiceObject* so = serviceObject()) {
        return qobject_cast<QtIVIZonedFeatureInterface*>(so->interfaceInstance(iface));
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
    Q_D(const QtIVIAbstractZonedFeature);
    return d->m_zone;
}

void QtIVIAbstractZonedFeature::setZone(const QString &zone)
{
    Q_D(QtIVIAbstractZonedFeature);
    if (backend() || d->m_zone == zone)
        return;
    d->m_zone = zone;
    emit zoneChanged();
}

void QtIVIAbstractZonedFeature::initializeZones()
{
    if (!backend() || !zone().isEmpty())
        return;

    Q_D(QtIVIAbstractZonedFeature);
    foreach (const QString &zone, backend()->availableZones()) {
        QtIVIAbstractZonedFeature* f = zoneAt(zone);
        if (!f) {
            if (this->zone() == zone)
                f = this;
            else
                f = createZoneFeature(zone);
            if (f) {
                d->m_zoneFeatures.append(f);
                d->m_zoneFeatureList.append(QVariant::fromValue(f));
                d->m_zoneFeatureMap.insert(f->zone(), QVariant::fromValue(f));
                emit availableZonesChanged(d->m_zoneFeatureMap.keys());
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
    Q_D(const QtIVIAbstractZonedFeature);
    foreach (QtIVIAbstractZonedFeature *f, d->m_zoneFeatures)
        if (f->zone() == zone)
            return f;
    return 0;
}

/*!
   Returns all zone instances of the feature.
*/
QList<QtIVIAbstractZonedFeature*> QtIVIAbstractZonedFeature::zones() const
{
    Q_D(const QtIVIAbstractZonedFeature);
    return d->m_zoneFeatures;
}

QtIVIAbstractZonedFeature::QtIVIAbstractZonedFeature(QtIVIAbstractZonedFeaturePrivate &dd, QObject *parent)
    : QtIVIAbstractFeature(dd, parent)
{
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
    Q_D(const QtIVIAbstractZonedFeature);
    return d->m_zoneFeatureMap;
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
    Q_D(const QtIVIAbstractZonedFeature);
    return d->m_zoneFeatureList;
}
