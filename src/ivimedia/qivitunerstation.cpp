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

#include "qivitunerstation.h"

QT_BEGIN_NAMESPACE

class QIviTunerStationPrivate : public QSharedData
{
public:
    QIviTunerStationPrivate()
        : m_frequency(-1)
    {}

    QIviTunerStationPrivate(const QIviTunerStationPrivate &other)
        : QSharedData(other)
        , m_stationName(other.m_stationName)
        , m_frequency(other.m_frequency)
    {}

    QString m_stationName;
    int m_frequency;
};

class QIviAmFmTunerStationPrivate : public QSharedData
{
public:
    QIviAmFmTunerStationPrivate()
        : m_band(QIviAmFmTuner::FMBand)
    {}

    QIviAmFmTunerStationPrivate(const QIviAmFmTunerStationPrivate &other)
        : QSharedData(other)
        , m_band(other.m_band)
    {}

    QIviAmFmTuner::Band m_band;
};

QT_END_NAMESPACE

QIviTunerStation::QIviTunerStation()
    : QIviSearchAndBrowseModelItem()
    , d(new QIviTunerStationPrivate)
{
}

QIviTunerStation::QIviTunerStation(const QIviTunerStation &rhs)
    : QIviSearchAndBrowseModelItem(rhs)
    , d(rhs.d)
{
}

QIviTunerStation &QIviTunerStation::operator=(const QIviTunerStation &rhs)
{
    QIviSearchAndBrowseModelItem::operator=(rhs);
    if (this != &rhs)
        d.operator=(rhs.d);
    return *this;
}

QIviTunerStation::~QIviTunerStation()
{
}

QString QIviTunerStation::stationName() const
{
    return d->m_stationName;
}

void QIviTunerStation::setStationName(const QString &stationName)
{
    d->m_stationName = stationName;
}

int QIviTunerStation::frequency() const
{
    return d->m_frequency;
}

void QIviTunerStation::setFrequency(int frequency)
{
    d->m_frequency = frequency;
}

QString QIviTunerStation::name() const
{
    return d->m_stationName;
}

QString QIviTunerStation::type() const
{
    return QLatin1String("tunerstation");
}

bool QIviTunerStation::operator==(const QIviTunerStation &other)
{
    return (QIviSearchAndBrowseModelItem::operator==(other) &&
            d->m_stationName == other.d->m_stationName &&
            d->m_frequency == other.d->m_frequency);
}

QIviAmFmTunerStation::QIviAmFmTunerStation()
    : QIviTunerStation()
    , d(new QIviAmFmTunerStationPrivate)
{
}

QIviAmFmTunerStation::QIviAmFmTunerStation(const QIviAmFmTunerStation &rhs)
    : QIviTunerStation(rhs)
    , d(rhs.d)
{
}

QIviAmFmTunerStation &QIviAmFmTunerStation::operator=(const QIviAmFmTunerStation &rhs)
{
    QIviTunerStation::operator=(rhs);
    if (this != &rhs)
        d.operator=(rhs.d);
    return *this;
}

QIviAmFmTunerStation::~QIviAmFmTunerStation()
{
}

QIviAmFmTuner::Band QIviAmFmTunerStation::band() const
{
    return d->m_band;
}

void QIviAmFmTunerStation::setBand(QIviAmFmTuner::Band band)
{
    d->m_band = band;
}

QString QIviAmFmTunerStation::type() const
{
    return QLatin1String("amfmtunerstation");
}

bool QIviAmFmTunerStation::operator==(const QIviAmFmTunerStation &other)
{
    return (QIviTunerStation::operator==(other) &&
            d->m_band == other.d->m_band);
}
