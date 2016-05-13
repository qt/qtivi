/****************************************************************************
**
** Copyright (C) 2016 Pelagicore AG
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtIVI module of the Qt Toolkit.
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

#include "qtiviabstractfeaturelistmodel.h"
#include "qtiviabstractfeaturelistmodel_p.h"

QtIVIHelperFeature::QtIVIHelperFeature(const QString &interface, QtIVIAbstractFeatureListModel *model)
    : QtIVIAbstractFeature(interface)
    , m_model(model)
{
}

bool QtIVIHelperFeature::acceptServiceObject(QtIVIServiceObject *so)
{
    return m_model->acceptServiceObject(so);
}

void QtIVIHelperFeature::connectToServiceObject(QtIVIServiceObject *so)
{
    m_model->connectToServiceObject(so);
}

void QtIVIHelperFeature::disconnectFromServiceObject(QtIVIServiceObject *so)
{
    m_model->disconnectFromServiceObject(so);
}

void QtIVIHelperFeature::clearServiceObject()
{
    m_model->clearServiceObject();
}

QtIVIAbstractFeatureListModelPrivate::QtIVIAbstractFeatureListModelPrivate(const QString &interface, QtIVIAbstractFeatureListModel *model)
    : QAbstractItemModelPrivate()
    , m_feature(new QtIVIHelperFeature(interface, model))
{

}

QtIVIAbstractFeatureListModelPrivate::~QtIVIAbstractFeatureListModelPrivate()
{
    delete m_feature;
}

QtIVIAbstractFeatureListModel::QtIVIAbstractFeatureListModel(const QString &interface, QObject *parent)
    : QAbstractListModel(*new QtIVIAbstractFeatureListModelPrivate(interface, this), parent)
{
    Q_D(QtIVIAbstractFeatureListModel);
    connect(d->m_feature, &QtIVIAbstractFeature::serviceObjectChanged, this, &QtIVIAbstractFeatureListModel::serviceObjectChanged);
    connect(d->m_feature, &QtIVIAbstractFeature::discoveryModeChanged, this, &QtIVIAbstractFeatureListModel::discoveryModeChanged);
    connect(d->m_feature, &QtIVIAbstractFeature::discoveryResultChanged, this, &QtIVIAbstractFeatureListModel::discoveryResultChanged);
    connect(d->m_feature, &QtIVIAbstractFeature::isValidChanged, this, &QtIVIAbstractFeatureListModel::isValidChanged);
    connect(d->m_feature, &QtIVIAbstractFeature::errorChanged, this, &QtIVIAbstractFeatureListModel::errorChanged);
}

QtIVIAbstractFeatureListModel::~QtIVIAbstractFeatureListModel()
{

}

QtIVIServiceObject *QtIVIAbstractFeatureListModel::serviceObject() const
{
    Q_D(const QtIVIAbstractFeatureListModel);
    return d->m_feature->serviceObject();
}

QtIVIAbstractFeature::DiscoveryMode QtIVIAbstractFeatureListModel::discoveryMode() const
{
    Q_D(const QtIVIAbstractFeatureListModel);
    return d->m_feature->discoveryMode();
}

QtIVIAbstractFeature::DiscoveryResult QtIVIAbstractFeatureListModel::discoveryResult() const
{
    Q_D(const QtIVIAbstractFeatureListModel);
    return d->m_feature->discoveryResult();
}

bool QtIVIAbstractFeatureListModel::isValid() const
{
    Q_D(const QtIVIAbstractFeatureListModel);
    return d->m_feature->isValid();
}

QtIVIAbstractFeature::Error QtIVIAbstractFeatureListModel::error() const
{
    Q_D(const QtIVIAbstractFeatureListModel);
    return d->m_feature->error();
}

QString QtIVIAbstractFeatureListModel::errorMessage() const
{
    Q_D(const QtIVIAbstractFeatureListModel);
    return d->m_feature->errorMessage();
}

bool QtIVIAbstractFeatureListModel::setServiceObject(QtIVIServiceObject *so)
{
    Q_D(QtIVIAbstractFeatureListModel);
    return d->m_feature->setServiceObject(so);
}

void QtIVIAbstractFeatureListModel::setDiscoveryMode(QtIVIAbstractFeature::DiscoveryMode discoveryMode)
{
    Q_D(QtIVIAbstractFeatureListModel);
    return d->m_feature->setDiscoveryMode(discoveryMode);
}

QtIVIAbstractFeature::DiscoveryResult QtIVIAbstractFeatureListModel::startAutoDiscovery()
{
    Q_D(QtIVIAbstractFeatureListModel);
    return d->m_feature->startAutoDiscovery();
}

QtIVIAbstractFeatureListModel::QtIVIAbstractFeatureListModel(QtIVIAbstractFeatureListModelPrivate &dd, QObject *parent)
    : QAbstractListModel(dd, parent)
{
    Q_D(QtIVIAbstractFeatureListModel);
    connect(d->m_feature, &QtIVIAbstractFeature::serviceObjectChanged, this, &QtIVIAbstractFeatureListModel::serviceObjectChanged);
    connect(d->m_feature, &QtIVIAbstractFeature::discoveryModeChanged, this, &QtIVIAbstractFeatureListModel::discoveryModeChanged);
    connect(d->m_feature, &QtIVIAbstractFeature::discoveryResultChanged, this, &QtIVIAbstractFeatureListModel::discoveryResultChanged);
    connect(d->m_feature, &QtIVIAbstractFeature::isValidChanged, this, &QtIVIAbstractFeatureListModel::isValidChanged);
    connect(d->m_feature, &QtIVIAbstractFeature::errorChanged, this, &QtIVIAbstractFeatureListModel::errorChanged);
}

QString QtIVIAbstractFeatureListModel::interfaceName() const
{
    Q_D(const QtIVIAbstractFeatureListModel);
    return d->m_feature->interfaceName();
}

QString QtIVIAbstractFeatureListModel::errorText() const
{
    Q_D(const QtIVIAbstractFeatureListModel);
    return d->m_feature->errorText();
}

void QtIVIAbstractFeatureListModel::setError(QtIVIAbstractFeature::Error error, const QString &message)
{
    Q_D(QtIVIAbstractFeatureListModel);
    d->m_feature->setError(error, message);
}
