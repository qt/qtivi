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

#include "qiviamfmtuner.h"
#include "qiviamfmtuner_p.h"
#include <QtIviCore/QIviServiceObject>
#include <QtDebug>

QIviAmFmTunerPrivate::QIviAmFmTunerPrivate(const QString &interface, QIviAmFmTuner *parent)
    : QIviAbstractFeaturePrivate(interface, parent)
    , q_ptr(parent)
    , m_frequency(-1)
    , m_band(QIviAmFmTuner::FMBand)
{
    qRegisterMetaType<QIviTunerStation>();
    qRegisterMetaType<QIviAmFmTunerStation>();
}

void QIviAmFmTunerPrivate::init()
{
}

void QIviAmFmTunerPrivate::clearToDefaults()
{
    m_frequency = -1;
    m_band = QIviAmFmTuner::FMBand;
    m_station = QVariant();
}

void QIviAmFmTunerPrivate::onFrequencyChanged(int frequency)
{
    if (m_frequency == frequency)
        return;

    Q_Q(QIviAmFmTuner);
    m_frequency = frequency;
    emit q->frequencyChanged(frequency);
}

void QIviAmFmTunerPrivate::onBandChanged(QIviAmFmTuner::Band band)
{
    if (m_band == band)
        return;

    Q_Q(QIviAmFmTuner);
    m_band = band;
    emit q->bandChanged(band);
}

void QIviAmFmTunerPrivate::onStationChanged(const QVariant &station)
{
    const QIviAmFmTunerStation *st = stationItem(station);
    if (st && m_station == station)
        return;

    Q_Q(QIviAmFmTuner);
    m_station = station;
    emit q->stationChanged(station);
}

//TODO replace me by a generic function for all classes
const QIviAmFmTunerStation *QIviAmFmTunerPrivate::stationItem(const QVariant &item)
{
    const void *data = item.constData();

    QMetaType type(item.userType());
    if (!type.flags().testFlag(QMetaType::IsGadget)) {
        qCritical() << "The passed QVariant needs to use the Q_GADGET macro";
        return nullptr;
    }

    const QMetaObject *mo = type.metaObject();
    while (mo) {
        if (mo->className() == QIviAmFmTunerStation::staticMetaObject.className())
            return reinterpret_cast<const QIviAmFmTunerStation*>(data);
        mo = mo->superClass();
    }

    qCritical() << "The passed QVariant is not derived from QIviPlayableItem";
    return nullptr;
}

QIviAmFmTunerBackendInterface *QIviAmFmTunerPrivate::tunerBackend() const
{
    Q_Q(const QIviAmFmTuner);
    QIviServiceObject *so = q->serviceObject();
    if (so)
        return qobject_cast<QIviAmFmTunerBackendInterface*>(so->interfaceInstance(QIviStringAmFmTunerInterfaceName));

    return nullptr;
}

/*!
    \class QIviAmFmTuner
    \inmodule QtIviMedia
    \brief Provides a interface to control the am fm tuner

    The QIviAmFmTuner provides the methods to control a am/fm tuner.
*/

/*!
    \qmltype AmFmTuner
    \instantiates QIviAmFmTuner
    \inqmlmodule QtIvi.Media
    \inherits AbstractFeature
    \brief Provides a interface to control the am fm tuner

    The AmFmTuner provides the methods to control a am/fm tuner.
*/

/*!
   \enum QIviAmFmTuner::Band
   \value AMBand
          The AM Band is based on the Amplitude Modulation technique and can range from 520 to 1610 kHz (1710 kHz).
          The step size is usually between 9 or 10 kHz.
   \value FMBand
          The FM Band is based on the Frequency Modulation technique and can range from 87.5 to 108.0 MHz.
          The step size is usually 100 kHz.
*/

/*!
   Constructs a QIviAmFmTuner.

   The \a parent argument is passed on to the \l QIviAbstractFeature base class.
*/
QIviAmFmTuner::QIviAmFmTuner(QObject *parent)
    : QIviAbstractFeature(*new QIviAmFmTunerPrivate(QIviStringAmFmTunerInterfaceName, this), parent)
{
    Q_D(QIviAmFmTuner);
    d->init();
}

/*!
    \qmlproperty int AmFmTuner::frequency
    \brief The current frequency of the tuner.
 */
/*!
    \property QIviAmFmTuner::frequency
    \brief The current frequency of the tuner.
 */
int QIviAmFmTuner::frequency() const
{
    Q_D(const QIviAmFmTuner);
    return d->m_frequency;
}

/*!
    \qmlproperty enumeration AmFmTuner::band
    \brief The current band of the tuner.

    Available values are:
    \value AMBand
           The AM Band is based on the Amplitude Modulation technique and can range from 520 to 1610 kHz (1710 kHz).
           The step size is usually between 9 or 10 kHz.
    \value FMBand
           The FM Band is based on the Frequency Modulation technique and can range from 87.5 to 108.0 MHz.
           The step size is usually 100 kHz.
 */
/*!
    \property QIviAmFmTuner::band
    \brief The current band of the tuner.
 */
QIviAmFmTuner::Band QIviAmFmTuner::band() const
{
    Q_D(const QIviAmFmTuner);
    return d->m_band;
}

/*!
    \qmlproperty AmFmTunerStation AmFmTuner::station
    \brief The currently tuned station.
 */
/*!
    \property QIviAmFmTuner::station
    \brief The currently tuned station.
 */
QVariant QIviAmFmTuner::station() const
{
    Q_D(const QIviAmFmTuner);
    return d->m_station;
}

/*!
    \qmlmethod AmFmTuner::tune(AmFmTunerStation station)

    Tunes to the provided \a station.
*/

/*!
    \fn void QIviAmFmTuner::tune(const QVariant &station)

    Tunes to the provided \a station.
*/
//TODO Change this to use the real class, but for this we need to refactor the Q_GADGET classes to real classes with copy constructors and propably QSharedData
void QIviAmFmTuner::tune(const QVariant &station)
{
    Q_D(QIviAmFmTuner);
    const QIviAmFmTunerStation *stationItem = d->stationItem(station);
    if (!stationItem)
        return;

    if (stationItem->band() != d->m_band)
        setBand(stationItem->band());
    if (stationItem->frequency() != d->m_frequency)
        setFrequency(stationItem->frequency());
}

void QIviAmFmTuner::setFrequency(int frequency)
{
    Q_D(QIviAmFmTuner);
    QIviAmFmTunerBackendInterface *backend = d->tunerBackend();
    if (!backend) {
        qWarning("Can't set the frequency without a connected backend");
        return;
    }

    backend->setFrequency(frequency);
}

void QIviAmFmTuner::setBand(QIviAmFmTuner::Band band)
{
    Q_D(QIviAmFmTuner);
    QIviAmFmTunerBackendInterface *backend = d->tunerBackend();
    if (!backend) {
        qWarning("Can't set the band without a connected backend");
        return;
    }

    backend->setBand(band);
}

/*!
    \qmlmethod AmFmTuner::stepUp()

    Increases the frequency by the current bands step size.
*/

/*!
    \fn void QIviAmFmTuner::stepUp()

    Increases the frequency by the current bands step size.
*/
void QIviAmFmTuner::stepUp()
{
    Q_D(QIviAmFmTuner);
    QIviAmFmTunerBackendInterface *backend = d->tunerBackend();
    if (!backend) {
        qWarning("Can't step up without a connected backend");
        return;
    }

    backend->stepUp();
}

/*!
    \qmlmethod AmFmTuner::stepDown()

    Decreases the frequency by the current bands step size.
*/

/*!
    \fn void QIviAmFmTuner::stepDown()

    Decreases the frequency by the current bands step size.
*/
void QIviAmFmTuner::stepDown()
{
    Q_D(QIviAmFmTuner);
    QIviAmFmTunerBackendInterface *backend = d->tunerBackend();
    if (!backend) {
        qWarning("Can't step down without a connected backend");
        return;
    }

    backend->stepDown();
}

/*!
    \qmlmethod AmFmTuner::seekUp()

    Seeks the next available Station in upwards direction and tunes it.

    If the end of the band where hit without finding a station, the search will be continued from the beginning of the band.
*/

/*!
    \fn void QIviAmFmTuner::seekUp()

    Seeks the next available Station in upwards direction and tunes it.

    If the end of the band where hit without finding a station, the search will be continued from the beginning of the band.
*/
void QIviAmFmTuner::seekUp()
{
    Q_D(QIviAmFmTuner);
    QIviAmFmTunerBackendInterface *backend = d->tunerBackend();
    if (!backend) {
        qWarning("Can't seek up without a connected backend");
        return;
    }

    backend->seekUp();
}

/*!
    \qmlmethod AmFmTuner::seekDown()

    Seeks the next available Station in downwards direction and tunes it.

    If the beginning of the band where hit without finding a station, the search will be continued from the end of the band.
*/

/*!
    \fn void QIviAmFmTuner::seekDown()

    Seeks the next available Station in downwards direction and tunes it.

    If the beginning of the band where hit without finding a station, the search will be continued from the end of the band.
*/
void QIviAmFmTuner::seekDown()
{
    Q_D(QIviAmFmTuner);
    QIviAmFmTunerBackendInterface *backend = d->tunerBackend();
    if (!backend) {
        qWarning("Can't seek down without a connected backend");
        return;
    }

    backend->seekDown();
}

/*!
    \internal
 */
QIviAmFmTuner::QIviAmFmTuner(QIviAmFmTunerPrivate &dd, QObject *parent)
    : QIviAbstractFeature(dd, parent)
{
    Q_D(QIviAmFmTuner);
    d->init();
}

/*!
    \reimp
 */
bool QIviAmFmTuner::acceptServiceObject(QIviServiceObject *serviceObject)
{
    return serviceObject->interfaces().contains(QIviStringAmFmTunerInterfaceName);
}

/*!
    \reimp
 */
void QIviAmFmTuner::connectToServiceObject(QIviServiceObject *serviceObject)
{
    Q_UNUSED(serviceObject);

    Q_D(QIviAmFmTuner);

    QIviAmFmTunerBackendInterface *backend = d->tunerBackend();
    if (!backend)
        return;

    QObjectPrivate::connect(backend, &QIviAmFmTunerBackendInterface::frequencyChanged,
                            d, &QIviAmFmTunerPrivate::onFrequencyChanged);
    QObjectPrivate::connect(backend, &QIviAmFmTunerBackendInterface::bandChanged,
                            d, &QIviAmFmTunerPrivate::onBandChanged);
    QObjectPrivate::connect(backend, &QIviAmFmTunerBackendInterface::stationChanged,
                            d, &QIviAmFmTunerPrivate::onStationChanged);
    backend->initialize();
}

/*!
    \reimp
 */
void QIviAmFmTuner::disconnectFromServiceObject(QIviServiceObject *serviceObject)
{
    QIviAmFmTunerBackendInterface *backend = qobject_cast<QIviAmFmTunerBackendInterface*>(serviceObject->interfaceInstance(QIviStringAmFmTunerInterfaceName));

    if (backend)
        disconnect(backend, 0, this, 0);
}

/*!
    \reimp
 */
void QIviAmFmTuner::clearServiceObject()
{
    Q_D(QIviAmFmTuner);
    d->clearToDefaults();
}

#include "moc_qiviamfmtuner.cpp"
