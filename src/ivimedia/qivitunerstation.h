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

#ifndef QIVITUNERSTATION_H
#define QIVITUNERSTATION_H

#include <QtIviMedia/qtivimediaglobal.h>
#include <QtIviMedia/QIviAmFmTuner>
#include <QtIviCore/QIviSearchAndBrowseListItem>

QT_BEGIN_NAMESPACE

class Q_QTIVIMEDIA_EXPORT QIviTunerStation : public QIviSearchAndBrowseListItem
{
    Q_GADGET

    Q_PROPERTY(QString stationName READ stationName)
    Q_PROPERTY(int frequency READ frequency)

public:
    QIviTunerStation();
    virtual ~QIviTunerStation();

    virtual QString stationName() const { return QString(); }
    virtual int frequency() const { return -1; }
};

class Q_QTIVIMEDIA_EXPORT QIviAmFmTunerStation : public QIviTunerStation
{
    Q_GADGET

    Q_PROPERTY(QIviAmFmTuner::Band band READ band)

public:
    QIviAmFmTunerStation();
    virtual ~QIviAmFmTunerStation();

    virtual QIviAmFmTuner::Band band() const { return QIviAmFmTuner::FMBand; }
};

QT_END_NAMESPACE

Q_DECLARE_METATYPE(QIviTunerStation)
Q_DECLARE_METATYPE(QIviAmFmTunerStation)

#endif // QIVITUNERSTATION_H
