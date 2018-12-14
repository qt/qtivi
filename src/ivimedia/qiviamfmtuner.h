/****************************************************************************
**
** Copyright (C) 2019 Luxoft Sweden AB
** Copyright (C) 2018 Pelagicore AG
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

#ifndef QIVIAMFMTUNER_H
#define QIVIAMFMTUNER_H

#include <QtIviCore/QIviAbstractFeature>
#include <QtIviMedia/qtivimediaglobal.h>
#include <QVariant>

QT_BEGIN_NAMESPACE

class QIviAmFmTunerStation;
class QIviAmFmTunerPrivate;

class Q_QTIVIMEDIA_EXPORT QIviAmFmTuner : public QIviAbstractFeature
{
    Q_OBJECT

    Q_PROPERTY(int frequency READ frequency WRITE setFrequency NOTIFY frequencyChanged)
    Q_PROPERTY(int minimumFrequency READ minimumFrequency NOTIFY minimumFrequencyChanged)
    Q_PROPERTY(int maximumFrequency READ maximumFrequency NOTIFY maximumFrequencyChanged)
    Q_PROPERTY(int stepSize READ stepSize NOTIFY stepSizeChanged)
    Q_PROPERTY(QIviAmFmTuner::Band band READ band WRITE setBand NOTIFY bandChanged)
    Q_PROPERTY(QIviAmFmTunerStation station READ station NOTIFY stationChanged)
    Q_PROPERTY(bool scanRunning READ isScanRunning NOTIFY scanRunningChanged)

public:
    explicit QIviAmFmTuner(QObject *parent = nullptr);

    enum Band {
        AMBand,
        FMBand
    };
    Q_ENUM(Band)


    int frequency() const;
    int minimumFrequency() const;
    int maximumFrequency() const;
    int stepSize() const;
    Band band() const;
    QIviAmFmTunerStation station() const;
    bool isScanRunning() const;

    Q_INVOKABLE void tune(const QIviAmFmTunerStation &station);

public Q_SLOTS:
    void setFrequency(int frequency);
    void setBand(QIviAmFmTuner::Band band);
    void stepUp();
    void stepDown();
    void seekUp();
    void seekDown();
    void startScan();
    void stopScan();

Q_SIGNALS:
    void frequencyChanged(int frequency);
    void minimumFrequencyChanged(int minimumFrequency);
    void maximumFrequencyChanged(int maximumFrequency);
    void stepSizeChanged(int stepSize);
    void bandChanged(QIviAmFmTuner::Band band);
    void stationChanged(const QIviAmFmTunerStation &station);
    void scanRunningChanged(bool scanRunning);
    void scanStarted();
    void scanStopped();

protected:
    QIviAmFmTuner(QIviAmFmTunerPrivate &dd, QObject *parent = nullptr);

    virtual void connectToServiceObject(QIviServiceObject *serviceObject) override;
    virtual void clearServiceObject() override;

private:
    Q_DECLARE_PRIVATE(QIviAmFmTuner)
    Q_PRIVATE_SLOT(d_func(), void onFrequencyChanged(int frequency))
    Q_PRIVATE_SLOT(d_func(), void onBandChanged(QIviAmFmTuner::Band band))
    Q_PRIVATE_SLOT(d_func(), void onStationChanged(const QIviAmFmTunerStation &station))
    Q_PRIVATE_SLOT(d_func(), void onMinimumFrequencyChanged(int frequency))
    Q_PRIVATE_SLOT(d_func(), void onMaximumFrequencyChanged(int frequency))
    Q_PRIVATE_SLOT(d_func(), void onStepSizeChanged(int stepSize))
    Q_PRIVATE_SLOT(d_func(), void onScanStatusChanged(bool scanRunning))
};

QT_END_NAMESPACE

#endif // QIVIAMFMTUNER_H
