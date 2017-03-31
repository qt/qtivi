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

#ifndef WINDOWCONTROLBACKEND_H
#define WINDOWCONTROLBACKEND_H

#include <QObject>
#include <QTimer>
#include <QtIviVehicleFunctions/QIviWindowControlBackendInterface>

class WindowControlBackend;
class WindowTimer : public QObject
{
    Q_OBJECT

public:
    WindowTimer(const QString &zone, bool isBlind, WindowControlBackend *backend);

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
    WindowControlBackend *m_backend;
};

class WindowControlBackend : public QIviWindowControlBackendInterface
{
    Q_OBJECT

public:
    WindowControlBackend(QObject *parent = nullptr);
    ~WindowControlBackend();

    QStringList availableZones() const;
    void initialize();

    void setHeaterMode(QIviWindowControl::HeaterMode value, const QString &zone);
    void setBlindMode(QIviWindowControl::BlindMode value, const QString &zone);
    void open(const QString &zone);
    void close(const QString &zone);

private:

    struct ZoneBackend {
        QIviWindowControl::HeaterMode heaterMode;
        QIviPropertyAttribute<QIviWindowControl::HeaterMode> heaterModeAttribute;
        bool heaterEnabled;
        QIviPropertyAttribute<bool> heaterAttribute;
        QIviWindowControl::BlindMode blindMode;
        QIviPropertyAttribute<QIviWindowControl::BlindMode> blindModeAttribute;
        QIviWindowControl::State blindState;
        QIviPropertyAttribute<QIviWindowControl::State> blindStateAttribute;
        WindowTimer *blindTimer;
        QIviWindowControl::State state;
        QIviPropertyAttribute<QIviWindowControl::State> stateAttribute;
        WindowTimer *stateTimer;
    };

    QMap<QString,ZoneBackend> m_zoneMap;
    friend class WindowTimer;
};

#endif // WINDOWCONTROLBACKEND_H
