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

#ifndef QIVIAMFMTUNER_P_H
#define QIVIAMFMTUNER_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists purely as an
// implementation detail. This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include "private/qtivimediaglobal_p.h"
#include "private/qiviabstractfeature_p.h"

#include "qiviamfmtuner.h"
#include "qiviamfmtunerbackendinterface.h"

#include <QtIviMedia/QIviTunerStation>

QT_BEGIN_NAMESPACE

class QIviAmFmTunerPrivate : public QIviAbstractFeaturePrivate
{
public:
    QIviAmFmTunerPrivate(const QString &interface, QIviAmFmTuner *parent);

    void clearToDefaults();
    void onFrequencyChanged(int frequency);
    void onMinimumFrequencyChanged(int frequency);
    void onMaximumFrequencyChanged(int frequency);
    void onStepSizeChanged(int stepSize);
    void onBandChanged(QIviAmFmTuner::Band band);
    void onStationChanged(const QIviAmFmTunerStation &station);
    void onScanStatusChanged(bool scanRunning);

    QIviAmFmTunerBackendInterface *tunerBackend() const;

    QIviAmFmTuner * const q_ptr;
    int m_frequency;
    int m_minimumFrequency;
    int m_maximumFrequency;
    int m_stepSize;
    QIviAmFmTuner::Band m_band;
    QIviAmFmTunerStation m_station;
    bool m_scanRunning;

    Q_DECLARE_PUBLIC(QIviAmFmTuner)
};

QT_END_NAMESPACE

#endif // QIVIAMFMTUNER_P_H
