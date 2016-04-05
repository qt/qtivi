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

#ifndef QTIVIPROPERTY_H
#define QTIVIPROPERTY_H

#include <QtCore/QObject>
#include <QtCore/QVariant>
#include <QtIVICore/qtiviglobal.h>
#include <QtIVICore/QtIVIPropertyAttribute>

QT_BEGIN_NAMESPACE

class QtIVIPropertyPrivate;
class Q_QTIVICORE_EXPORT QtIVIProperty : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QVariant value READ value WRITE setValue NOTIFY valueChanged)
    Q_PROPERTY(bool available READ isAvailable NOTIFY availableChanged)
    Q_PROPERTY(QVariant minimumValue READ minimumValue NOTIFY minimumValueChanged)
    Q_PROPERTY(QVariant maximumValue READ maximumValue NOTIFY maximumValueChanged)
    Q_PROPERTY(QVariantList availableValues READ availableValues NOTIFY availableValuesChanged)

public:
    virtual ~QtIVIProperty();

    virtual bool isAvailable() const = 0;
    virtual QVariant minimumValue() const  = 0;
    virtual QVariant maximumValue() const = 0;
    virtual QVariantList availableValues() const = 0;
    virtual QVariant value() const = 0;

public Q_SLOTS:
    void setValue(const QVariant &value);

Q_SIGNALS:
    void valueChanged(const QVariant &value);
    void availableChanged(bool available);
    void minimumValueChanged(const QVariant &minimumValue);
    void maximumValueChanged(const QVariant &maximumValue);
    void availableValuesChanged(const QVariantList &availableValues);

protected:
    QtIVIProperty(int userType, const QObject *receiver, QtPrivate::QSlotObjectBase *attributeGetter, QtPrivate::QSlotObjectBase *valueGetter);
    void setValueSetter(QtPrivate::QSlotObjectBase *valueSetter);
    QtPrivate::QSlotObjectBase *attributeGetter() const;
    QtPrivate::QSlotObjectBase *valueGetter() const;

private:
    QtIVIPropertyPrivate * const d_ptr;
    Q_DECLARE_PRIVATE(QtIVIProperty)
};

QT_END_NAMESPACE

#endif // QTIVIPROPERTY_H
