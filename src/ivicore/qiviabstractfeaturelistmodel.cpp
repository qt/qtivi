/****************************************************************************
**
** Copyright (C) 2016 Pelagicore AG
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

#include "qiviabstractfeaturelistmodel.h"
#include "qiviabstractfeaturelistmodel_p.h"

QIviHelperFeature::QIviHelperFeature(const QString &interface, QIviAbstractFeatureListModel *model)
    : QIviAbstractFeature(interface)
    , m_model(model)
{
}

bool QIviHelperFeature::acceptServiceObject(QIviServiceObject *so)
{
    return m_model->acceptServiceObject(so);
}

void QIviHelperFeature::connectToServiceObject(QIviServiceObject *so)
{
    m_model->connectToServiceObject(so);
}

void QIviHelperFeature::disconnectFromServiceObject(QIviServiceObject *so)
{
    m_model->disconnectFromServiceObject(so);
}

void QIviHelperFeature::clearServiceObject()
{
    m_model->clearServiceObject();
}

QIviAbstractFeatureListModelPrivate::QIviAbstractFeatureListModelPrivate(const QString &interface, QIviAbstractFeatureListModel *model)
    : QAbstractItemModelPrivate()
    , m_feature(new QIviHelperFeature(interface, model))
{

}

QIviAbstractFeatureListModelPrivate::~QIviAbstractFeatureListModelPrivate()
{
    delete m_feature;
}

QIviAbstractFeatureListModel::QIviAbstractFeatureListModel(const QString &interface, QObject *parent)
    : QAbstractListModel(*new QIviAbstractFeatureListModelPrivate(interface, this), parent)
{
    Q_D(QIviAbstractFeatureListModel);
    connect(d->m_feature, &QIviAbstractFeature::serviceObjectChanged, this, &QIviAbstractFeatureListModel::serviceObjectChanged);
    connect(d->m_feature, &QIviAbstractFeature::discoveryModeChanged, this, &QIviAbstractFeatureListModel::discoveryModeChanged);
    connect(d->m_feature, &QIviAbstractFeature::discoveryResultChanged, this, &QIviAbstractFeatureListModel::discoveryResultChanged);
    connect(d->m_feature, &QIviAbstractFeature::isValidChanged, this, &QIviAbstractFeatureListModel::isValidChanged);
    connect(d->m_feature, &QIviAbstractFeature::errorChanged, this, &QIviAbstractFeatureListModel::errorChanged);
}

QIviAbstractFeatureListModel::~QIviAbstractFeatureListModel()
{

}

QIviServiceObject *QIviAbstractFeatureListModel::serviceObject() const
{
    Q_D(const QIviAbstractFeatureListModel);
    return d->m_feature->serviceObject();
}

QIviAbstractFeature::DiscoveryMode QIviAbstractFeatureListModel::discoveryMode() const
{
    Q_D(const QIviAbstractFeatureListModel);
    return d->m_feature->discoveryMode();
}

QIviAbstractFeature::DiscoveryResult QIviAbstractFeatureListModel::discoveryResult() const
{
    Q_D(const QIviAbstractFeatureListModel);
    return d->m_feature->discoveryResult();
}

bool QIviAbstractFeatureListModel::isValid() const
{
    Q_D(const QIviAbstractFeatureListModel);
    return d->m_feature->isValid();
}

QIviAbstractFeature::Error QIviAbstractFeatureListModel::error() const
{
    Q_D(const QIviAbstractFeatureListModel);
    return d->m_feature->error();
}

QString QIviAbstractFeatureListModel::errorMessage() const
{
    Q_D(const QIviAbstractFeatureListModel);
    return d->m_feature->errorMessage();
}

bool QIviAbstractFeatureListModel::setServiceObject(QIviServiceObject *so)
{
    Q_D(QIviAbstractFeatureListModel);
    return d->m_feature->setServiceObject(so);
}

void QIviAbstractFeatureListModel::setDiscoveryMode(QIviAbstractFeature::DiscoveryMode discoveryMode)
{
    Q_D(QIviAbstractFeatureListModel);
    return d->m_feature->setDiscoveryMode(discoveryMode);
}

QIviAbstractFeature::DiscoveryResult QIviAbstractFeatureListModel::startAutoDiscovery()
{
    Q_D(QIviAbstractFeatureListModel);
    return d->m_feature->startAutoDiscovery();
}

QIviAbstractFeatureListModel::QIviAbstractFeatureListModel(QIviAbstractFeatureListModelPrivate &dd, QObject *parent)
    : QAbstractListModel(dd, parent)
{
    Q_D(QIviAbstractFeatureListModel);
    connect(d->m_feature, &QIviAbstractFeature::serviceObjectChanged, this, &QIviAbstractFeatureListModel::serviceObjectChanged);
    connect(d->m_feature, &QIviAbstractFeature::discoveryModeChanged, this, &QIviAbstractFeatureListModel::discoveryModeChanged);
    connect(d->m_feature, &QIviAbstractFeature::discoveryResultChanged, this, &QIviAbstractFeatureListModel::discoveryResultChanged);
    connect(d->m_feature, &QIviAbstractFeature::isValidChanged, this, &QIviAbstractFeatureListModel::isValidChanged);
    connect(d->m_feature, &QIviAbstractFeature::errorChanged, this, &QIviAbstractFeatureListModel::errorChanged);
}

QString QIviAbstractFeatureListModel::interfaceName() const
{
    Q_D(const QIviAbstractFeatureListModel);
    return d->m_feature->interfaceName();
}

QString QIviAbstractFeatureListModel::errorText() const
{
    Q_D(const QIviAbstractFeatureListModel);
    return d->m_feature->errorText();
}

void QIviAbstractFeatureListModel::setError(QIviAbstractFeature::Error error, const QString &message)
{
    Q_D(QIviAbstractFeatureListModel);
    d->m_feature->setError(error, message);
}
