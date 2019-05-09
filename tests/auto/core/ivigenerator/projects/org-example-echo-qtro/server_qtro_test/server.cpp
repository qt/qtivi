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

#include "server.h"
#include "core.h"
#include <QTest>

bool Server::start()
{
    bool val =  true;
    val = val && Core::instance()->host()->enableRemoting(&m_echoService, QStringLiteral("org.example.echo.Echo"));
    val = val && Core::instance()->host()->enableRemoting(&m_contactsModelService, QStringLiteral("org.example.echo.Echo.contactList"));
    val = val && Core::instance()->host()->enableRemoting(&m_echoZonedService, QStringLiteral("org.example.echo.EchoZoned"));
    //Give QtRO time to announce the service
    QTest::qWait(200);
    return val;
}

bool Server::stop()
{
    bool val =  true;
    val = val && Core::instance()->host()->disableRemoting(&m_echoService);
    val = val && Core::instance()->host()->disableRemoting(&m_contactsModelService);
    val = val && Core::instance()->host()->disableRemoting(&m_echoZonedService);
    //Give QtRO time to send the disconnect message to the Replica
    QTest::qWait(200);
    return  val;
}

Server::~Server()
{
    stop();
}
