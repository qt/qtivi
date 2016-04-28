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

#ifndef QTIVIWINDOWCONTROLBACKENDINTERFACE_H
#define QTIVIWINDOWCONTROLBACKENDINTERFACE_H

#include <QtCore/QObject>

#include <QtIVICore/qtivizonedfeatureinterface.h>
#include <QtIVICore/qtivipropertyattribute.h>
#include <QtIVIVehicleFunctions/qtiviwindowcontrol.h>

QT_BEGIN_NAMESPACE

class Q_QTIVIVEHICLEFUNCTIONS_EXPORT QtIVIWindowControlBackendInterface : public QtIVIZonedFeatureInterface
{
    Q_OBJECT

public:
    QtIVIWindowControlBackendInterface(QObject *parent = 0);

    virtual void setHeaterMode(QtIVIWindowControl::HeaterMode value, const QString &zone) = 0;
    virtual void setBlindMode(QtIVIWindowControl::BlindMode value, const QString &zone) = 0;
    virtual void open(const QString &zone) = 0;
    virtual void close(const QString &zone) = 0;

Q_SIGNALS:
    void heaterModeChanged(QtIVIWindowControl::HeaterMode value, const QString &zone = QString());
    void heaterModeAttributeChanged(const QtIVIPropertyAttribute<QtIVIWindowControl::HeaterMode> &heaterMode, const QString &zone = QString());
    void heaterEnabledChanged(bool enabled, const QString &zone = QString());
    void heaterAttributeChanged(const QtIVIPropertyAttribute<bool> &heater, const QString &zone = QString());
    void blindModeChanged(QtIVIWindowControl::BlindMode value, const QString &zone = QString());
    void blindModeAttributeChanged(const QtIVIPropertyAttribute<QtIVIWindowControl::BlindMode> &blindMode, const QString &zone = QString());
    void blindStateChanged(QtIVIWindowControl::State value, const QString &zone = QString());
    void blindStateAttributeChanged(const QtIVIPropertyAttribute<QtIVIWindowControl::State> &blindState, const QString &zone = QString());
    void stateChanged(QtIVIWindowControl::State value, const QString &zone = QString());
    void stateAttributeChanged(const QtIVIPropertyAttribute<QtIVIWindowControl::State> &state, const QString &zone = QString());
};

QT_END_NAMESPACE

#endif // QTIVIWINDOWCONTROLBACKENDINTERFACE_H
