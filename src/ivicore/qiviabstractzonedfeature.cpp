/****************************************************************************
**
** Copyright (C) 2016 Pelagicore AG
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtIvi module of the Qt Toolkit.
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
#include <QtIviCore/qiviserviceobject.h>

#include "qiviabstractzonedfeature.h"
#include "qiviabstractzonedfeature_p.h"
#include "qivizonedfeatureinterface.h"

QIviAbstractZonedFeaturePrivate::QIviAbstractZonedFeaturePrivate(const QString &interface, const QString &zone, QIviAbstractFeature *parent)
    : QIviAbstractFeaturePrivate(interface, parent)
    , m_zone(zone)
{}

QIviAbstractZonedFeaturePrivate::~QIviAbstractZonedFeaturePrivate()
{
}

/*!
    \class QIviAbstractZonedFeature
    \inmodule QtIviCore

    \since 5.6

    \brief The QIviAbstractZonedFeature is the base class for all QtIvi vehicle features.

    QIviAbstractZonedFeature contains all feature attributes and zone handling.
*/

/*!
    \qmltype AbstractZonedFeature
    \instantiates QIviAbstractZonedFeature
    \inqmlmodule QtIvi 1.0
    \inherits AbstractFeature
    \brief The AbstractZonedFeature is not directly accessible. The QML type provides
    base QML properties for each QML Vehicle feature like zone and error access.
*/

/*!
    Constructs a vehicle feature with a specific \a interface and \a zone.

    If \a parent is of type QIviAbstractZonedFeature, then the created instance
    uses parent for the backend connection. Parent is connected to the
    backend and will forward updates between child features and the backend.

    \sa QIviAbstractFeature
*/
QIviAbstractZonedFeature::QIviAbstractZonedFeature(const QString &interface, const QString &zone, QObject *parent)
    : QIviAbstractFeature(*new QIviAbstractZonedFeaturePrivate(interface, zone, this), parent)
{
}

/*!
    Destroys vehicle feature
*/
QIviAbstractZonedFeature::~QIviAbstractZonedFeature()
{
}

/*!
    \reimp
*/
bool QIviAbstractZonedFeature::acceptServiceObject(QIviServiceObject *serviceObject)
{
    if (QIviAbstractZonedFeature *parentFeature = qobject_cast<QIviAbstractZonedFeature*>(parent()))
        return parentFeature->acceptServiceObject(serviceObject);
    else if (serviceObject)
        return serviceObject->interfaces().contains(interfaceName());
    return false;
}

/*!
    \reimp
*/
void QIviAbstractZonedFeature::connectToServiceObject(QIviServiceObject *serviceObject)
{
    QIviZonedFeatureInterface *backend(0);
    if (QIviAbstractZonedFeature* parentFeature = qobject_cast<QIviAbstractZonedFeature*>(parent()))
        backend = parentFeature->backend();
    else if (serviceObject)
        backend = qobject_cast<QIviZonedFeatureInterface*>(serviceObject->interfaceInstance(interfaceName()));

    if (backend) {
        connect(backend, &QIviZonedFeatureInterface::errorChanged, this, &QIviAbstractZonedFeature::onErrorChanged);
        initializeZones();
    }
}

/*!
    \reimp
*/
void QIviAbstractZonedFeature::clearServiceObject()
{
    Q_D(QIviAbstractZonedFeature);
    qDeleteAll(d->m_zoneFeatures);
    d->m_zoneFeatures.clear();
    d->m_zoneFeatureList.clear();
    d->m_zoneFeatureMap.clear();
}

/*!
   Returns pointer to the backend \a interface

   Returns parent backend if parent is QIviAbstractZonedFeature type.
   Returns zero if no backend connected.
*/
QIviZonedFeatureInterface *QIviAbstractZonedFeature::backend(const QString &interface) const
{
    QString iface = interface;
    if (iface.isEmpty())
        iface = interfaceName();

    if (QIviAbstractZonedFeature *parentFeature = qobject_cast<QIviAbstractZonedFeature*>(parent())) {
        return parentFeature->backend();
    } else if (QIviServiceObject *so = serviceObject()) {
        return qobject_cast<QIviZonedFeatureInterface*>(so->interfaceInstance(iface));
    }
    return 0;
}


/*!
   \fn virtual QIviAbstractZonedFeature *QIviAbstractZonedFeature::createZoneFeature(const QString &zone) = 0

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
   \property QIviAbstractZonedFeature::zone

   \brief Name of the zone of this zoned feature.

   The zone can be given in the feature initialization. With this property it's
   possible to control only a single specific feature zone.

   This property is writable only before the backend is connected. When the backend is
   discovered and the component is verified to be valid, zone is not writable anymore.
   It's not recommended to change the zone after the initialization.

   It's recommended to initialize the zone in the feature constructor:

   \code
   QIviClimateControl *climateControl = new QIviClimateControl("FrontLeft", this);
   climateControl->startAutoDiscovery();
   QString zone = climateControl->zone();
   \endcode
 */
QString QIviAbstractZonedFeature::zone() const
{
    Q_D(const QIviAbstractZonedFeature);
    return d->m_zone;
}

void QIviAbstractZonedFeature::setZone(const QString &zone)
{
    Q_D(QIviAbstractZonedFeature);
    if (backend() || d->m_zone == zone)
        return;
    d->m_zone = zone;
    emit zoneChanged();
}

void QIviAbstractZonedFeature::initializeZones()
{
    if (!backend() || !zone().isEmpty())
        return;

    Q_D(QIviAbstractZonedFeature);
    const auto zones = backend()->availableZones();
    for (const QString &zone : zones) {
        QIviAbstractZonedFeature *f = zoneAt(zone);
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
   \property QIviAbstractZonedFeature::availableZones

   List of the available zones.
 */
QStringList QIviAbstractZonedFeature::availableZones() const
{
    if (backend()) {
        return backend()->availableZones();
    }
    return QStringList();
}


/*!
   Returns the given \a zone instance of the feature.
*/
QIviAbstractZonedFeature *QIviAbstractZonedFeature::zoneAt(const QString &zone) const
{
    Q_D(const QIviAbstractZonedFeature);
    for (QIviAbstractZonedFeature *f : d->m_zoneFeatures)
        if (f->zone() == zone)
            return f;
    return 0;
}

/*!
   Returns all zone instances of the feature.
*/
QList<QIviAbstractZonedFeature*> QIviAbstractZonedFeature::zones() const
{
    Q_D(const QIviAbstractZonedFeature);
    return d->m_zoneFeatures;
}

QIviAbstractZonedFeature::QIviAbstractZonedFeature(QIviAbstractZonedFeaturePrivate &dd, QObject *parent)
    : QIviAbstractFeature(dd, parent)
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
   \property QIviAbstractZonedFeature::zoneAt

   Direct feature access to the given zone.
 */
QVariantMap QIviAbstractZonedFeature::zoneFeatureMap() const
{
    Q_D(const QIviAbstractZonedFeature);
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
   \property QIviAbstractZonedFeature::zones

   Access to the feature zones model.
 */
QVariantList QIviAbstractZonedFeature::zoneFeatureList() const
{
    Q_D(const QIviAbstractZonedFeature);
    return d->m_zoneFeatureList;
}
