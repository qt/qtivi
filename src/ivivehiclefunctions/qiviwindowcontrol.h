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

#ifndef QIVIWINDOWCONTROL_H
#define QIVIWINDOWCONTROL_H

#include <QtIviCore/QIviAbstractZonedFeature>
#include <QtIviCore/QIviProperty>
#include <QtIviVehicleFunctions/qtivivehiclefunctionsglobal.h>

QT_BEGIN_NAMESPACE

class QIviWindowControlBackendInterface;
class QIviWindowControlPrivate;

static const QLatin1String QIviStringWindowControlInterfaceName("com.qt-project.qtivi.WindowControl");

class Q_QTIVIVEHICLEFUNCTIONS_EXPORT QIviWindowControl : public QIviAbstractZonedFeature
{
    Q_OBJECT

    Q_PROPERTY(QIviProperty *heaterMode READ heaterModeProperty CONSTANT)
    Q_PROPERTY(QIviProperty *heater READ heaterProperty CONSTANT)
    Q_PROPERTY(QIviProperty *blindMode READ blindModeProperty CONSTANT)
    Q_PROPERTY(QIviProperty *blindState READ blindStateProperty CONSTANT)
    Q_PROPERTY(QIviProperty *state READ stateProperty CONSTANT)

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

    QIviWindowControl(const QString &zone=QString(), QObject *parent = Q_NULLPTR);
    ~QIviWindowControl();

    QIviWindowControl::HeaterMode heaterMode() const;
    QIviPropertyAttribute<QIviWindowControl::HeaterMode> heaterModeAttribute() const;
    QIviProperty *heaterModeProperty() const;
    bool isHeaterEnabled() const;
    QIviPropertyAttribute<bool> heaterAttribute() const;
    QIviProperty *heaterProperty() const;
    QIviWindowControl::BlindMode blindMode() const;
    QIviPropertyAttribute<QIviWindowControl::BlindMode> blindModeAttribute() const;
    QIviProperty *blindModeProperty() const;
    QIviWindowControl::State blindState() const;
    QIviPropertyAttribute<QIviWindowControl::State> blindStateAttribute() const;
    QIviProperty *blindStateProperty() const;
    QIviWindowControl::State state() const;
    QIviPropertyAttribute<QIviWindowControl::State> stateAttribute() const;
    QIviProperty *stateProperty() const;

public Q_SLOTS:
    void setHeaterMode(QIviWindowControl::HeaterMode value);
    void setBlindMode(QIviWindowControl::BlindMode value);
    void open();
    void close();

Q_SIGNALS:
    void heaterModeChanged(QIviWindowControl::HeaterMode value);
    void heaterModeAttributeChanged(const QIviPropertyAttribute<QIviWindowControl::HeaterMode> &attribute);
    void heaterEnabledChanged(bool enabled);
    void heaterAttributeChanged(const QIviPropertyAttribute<bool> &attribute);
    void blindModeChanged(QIviWindowControl::BlindMode value);
    void blindModeAttributeChanged(const QIviPropertyAttribute<QIviWindowControl::BlindMode> &attribute);
    void blindStateChanged(QIviWindowControl::State value);
    void blindStateAttributeChanged(const QIviPropertyAttribute<QIviWindowControl::State> &attribute);
    void stateChanged(QIviWindowControl::State value);
    void stateAttributeChanged(const QIviPropertyAttribute<QIviWindowControl::State> &attribute);

protected:
    QIviWindowControl(QIviWindowControlPrivate &dd, QObject *parent = Q_NULLPTR);
    virtual QIviAbstractZonedFeature *createZoneFeature(const QString &zone) Q_DECL_OVERRIDE;

    virtual void connectToServiceObject(QIviServiceObject *serviceObject) Q_DECL_OVERRIDE;
    virtual void disconnectFromServiceObject(QIviServiceObject *serviceObject) Q_DECL_OVERRIDE;
    virtual void clearServiceObject() Q_DECL_OVERRIDE;

private:
    Q_DECLARE_PRIVATE(QIviWindowControl)
    Q_PRIVATE_SLOT(d_func(), void onHeaterModeChanged(QIviWindowControl::HeaterMode heaterMode, const QString &zone))
    Q_PRIVATE_SLOT(d_func(), void onHeaterModeAttributeChanged(const QIviPropertyAttribute<QIviWindowControl::HeaterMode> &attribute, const QString &zone))
    Q_PRIVATE_SLOT(d_func(), void onHeaterEnabledChanged(bool heater, const QString &zone))
    Q_PRIVATE_SLOT(d_func(), void onHeaterAttributeChanged(const QIviPropertyAttribute<bool> &attribute, const QString &zone))
    Q_PRIVATE_SLOT(d_func(), void onBlindModeChanged(QIviWindowControl::BlindMode blindMode, const QString &zone))
    Q_PRIVATE_SLOT(d_func(), void onBlindModeAttributeChanged(const QIviPropertyAttribute<QIviWindowControl::BlindMode> &attribute, const QString &zone))
    Q_PRIVATE_SLOT(d_func(), void onBlindStateChanged(QIviWindowControl::State blindState, const QString &zone))
    Q_PRIVATE_SLOT(d_func(), void onBlindStateAttributeChanged(const QIviPropertyAttribute<QIviWindowControl::State> &attribute, const QString &zone))
    Q_PRIVATE_SLOT(d_func(), void onStateChanged(QIviWindowControl::State state, const QString &zone))
    Q_PRIVATE_SLOT(d_func(), void onStateAttributeChanged(const QIviPropertyAttribute<QIviWindowControl::State> &attribute, const QString &zone))
};

QT_END_NAMESPACE

Q_DECLARE_METATYPE(QIviPropertyAttribute<QIviWindowControl::HeaterMode>)
Q_DECLARE_METATYPE(QIviPropertyAttribute<QIviWindowControl::BlindMode>)
Q_DECLARE_METATYPE(QIviPropertyAttribute<QIviWindowControl::State>)

#endif // QIVIWINDOWCONTROL_H
