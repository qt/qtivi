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

#ifndef QIVIWINDOWCONTROL_P_H
#define QIVIWINDOWCONTROL_P_H

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

#include <QtIviCore/QIviProperty>
#include "private/qtiviabstractzonedfeature_p.h"

#include "qtiviwindowcontrol.h"

QT_BEGIN_NAMESPACE

class QIviWindowControlPrivate : public QIviAbstractZonedFeaturePrivate
{
public:
    QIviWindowControlPrivate(const QString &interface, const QString &zone, QIviWindowControl *parent);

    void init();
    void clearToDefaults();

    void onHeaterModeChanged(QIviWindowControl::HeaterMode heaterMode, const QString &zone);
    void onHeaterModeAttributeChanged(const QIviPropertyAttribute<QIviWindowControl::HeaterMode> &attribute, const QString &zone);
    void onHeaterEnabledChanged(bool heater, const QString &zone);
    void onHeaterAttributeChanged(const QIviPropertyAttribute<bool> &attribute, const QString &zone);
    void onBlindModeChanged(QIviWindowControl::BlindMode blindMode, const QString &zone);
    void onBlindModeAttributeChanged(const QIviPropertyAttribute<QIviWindowControl::BlindMode> &attribute, const QString &zone);
    void onBlindStateChanged(QIviWindowControl::State blindState, const QString &zone);
    void onBlindStateAttributeChanged(const QIviPropertyAttribute<QIviWindowControl::State> &attribute, const QString &zone);
    void onStateChanged(QIviWindowControl::State state, const QString &zone);
    void onStateAttributeChanged(const QIviPropertyAttribute<QIviWindowControl::State> &attribute, const QString &zone);


    QIviWindowControlBackendInterface *windowControlBackend();

    QIviWindowControl::HeaterMode m_heaterMode;
    QIviPropertyAttribute<QIviWindowControl::HeaterMode> m_heaterModeAttribute;
    QIviProperty *m_heaterModeProperty;
    bool m_heater;
    QIviPropertyAttribute<bool> m_heaterAttribute;
    QIviProperty *m_heaterProperty;
    QIviWindowControl::BlindMode m_blindMode;
    QIviPropertyAttribute<QIviWindowControl::BlindMode> m_blindModeAttribute;
    QIviProperty *m_blindModeProperty;
    QIviWindowControl::State m_blindState;
    QIviPropertyAttribute<QIviWindowControl::State> m_blindStateAttribute;
    QIviProperty *m_blindStateProperty;
    QIviWindowControl::State m_state;
    QIviPropertyAttribute<QIviWindowControl::State> m_stateAttribute;
    QIviProperty *m_stateProperty;

    QIviWindowControl * const q_ptr;
    Q_DECLARE_PUBLIC(QIviWindowControl)
};

QT_END_NAMESPACE

#endif // QIVIWINDOWCONTROL_P_H
