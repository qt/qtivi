/****************************************************************************
**
** Copyright (C) 2018 Pelagicore AG
** Copyright (C) 2017 Klaralvdalens Datakonsult AB (KDAB).
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

#ifndef QTIVIVEHICLEFUNCTIONS_QIVICONCRETEWINDOWCONTROLBACKEND_H_
#define QTIVIVEHICLEFUNCTIONS_QIVICONCRETEWINDOWCONTROLBACKEND_H_

#include "qiviwindowcontrolbackend.h"
#include <QTimer>

QT_BEGIN_NAMESPACE

class QIviConcreteWindowControlBackend;
class WindowTimer : public QObject
{
public:
    WindowTimer(const QString &zone, bool isBlind, QIviConcreteWindowControlBackend *backend);

    void setOpeningTime(int intervalInSeconds);
    void open();
    void close();

public slots:
    void checkValue();

private:
    QTimer *m_timer;
    bool m_opening;
    int m_currentValue;
    int m_interval;
    QString m_zone;
    bool m_blind;
    QIviConcreteWindowControlBackend *m_backend;
};

class QIviConcreteWindowControlBackend : public QIviWindowControlBackend
{
public:
    explicit QIviConcreteWindowControlBackend(QObject *parent = nullptr);
    ~QIviConcreteWindowControlBackend();

    virtual void setBlindMode(QtIviVehicleFunctionsModule::BlindMode blindMode, const QString &zone) override;
    virtual void open(const QString &zone) override;
    virtual void close(const QString &zone) override;

    QtIviVehicleFunctionsModule::WindowState windowState(QString zone);
    void setWindowState(QtIviVehicleFunctionsModule::WindowState state, const QString &zone);
    QtIviVehicleFunctionsModule::WindowState blindState(QString zone);
    void setBlindState(QtIviVehicleFunctionsModule::WindowState state, const QString &zone);

private:
    QMap<QString,WindowTimer*> m_zoneWindowTimers;
    QMap<QString,WindowTimer*> m_zoneBlindTimers;
};

QT_END_NAMESPACE

#endif // QTIVIVEHICLEFUNCTIONS_QIVICONCRETEWINDOWCONTROLBACKEND_H_
