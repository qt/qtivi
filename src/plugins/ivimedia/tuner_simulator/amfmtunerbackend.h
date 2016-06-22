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

#ifndef AMFMTUNERBACKEND_H
#define AMFMTUNERBACKEND_H

#include <QtIviMedia/QIviAmFmTunerBackendInterface>
#include <QtIviMedia/QIviTunerStation>

class AmFmStation : public QIviAmFmTunerStation
{
    Q_GADGET

public:
    virtual QString id() const { return m_id; }
    void setId(const QString &id) { m_id = id; }
    virtual QString name() const { return m_stationName; }
    virtual QString type() const { return "amfmstation"; }

    virtual QString stationName() const { return m_stationName; }
    void setStationName(const QString &name) { m_stationName = name; }

    virtual int frequency() const { return m_frequency; }
    void setFrequency(int frequency) { m_frequency = frequency; }

    virtual QIviAmFmTuner::Band band() const { return m_band; }
    void setBand(QIviAmFmTuner::Band band) { m_band = band; }

private:
    QString m_id;
    QString m_stationName;
    int m_frequency;
    QIviAmFmTuner::Band m_band;
};
Q_DECLARE_METATYPE(AmFmStation)

class AmFmTunerBackend : public QIviAmFmTunerBackendInterface
{
    Q_OBJECT
public:
    explicit AmFmTunerBackend(QObject *parent = 0);

    virtual void initialize() Q_DECL_OVERRIDE;
    virtual void setFrequency(int frequency) Q_DECL_OVERRIDE;
    virtual void setBand(QIviAmFmTuner::Band band) Q_DECL_OVERRIDE;
    virtual void stepUp() Q_DECL_OVERRIDE;
    virtual void stepDown() Q_DECL_OVERRIDE;
    virtual void seekUp() Q_DECL_OVERRIDE;
    virtual void seekDown() Q_DECL_OVERRIDE;

private:
    void setCurrentStation(const AmFmStation &station);
    int stationIndexFromFrequency(int frequency) const;
    AmFmStation stationAt(int frequency) const;

    QIviAmFmTuner::Band m_band;
    struct BandData {
        QVector<AmFmStation> m_stations;
        int m_stepSize;
        int m_frequency;
    };
    QHash<QIviAmFmTuner::Band, BandData> m_bandHash;

    friend class SearchAndBrowseBackend;
};

#endif // AMFMTUNERBACKEND_H
