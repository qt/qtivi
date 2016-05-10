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

#ifndef QTIVIWINDOWCONTROL_H
#define QTIVIWINDOWCONTROL_H

#include <QtIVICore/QtIVIAbstractZonedFeature>
#include <QtIVICore/QtIVIProperty>
#include <QtIVIVehicleFunctions/qtivivehiclefunctionsglobal.h>

QT_BEGIN_NAMESPACE

class QtIVIWindowControlBackendInterface;
class QtIVIWindowControlPrivate;

static const QLatin1String QtIVIStringWindowControlInterfaceName("com.qt-project.qtivi.WindowControl");

class Q_QTIVIVEHICLEFUNCTIONS_EXPORT QtIVIWindowControl : public QtIVIAbstractZonedFeature
{
    Q_OBJECT

    Q_PROPERTY(QtIVIProperty *heaterMode READ heaterModeProperty CONSTANT)
    Q_PROPERTY(QtIVIProperty *heater READ heaterProperty CONSTANT)
    Q_PROPERTY(QtIVIProperty *blindMode READ blindModeProperty CONSTANT)
    Q_PROPERTY(QtIVIProperty *blindState READ blindStateProperty CONSTANT)
    Q_PROPERTY(QtIVIProperty *state READ stateProperty CONSTANT)

public:

    enum HeaterMode {
        HeaterOn,
        HeaterOff,
        AutoHeater
    };
    Q_ENUM(HeaterMode)

    enum BlindMode {
        BlindOpen,
        BlindClosed,
        AutoBlind
    };
    Q_ENUM(BlindMode)

    enum State {
        FullyOpen,
        Open,
        Closed
    };
    Q_ENUM(State)

    QtIVIWindowControl(const QString &zone=QString(), QObject *parent=0);
    ~QtIVIWindowControl();

    QtIVIWindowControl::HeaterMode heaterMode() const;
    QtIVIPropertyAttribute<QtIVIWindowControl::HeaterMode> heaterModeAttribute() const;
    QtIVIProperty *heaterModeProperty() const;
    bool isHeaterEnabled() const;
    QtIVIPropertyAttribute<bool> heaterAttribute() const;
    QtIVIProperty *heaterProperty() const;
    QtIVIWindowControl::BlindMode blindMode() const;
    QtIVIPropertyAttribute<QtIVIWindowControl::BlindMode> blindModeAttribute() const;
    QtIVIProperty *blindModeProperty() const;
    QtIVIWindowControl::State blindState() const;
    QtIVIPropertyAttribute<QtIVIWindowControl::State> blindStateAttribute() const;
    QtIVIProperty *blindStateProperty() const;
    QtIVIWindowControl::State state() const;
    QtIVIPropertyAttribute<QtIVIWindowControl::State> stateAttribute() const;
    QtIVIProperty *stateProperty() const;

public Q_SLOTS:
    void setHeaterMode(QtIVIWindowControl::HeaterMode value);
    void setBlindMode(QtIVIWindowControl::BlindMode value);
    void open();
    void close();

Q_SIGNALS:
    void heaterModeChanged(QtIVIWindowControl::HeaterMode value);
    void heaterModeAttributeChanged(const QtIVIPropertyAttribute<QtIVIWindowControl::HeaterMode> &attribute);
    void heaterEnabledChanged(bool enabled);
    void heaterAttributeChanged(const QtIVIPropertyAttribute<bool> &attribute);
    void blindModeChanged(QtIVIWindowControl::BlindMode value);
    void blindModeAttributeChanged(const QtIVIPropertyAttribute<QtIVIWindowControl::BlindMode> &attribute);
    void blindStateChanged(QtIVIWindowControl::State value);
    void blindStateAttributeChanged(const QtIVIPropertyAttribute<QtIVIWindowControl::State> &attribute);
    void stateChanged(QtIVIWindowControl::State value);
    void stateAttributeChanged(const QtIVIPropertyAttribute<QtIVIWindowControl::State> &attribute);

protected:
    QtIVIWindowControl(QtIVIWindowControlPrivate &dd, QObject *parent = Q_NULLPTR);
    virtual QtIVIAbstractZonedFeature *createZoneFeature(const QString &zone) Q_DECL_OVERRIDE;

    virtual void connectToServiceObject(QtIVIServiceObject *serviceObject) Q_DECL_OVERRIDE;
    virtual void disconnectFromServiceObject(QtIVIServiceObject *serviceObject) Q_DECL_OVERRIDE;
    virtual void clearServiceObject() Q_DECL_OVERRIDE;

private:
    Q_DECLARE_PRIVATE(QtIVIWindowControl)
    Q_PRIVATE_SLOT(d_func(), void onHeaterModeChanged(QtIVIWindowControl::HeaterMode heaterMode, const QString &zone))
    Q_PRIVATE_SLOT(d_func(), void onHeaterModeAttributeChanged(const QtIVIPropertyAttribute<QtIVIWindowControl::HeaterMode> &attribute, const QString &zone))
    Q_PRIVATE_SLOT(d_func(), void onHeaterEnabledChanged(bool heater, const QString &zone))
    Q_PRIVATE_SLOT(d_func(), void onHeaterAttributeChanged(const QtIVIPropertyAttribute<bool> &attribute, const QString &zone))
    Q_PRIVATE_SLOT(d_func(), void onBlindModeChanged(QtIVIWindowControl::BlindMode blindMode, const QString &zone))
    Q_PRIVATE_SLOT(d_func(), void onBlindModeAttributeChanged(const QtIVIPropertyAttribute<QtIVIWindowControl::BlindMode> &attribute, const QString &zone))
    Q_PRIVATE_SLOT(d_func(), void onBlindStateChanged(QtIVIWindowControl::State blindState, const QString &zone))
    Q_PRIVATE_SLOT(d_func(), void onBlindStateAttributeChanged(const QtIVIPropertyAttribute<QtIVIWindowControl::State> &attribute, const QString &zone))
    Q_PRIVATE_SLOT(d_func(), void onStateChanged(QtIVIWindowControl::State state, const QString &zone))
    Q_PRIVATE_SLOT(d_func(), void onStateAttributeChanged(const QtIVIPropertyAttribute<QtIVIWindowControl::State> &attribute, const QString &zone))
};

QT_END_NAMESPACE

Q_DECLARE_METATYPE(QtIVIPropertyAttribute<QtIVIWindowControl::HeaterMode>)
Q_DECLARE_METATYPE(QtIVIPropertyAttribute<QtIVIWindowControl::BlindMode>)
Q_DECLARE_METATYPE(QtIVIPropertyAttribute<QtIVIWindowControl::State>)

#endif // QTIVIWINDOWCONTROL_H
