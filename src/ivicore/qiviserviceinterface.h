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

#ifndef QIVISERVICEINTERFACE_H
#define QIVISERVICEINTERFACE_H

#include <QtIviCore/qivifeatureinterface.h>
#include <QtIviCore/qtiviglobal.h>

#include <QtCore/QtPlugin>

QT_BEGIN_NAMESPACE

template <class T> T qivi_interface_cast(QObject *backend)
{
    T inst = qobject_cast<T>(backend);
    static bool showOnce = true;
    if (!inst && showOnce) {
        typedef typename std::remove_cv<typename std::remove_pointer<T>::type>::type ObjType;
        qCritical("Casting the backend to the interface %s failed.\n"
                  "Either the backend pointer is not of the correct type or casting failed because "
                  "debug and release libraries were mixed.", ObjType::staticMetaObject.className());
        showOnce = false;
    }
    return inst;
}

class Q_QTIVICORE_EXPORT QIviServiceInterface
{
public:
    virtual ~QIviServiceInterface();

    virtual QStringList interfaces() const = 0;
    virtual QIviFeatureInterface *interfaceInstance(const QString &interfaceName) const = 0;

    template <class T> T interfaceInstance(const QString &interfaceName) const {
        T inst = qivi_interface_cast<T>(interfaceInstance(interfaceName));
        return inst;
    }
};

#define QIviServiceInterface_iid "org.qt-project.qtivi.QIviServiceInterface/1.0"
Q_DECLARE_INTERFACE(QIviServiceInterface, QIviServiceInterface_iid)

QT_END_NAMESPACE

Q_DECLARE_METATYPE(QIviServiceInterface*)

#endif // QIVISERVICEINTERFACE_H

