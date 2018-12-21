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

#ifndef QIVIMEDIADEVICE_H
#define QIVIMEDIADEVICE_H

#include <QtIviCore/QIviServiceObject>
#include <QtIviMedia/qtivimediaglobal.h>

QT_BEGIN_NAMESPACE

class Q_QTIVIMEDIA_EXPORT QIviMediaDevice : public QIviServiceObject
{
    Q_OBJECT

    Q_PROPERTY(QString type READ type CONSTANT)
    Q_PROPERTY(QString name READ name NOTIFY nameChanged)

public:
    explicit QIviMediaDevice(QObject *parent = nullptr);
    virtual QString type() const = 0;
    virtual QString name() const = 0;

Q_SIGNALS:
    void nameChanged(QString name);
};

class Q_QTIVIMEDIA_EXPORT QIviMediaUsbDevice : public QIviMediaDevice
{
    Q_OBJECT

public:
    explicit QIviMediaUsbDevice(QObject *parent = nullptr);

    virtual QString type() const override;

    //TODO add a signal to indicate that the stick can be removed now.
public Q_SLOTS:
    virtual void eject() = 0;
};

QT_END_NAMESPACE

#endif // QIVIMEDIADEVICE_H
