/****************************************************************************
**
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

EchoService::EchoService()
    :m_testCombo(Contact(QStringLiteral("Antti"), 34, true), EchoModule::Friday),
     m_testId(QStringLiteral("id123"))
{

}

void EchoService::setLastMessage(QString lastMessage)
{
    EchoSimpleSource::setLastMessage(lastMessage);
}

QString EchoService::echo(const QString &msg)
{
    emit echoSlotCalled(msg);
    return msg;
}

QString EchoService::id()
{
    emit idSlotCalled();
    return m_testId;
}

Combo EchoService::getCombo()
{
    emit getComboSlotCalled();
    return m_testCombo;
}

void EchoService::voidSlot()
{
    emit voidSlotCalled();
}

void EchoService::voidSlot2(int param)
{
    emit voidSlot2Called(param);
}
