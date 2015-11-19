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

#include "qtiviabstractzonedfeature.h"
#include "qtivivehiclebackendinterface.h"

/*!
 * \class QtIVIVehicleBackendInterface
 *
 * \brief The QtIVIVehicleBackendInterface defines the base interface for the feature
 * backends classes
 *
 * \ingroup backends
 * \inmodule QtIVIVehicleFunctions
 *
 * The QtIVIVehicleBackendInterface is the interface used by QtIVIAbstractZonedFeature and
 * it subclasses.
 **
 * \sa QtIVIAbstractZonedFeature
 */

/*!
 * \fn QtIVIVehicleBackendInterface::QtIVIVehicleBackendInterface(QObject *parent=0)
 *
 * Constructs a backend base interface.
 *
 * The \a parent is sent to the QObject constructor.
 */

/*!
 * \fn QString QtIVIVehicleBackendInterface::interfaceName() const
 *
 * Returns a name of the interface that backend implements
 *
 * \sa QtIVIAbstractZonedFeature::interfaceName()
 */

/*!
 * \fn QString QtIVIVehicleBackendInterface::availableZones() const
 *
 * Returns a list of supported zone names.
 *
 * The returned names must be valid QML property names, i.e. \c {[a-z_][A-Za-z0-9_]*}.
 */

/*!
 * \fn void QtIVIVehicleBackendInterface::initializeAttributes() const
 *
 * Called from the client to initialize attributes right after it's connected
 *
 * In this function all supported attributes for each zone need to be emitted with
 * the initialized value.
 */

/*!
 * \fn void errorChanged(QtIVIAbstractZonedFeature::Error error, const QString &message = QString())
 *
 * The signal is emitted when \a error occurs in the backend. Error \a message is optional.
 */

/*!
    \class QtIVIAbstractZonedFeature
    \inmodule QtIVI
    \since 5.6

    \brief The QtIVIAbstractZonedFeature is base class for all QtIVI vehicle features.

    QtIVIAbstractZonedFeature contains all feature attributes and zone handling.
*/

/*!
    Constructs vehicle feature by \a interface and \a zone

    If parent is QtIVIAbstractZonedFeature type then created instance
    uses parent backend connection. Parent is connected to the
    backend and will forward updates between children features
    and backend.
*/
QtIVIAbstractZonedFeature::QtIVIAbstractZonedFeature(const QString &interface, const QString &zone, bool autoDiscovery, QObject *parent)
    : QtIVIAbstractFeature(interface, autoDiscovery, parent)
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
   Accept service object
*/
bool QtIVIAbstractZonedFeature::acceptServiceObject(QtIVIServiceObject *serviceObject)
{
    if (QtIVIAbstractZonedFeature* parentFeature = qobject_cast<QtIVIAbstractZonedFeature*>(parent()))
        return parentFeature->acceptServiceObject(serviceObject);
    else
        return serviceObject->interfaces().contains(interfaceName());
}

/*!
   Connect to service object backend
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
   Disconnect from service object backend
*/
void QtIVIAbstractZonedFeature::disconnectFromServiceObject(QtIVIServiceObject* so)
{
    QtIVIVehicleBackendInterface *backend = qobject_cast<QtIVIVehicleBackendInterface*>(so->interfaceInstance(interfaceName()));

    if (backend)
        disconnect(backend, 0, this, 0);
}

/*!
   Clear service object
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
  /fn QtIVIAbstractZonedFeature *QtIVIAbstractZonedFeature::createZoneFeature(const QString &zone)

  Create new \a child feature to the given \a zone.

   Returns zero if feature can't be created for the given feature and zone.
*/

/*!
   Returns zone
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
   Update error from backend
*/
void QtIVIAbstractZonedFeature::onErrorChanged(QtIVIAbstractZonedFeature::Error error, const QString &message)
{
    setError(error, message);
}

/*!
   Returns list of available zones
*/
QStringList QtIVIAbstractZonedFeature::availableZones() const
{
    if (backend()) {
        return backend()->availableZones();
    }
    return QStringList();
}


/*!
   Returns given \a zone instance of the feature
*/
QtIVIAbstractZonedFeature *QtIVIAbstractZonedFeature::zoneAt(const QString &zone) const
{
    foreach (QtIVIAbstractZonedFeature *f, m_zoneFeatures)
        if (f->zone() == zone)
            return f;
    return 0;
}

/*!
   Returns all zone instances of the feature
*/
QList<QtIVIAbstractZonedFeature*> QtIVIAbstractZonedFeature::zones() const
{
    return m_zoneFeatures;
}

QVariantMap QtIVIAbstractZonedFeature::zoneFeatureMap() const
{
    return m_zoneFeatureMap;
}

QVariantList QtIVIAbstractZonedFeature::zoneFeatureList() const
{
    return m_zoneFeatureList;
}

void QtIVIAbstractZonedFeature::setError(QtIVIAbstractZonedFeature::Error error, const QString &message)
{
    m_error = error;
    if (m_error == QtIVIAbstractZonedFeature::NoError)
        m_errorMessage.clear();
    m_errorMessage = errorText() + " " + message;
    emit errorChanged(m_error, m_errorMessage);
}

/*!
   Returns last error
*/
QtIVIAbstractZonedFeature::Error QtIVIAbstractZonedFeature::error() const
{
    return m_error;
}

/*!
   Returns last error with the message

   Empty if no error
*/
QString QtIVIAbstractZonedFeature::errorMessage() const
{
    return m_errorMessage;
}

/*!
   Returns attribute error in text

   Empty if no error
*/
QString QtIVIAbstractZonedFeature::errorText() const
{
    if (m_error == QtIVIAbstractZonedFeature::NoError)
        return QString();
    QMetaEnum metaEnum = this->metaObject()->enumerator(0);
    return QLatin1String(metaEnum.valueToKey(m_error));
}
