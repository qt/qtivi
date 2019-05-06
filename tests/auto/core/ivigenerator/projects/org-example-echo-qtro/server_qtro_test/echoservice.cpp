/****************************************************************************
**
** Copyright (C) 2019 Luxoft Sweden AB
** Copyright (C) 2018 Pelagicore AG
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtIvi module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:GPL-EXCEPT-QTAS$
** Commercial License Usage
** Licensees holding valid commercial Qt Automotive Suite licenses may use
** this file in accordance with the commercial license agreement provided
** with the Software or, alternatively, in accordance with the terms
** contained in a written agreement between you and The Qt Company.  For
** licensing terms and conditions see https://www.qt.io/terms-conditions.
** For further information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 as published by the Free Software
** Foundation with exceptions as appearing in the file LICENSE.GPL3-EXCEPT
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "echoservice.h"
#include <QtIviRemoteObjectsHelper>
#include <QTimer>

EchoService::EchoService()
    : m_testCombo(Contact(QStringLiteral("Antti"), 34, true, QVariant()), EchoModule::Friday)
    , m_testId(QStringLiteral("id123"))
{

}

void EchoService::setLastMessage(QString lastMessage)
{
    EchoSimpleSource::setLastMessage(lastMessage);
}

QVariant EchoService::echo(const QString &msg)
{
    emit echoSlotCalled(msg);
    return msg;
}

QVariant EchoService::id()
{
    emit idSlotCalled();
    return m_testId;
}

QVariant EchoService::getCombo()
{
    emit getComboSlotCalled();
    return QVariant::fromValue(m_testCombo);
}

QVariant EchoService::voidSlot()
{
    emit voidSlotCalled();
    return QVariant();
}

QVariant EchoService::voidSlot2(int param)
{
    emit voidSlot2Called(param);
    return QVariant();
}

QVariant EchoService::timer(int interval)
{
    static quint64 counter = 0;
    QIviRemoteObjectPendingResult pendingResult(counter++, false);
    QTimer::singleShot(interval, this, [this, pendingResult](){
        emit pendingResultAvailable(pendingResult.id(), true, QVariant());
    });
    return QVariant::fromValue(pendingResult);
}
