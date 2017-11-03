{#
# Copyright (C) 2017 Klaralvdalens Datakonsult AB (KDAB).
# Copyright (C) 2017 Pelagicore AG
# Contact: https://www.qt.io/licensing/
#
# This file is part of the QtIvi module of the Qt Toolkit.
#
# $QT_BEGIN_LICENSE:LGPL-QTAS$
# Commercial License Usage
# Licensees holding valid commercial Qt Automotive Suite licenses may use
# this file in accordance with the commercial license agreement provided
# with the Software or, alternatively, in accordance with the terms
# contained in a written agreement between you and The Qt Company.  For
# licensing terms and conditions see https://www.qt.io/terms-conditions.
# For further information use the contact form at https://www.qt.io/contact-us.
#
# GNU Lesser General Public License Usage
# Alternatively, this file may be used under the terms of the GNU Lesser
# General Public License version 3 as published by the Free Software
# Foundation and appearing in the file LICENSE.LGPL3 included in the
# packaging of this file. Please review the following information to
# ensure the GNU Lesser General Public License version 3 requirements
# will be met: https://www.gnu.org/licenses/lgpl-3.0.html.
#
# GNU General Public License Usage
# Alternatively, this file may be used under the terms of the GNU
# General Public License version 2.0 or (at your option) the GNU General
# Public license version 3 or any later version approved by the KDE Free
# Qt Foundation. The licenses are as published by the Free Software
# Foundation and appearing in the file LICENSE.GPL2 and LICENSE.GPL3
# included in the packaging of this file. Please review the following
# information to ensure the GNU General Public License requirements will
# be met: https://www.gnu.org/licenses/gpl-2.0.html and
# https://www.gnu.org/licenses/gpl-3.0.html.
#
# $QT_END_LICENSE$
#
# SPDX-License-Identifier: LGPL-3.0
#}
{% include "generated_comment.cpp.tpl" %}

#include "validationstatus.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>
#include <QObject>
#include <QMetaProperty>

QString idlToQtTypeName(const QString& idlTypeName)
{
    QString result = idlTypeName;
    if (idlTypeName == QLatin1String("string"))
        result = "QString";
    return result;
}

bool checkProperty(const QMetaProperty& metaProp, const QJsonValue& jValProp, QString& errorMessage)
{
    bool result = true;
    QJsonObject jObjProp = jValProp.toObject();
    if (metaProp.name() != jObjProp[QLatin1String("name")].toString())
        return false;
    // Anyway it should be readable
    if (!metaProp.isReadable()) {
        errorMessage += QString("Property %1 is not readable\n").arg(metaProp.name());
        return false;
    }
    // Writable only if not readonly
    if (metaProp.isWritable() && jObjProp[QLatin1String("readonly")].toBool()) {
        errorMessage += QString("Property %1 is writable whereas it's defined as read-only\n").arg(metaProp.name());
        return false;
    }
    // Check the property type as well
    QString metaTypeName = metaProp.typeName();
    QString propTypeName = idlToQtTypeName(jObjProp[QLatin1String("type")].toObject()[QLatin1String("name")].toString());
    if (metaTypeName != propTypeName)
        return false;
    return result;
}

bool checkMethod(const QMetaMethod& metaOp, const QJsonValue& jValOp, QString& errorMessage)
{
    QJsonObject jObjOp = jValOp.toObject();
    if (metaOp.name() != jObjOp[QLatin1String("name")].toString())
        return false;

    // Check the method parameter types and return type as well
    QString metaReturnType = metaOp.typeName();
    QString opReturnType = jObjOp[QLatin1String("type")].toObject()[QLatin1String("name")].toString();
    opReturnType = idlToQtTypeName(opReturnType);
    if (metaReturnType != opReturnType) {
        return false;
    }
    QJsonValue jValParams = jObjOp[QLatin1String("parameters")];
    Q_ASSERT(jValParams.isNull() || jValParams.isArray());
    QJsonArray jArrParams = jValParams.toArray();
    if (jArrParams.size() != metaOp.parameterCount()) {
        errorMessage += QString("Amount of parameters in method %1 differs from its IDL definition (%2 vs %3)\n")
                .arg(metaOp.name().data())
                .arg(metaOp.parameterCount())
                .arg(jArrParams.size());
        return false;
    }

    for (int i = 0; i < metaOp.parameterCount(); ++i) {
        QString metaParamType = jArrParams[i].toObject()[QLatin1String("type")].toObject()[QLatin1String("name")].toString();
        metaParamType = idlToQtTypeName(metaParamType);
        QString genParamType = metaOp.parameterTypes().at(i);
        if (genParamType != metaParamType) {
            errorMessage += QString("Method %1: %2-th parameter type differs from IDL definition (%3 vs %4)\n")
                    .arg(metaOp.name().data())
                    .arg(i)
                    .arg(genParamType)
                    .arg(metaParamType);
            return false;
        }
    }
    return true;
}

bool checkGeneration(QString& errorMessage, QObject& obj)
{
    bool result = true;
    auto* metaObj = obj.metaObject();
    int classInfoCount = metaObj->classInfoCount();
    QMetaClassInfo metaClassInfo;
    const QString META_INFO_NAME = QLatin1String("IviJson");
    for (int index = 0; index < classInfoCount; ++index) {
        metaClassInfo = metaObj->classInfo(index);
        if (metaClassInfo.name() == META_INFO_NAME)
            break;
    }
    if (metaClassInfo.name() != META_INFO_NAME) {
        return false;
    }
    QJsonObject metaDoc(QJsonDocument::fromJson(metaClassInfo.value()).object());
    auto jVal = metaDoc[QLatin1String("interfaces")];
    if (jVal.isNull() || !jVal.isArray()) {
        errorMessage += QString("Interfaces list is empty in the class meta info");
        return false;
    }


    QJsonArray jArrInterfaces = jVal.toArray();
    QJsonObject jInterface;
    for (auto jValInterface : jArrInterfaces) {
        Q_ASSERT(jValInterface.isObject());
        if (jValInterface.toObject()[QLatin1String("name")] == QLatin1String(metaObj->className())) {
            jInterface = jValInterface.toObject();
            break;
        }
    }
    if (jInterface.empty()) {
        errorMessage += QString("Interface %1 is not implemented!\n").arg(metaObj->className());
        return false;
    }

    // Check the properties
    QJsonValue jValProps = jInterface[QLatin1String("properties")];
    auto jArrProps = jValProps.toArray();
    for (auto jValProp : jArrProps) {
        auto propName = jValProp.toObject()[QLatin1String("name")].toString();
        int propIdx = metaObj->indexOfProperty(propName.toLatin1().data());
        if (propIdx == -1) {
            errorMessage += QString("Property %1 has been not generated by the generator!\n").arg(propName);
            result = false;
        } else {
            result &= checkProperty(metaObj->property(propIdx), jValProp, errorMessage);
        }
    }

    // Check the methods
    QJsonValue jValOps = jInterface[QLatin1String("operations")];
    auto jArrOps = jValOps.toArray();
    for (auto jValOp : jArrOps) {
        bool opFound = false;
        QString opName = jValOp.toObject()[QLatin1String("name")].toString();
        for (int i = 0; i < metaObj->methodCount(); ++i) {
            if (metaObj->method(i).name() == opName) {
                opFound = true;
                result &= checkMethod(metaObj->method(i), jValOp, errorMessage);
            }
        }
        if (!opFound) {
            errorMessage += QString("Method %1 has been not generated by the generator!").arg(opName);
            result = false;
        }
    }

    return result;
}
