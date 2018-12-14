/****************************************************************************
**
** Copyright (C) 2019 Luxoft Sweden AB
** Copyright (C) 2018 Pelagicore AG
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

#ifndef QTIVICOREMODULE_H
#define QTIVICOREMODULE_H

#include <QtIviCore/qtiviglobal.h>
#include <QObject>

QT_BEGIN_NAMESPACE

class Q_QTIVICORE_EXPORT QtIviCoreModule : public QObject
{
    Q_OBJECT
public:
    explicit QtIviCoreModule(QObject *parent = nullptr);

    enum ModelCapability {
        NoExtras = 0x0,
        SupportsGetSize = 0x1, // (the backend knows the size of the model when the query is done and the user can select a different way for loading the model content)
        SupportsFiltering = 0x2,
        SupportsSorting = 0x4,
        SupportsAndConjunction = 0x8,
        SupportsOrConjunction = 0x10,
        SupportsStatelessNavigation = 0x20, // (the backend supports to have multiple models showing different contentTypes and filters at the same time)
        SupportsInsert = 0x40,
        SupportsMove = 0x80,
        SupportsRemove = 0x100
    };
    Q_DECLARE_FLAGS(ModelCapabilities, ModelCapability)
    Q_FLAG(ModelCapabilities)

    static void registerTypes();
    static void registerQmlTypes(const QString& uri, int majorVersion = 1, int minorVersion = 0);
};

QT_END_NAMESPACE

#endif // QTIVICOREMODULE_H
