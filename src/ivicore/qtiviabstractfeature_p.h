/****************************************************************************
**
** Copyright (C) 2016 Pelagicore AG
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

#ifndef QTIVIABSTRACTFEATURE_P_H
#define QTIVIABSTRACTFEATURE_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists purely as an
// implementation detail. This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <QtCore/QObject>

#include "qtiviabstractfeature.h"

QT_BEGIN_NAMESPACE

class QtIVIAbstractFeaturePrivate : public QObject
{
public:
    QtIVIAbstractFeaturePrivate(const QString &interface, QtIVIAbstractFeature *parent);

    void setDiscoveryResult(QtIVIAbstractFeature::DiscoveryResult discoveryResult);

    QtIVIAbstractFeature * const q_ptr;
    Q_DECLARE_PUBLIC(QtIVIAbstractFeature)

    QString m_interface;
    QtIVIServiceObject* m_serviceObject;
    QtIVIAbstractFeature::DiscoveryMode m_discoveryMode;
    QtIVIAbstractFeature::DiscoveryResult m_discoveryResult;
    QString m_errorMessage;
    QtIVIAbstractFeature::Error m_error;
    bool m_qmlCreation;
};

QT_END_NAMESPACE

#endif // QTIVIABSTRACTFEATURE_P_H
