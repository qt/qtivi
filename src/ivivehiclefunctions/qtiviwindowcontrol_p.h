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

#ifndef QTIVIWINDOWCONTROL_P_H
#define QTIVIWINDOWCONTROL_P_H

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

#include <QtIVICore/QtIVIProperty>
#include "private/qtiviabstractzonedfeature_p.h"

#include "qtiviwindowcontrol.h"

QT_BEGIN_NAMESPACE

class QtIVIWindowControlPrivate : public QtIVIAbstractZonedFeaturePrivate
{
public:
    QtIVIWindowControlPrivate(const QString &interface, const QString &zone, QtIVIWindowControl *parent);

    void init();
    void clearToDefaults();

    void onHeaterModeChanged(QtIVIWindowControl::HeaterMode heaterMode, const QString &zone);
    void onHeaterModeAttributeChanged(const QtIVIPropertyAttribute<QtIVIWindowControl::HeaterMode> &attribute, const QString &zone);
    void onHeaterEnabledChanged(bool heater, const QString &zone);
    void onHeaterAttributeChanged(const QtIVIPropertyAttribute<bool> &attribute, const QString &zone);
    void onBlindModeChanged(QtIVIWindowControl::BlindMode blindMode, const QString &zone);
    void onBlindModeAttributeChanged(const QtIVIPropertyAttribute<QtIVIWindowControl::BlindMode> &attribute, const QString &zone);
    void onBlindStateChanged(QtIVIWindowControl::State blindState, const QString &zone);
    void onBlindStateAttributeChanged(const QtIVIPropertyAttribute<QtIVIWindowControl::State> &attribute, const QString &zone);
    void onStateChanged(QtIVIWindowControl::State state, const QString &zone);
    void onStateAttributeChanged(const QtIVIPropertyAttribute<QtIVIWindowControl::State> &attribute, const QString &zone);


    QtIVIWindowControlBackendInterface *windowControlBackend();

    QtIVIWindowControl::HeaterMode m_heaterMode;
    QtIVIPropertyAttribute<QtIVIWindowControl::HeaterMode> m_heaterModeAttribute;
    QtIVIProperty *m_heaterModeProperty;
    bool m_heater;
    QtIVIPropertyAttribute<bool> m_heaterAttribute;
    QtIVIProperty *m_heaterProperty;
    QtIVIWindowControl::BlindMode m_blindMode;
    QtIVIPropertyAttribute<QtIVIWindowControl::BlindMode> m_blindModeAttribute;
    QtIVIProperty *m_blindModeProperty;
    QtIVIWindowControl::State m_blindState;
    QtIVIPropertyAttribute<QtIVIWindowControl::State> m_blindStateAttribute;
    QtIVIProperty *m_blindStateProperty;
    QtIVIWindowControl::State m_state;
    QtIVIPropertyAttribute<QtIVIWindowControl::State> m_stateAttribute;
    QtIVIProperty *m_stateProperty;

    QtIVIWindowControl * const q_ptr;
    Q_DECLARE_PUBLIC(QtIVIWindowControl)
};

QT_END_NAMESPACE

#endif // QTIVIWINDOWCONTROL_P_H
