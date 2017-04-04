/****************************************************************************
**
** Copyright (C) 2017 Pelagicore AG
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

#include "qiviwindowcontrolbackendinterface.h"

QT_BEGIN_NAMESPACE

/*!
    \class QIviWindowControlBackendInterface
    \inmodule QtIviVehicleFunctions
    \ingroup backends
    \inherits QIviZonedFeatureInterface
    \keyword org.qt-project.qtivi.WindowControl/1.0
    \brief The QIviWindowControlBackendInterface defines the interface for backends to the
    QIviWindowControl feature class.

    The QIviWindowControlBackendInterface is the interface used by \l QIviWindowControl

    The interface is discovered by a \l QIviWindowControl object, which connects to it and sets up
    the connections to it.

    \sa QIviWindowControl
*/

/*!
    \fn QIviWindowControlBackendInterface::QIviWindowControlBackendInterface(QObject *parent = nullptr)

    Constructs a backend interface.

    The \a parent is sent to the QObject constructor.
*/
QIviWindowControlBackendInterface::QIviWindowControlBackendInterface(QObject *parent)
    : QIviZonedFeatureInterface(parent)
{
}

/*!
    \fn virtual void QIviWindowControlBackendInterface::setHeaterMode(QIviWindowControl::HeaterMode value, const QString &zone) = 0

    Sets the heater mode of \a zone to \a value. The available modes can be defined using the attribute system.

    This method is expected to emit a \l heaterModeChanged() signal when the internal state changes
    due to this function call. The signal is even expected to be emitted if the given \a value is not supported and no
    actual change takes place.

    \sa heaterModeChanged()
*/

/*!
    \fn virtual void QIviWindowControlBackendInterface::setBlindMode(QIviWindowControl::BlindMode value, const QString &zone) = 0

    Sets the blind mode of \a zone to \a value.  The available modes can be defined using the attribute system.

    This method is expected to emit a \l blindModeChanged() signal when the internal state changes
    due to this function call. The signal is even expected to be emitted if the given \a value is not supported and no
    actual change takes place.

    \sa blindModeChanged()
*/

/*!
    \fn virtual void QIviWindowControlBackendInterface::open(const QString &zone) = 0

    Opens the window identified by \a zone. Whether the window can be opened is controlled by the attribute system.

    \sa stateChanged()
*/

/*!
    \fn virtual void QIviWindowControlBackendInterface::close(const QString &zone) = 0

    Closes the window identified by \a zone. Whether the window can be closed is controlled by the attribute system.

    \sa stateChanged()
*/

/*!
    \fn virtual void QIviWindowControlBackendInterface::heaterModeChanged(QIviWindowControl::HeaterMode value, const QString &zone = QString())

    The signal is emitted when the heater mode is changed for \a zone to \a value. The available modes can be defined using the attribute system.

    \sa setHeaterMode()
*/

/*!
    \fn void QIviWindowControlBackendInterface::heaterModeAttributeChanged(const QIviPropertyAttribute<QIviWindowControl::HeaterMode> &attribute, const QString &zone = QString())

    The signal is emitted when the heater mode attribute for \a zone is changed to \a attribute.
*/

/*!
    \fn virtual void QIviWindowControlBackendInterface::heaterEnabledChanged(bool enabled, const QString &zone = QString())

    The signal is emitted when the \a zone heater state is changed to \a enabled.
*/

/*!
    \fn void QIviWindowControlBackendInterface::heaterAttributeChanged(const QIviPropertyAttribute<bool> &attribute, const QString &zone = QString())

    The signal is emitted when the heater attribute for \a zone is changed to \a attribute.
*/

/*!
    \fn virtual void QIviWindowControlBackendInterface::blindModeChanged(QIviWindowControl::BlindMode value, const QString &zone = QString())

    The signal is emitted when the blind mode is changed for \a zone to \a value. The available modes can be defined using the attribute system.

    \sa setBlindMode()
*/

/*!
    \fn void QIviWindowControlBackendInterface::blindModeAttributeChanged(const QIviPropertyAttribute<QIviWindowControl::BlindMode> &attribute, const QString &zone = QString())

    The signal is emitted when the blind mode attribute for \a zone is changed to \a attribute.
*/

/*!
    \fn virtual void QIviWindowControlBackendInterface::blindStateChanged(QIviWindowControl::State value, const QString &zone = QString())

    The signal is emitted when the blind state is changed for \a zone to \a value. The available states can be defined using the attribute system.
*/

/*!
    \fn void QIviWindowControlBackendInterface::blindStateAttributeChanged(const QIviPropertyAttribute<QIviWindowControl::State> &attribute, const QString &zone = QString())

    The signal is emitted when the blind state attribute for \a zone is changed to \a attribute.
*/

/*!
    \fn virtual void QIviWindowControlBackendInterface::stateChanged(QIviWindowControl::State value, const QString &zone = QString())

    The signal is emitted when the state is changed for \a zone to \a value. The available states can be defined using the attribute system.
*/

/*!
    \fn void QIviWindowControlBackendInterface::stateAttributeChanged(const QIviPropertyAttribute<QIviWindowControl::State> &attribute, const QString &zone = QString())

    The signal is emitted when the state attribute for \a zone is changed to \a attribute.
*/

QT_END_NAMESPACE
