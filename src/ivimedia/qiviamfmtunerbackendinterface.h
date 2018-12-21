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

#ifndef QIVIAMFMTUNERBACKENDINTERFACE_H
#define QIVIAMFMTUNERBACKENDINTERFACE_H

#include <QtIviCore/QIviFeatureInterface>
#include <QtIviMedia/qtivimediaglobal.h>
#include <QtIviMedia/QIviAmFmTuner>

QT_BEGIN_NAMESPACE

class QIviPlayableItem;

class Q_QTIVIMEDIA_EXPORT QIviAmFmTunerBackendInterface : public QIviFeatureInterface
{
    Q_OBJECT

public:
    explicit QIviAmFmTunerBackendInterface(QObject *parent = nullptr);

    virtual void setFrequency(int frequency) = 0;
    virtual void setBand(QIviAmFmTuner::Band band) = 0;
    virtual void stepUp() = 0;
    virtual void stepDown() = 0;
    virtual void seekUp() = 0;
    virtual void seekDown() = 0;
    virtual void startScan() = 0;
    virtual void stopScan() = 0;

Q_SIGNALS:
    void frequencyChanged(int frequency);
    void minimumFrequencyChanged(int minimumFrequency);
    void maximumFrequencyChanged(int maximumFrequency);
    void stepSizeChanged(int stepSize);
    void bandChanged(QIviAmFmTuner::Band band);
    void stationChanged(const QIviAmFmTunerStation &station);
    void scanStatusChanged(bool scanRunning);
};

#define QIviAmFmTuner_iid "org.qt-project.qtivi.AmFmTuner/1.0"

QT_END_NAMESPACE

#endif // QIVIAMFMTUNERBACKENDINTERFACE_H
