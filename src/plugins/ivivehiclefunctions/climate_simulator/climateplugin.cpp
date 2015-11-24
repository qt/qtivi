/****************************************************************************
**
** Copyright (C) 2015 Pelagicore AG
** Contact: http://www.qt.io/ or http://www.pelagicore.com/
**
** This file is part of the QtIVI module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL3-PELAGICORE$
** Commercial License Usage
** Licensees holding valid commercial Qt IVI licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Pelagicore. For licensing terms
** and conditions, contact us at http://www.pelagicore.com.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPLv3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl-3.0.html.
**
** $QT_END_LICENSE$
**
** SPDX-License-Identifier: LGPL-3.0
**
****************************************************************************/

#include "climateplugin.h"
#include "climatecontrolbackend.h"

#include <QtIVIVehicleFunctions/QtIVIClimateControlBackendInterface>
#include <QStringList>

ClimatePlugin::ClimatePlugin(QObject *parent) :
    QObject(parent),
    m_climate(new ClimateControlBackend(this))
{
}

QStringList ClimatePlugin::interfaces() const
{
    QStringList list;
    list << QtIVIClimateControlBackendInterface::interfaceName;
    return list;
}

QObject *ClimatePlugin::interfaceInstance(const QString &interface) const
{
    if (interface != QtIVIClimateControlBackendInterface::interfaceName)
        return 0;

    return m_climate;
}
