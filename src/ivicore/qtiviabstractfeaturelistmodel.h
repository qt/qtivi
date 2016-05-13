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

#ifndef QTIVIABSTRACTFEATURELISTMODEL_H
#define QTIVIABSTRACTFEATURELISTMODEL_H

#include <QAbstractListModel>
#include <QtIVICore/QtIVIAbstractFeature>

QT_BEGIN_NAMESPACE

class QtIVIAbstractFeatureListModelPrivate;

class Q_QTIVICORE_EXPORT QtIVIAbstractFeatureListModel : public QAbstractListModel
{
    Q_OBJECT

    Q_PROPERTY(QtIVIAbstractFeature::DiscoveryMode discoveryMode READ discoveryMode WRITE setDiscoveryMode NOTIFY discoveryModeChanged)
    Q_PROPERTY(QtIVIAbstractFeature::DiscoveryResult discoveryResult READ discoveryResult NOTIFY discoveryResultChanged)
    Q_PROPERTY(QtIVIServiceObject* serviceObject READ serviceObject WRITE setServiceObject NOTIFY serviceObjectChanged)
    Q_PROPERTY(bool isValid READ isValid NOTIFY isValidChanged)
    Q_PROPERTY(QString error READ errorMessage NOTIFY errorChanged)

public:

    explicit QtIVIAbstractFeatureListModel(const QString &interface, QObject *parent = 0);
    ~QtIVIAbstractFeatureListModel();

    QtIVIServiceObject *serviceObject() const;
    QtIVIAbstractFeature::DiscoveryMode discoveryMode() const;
    QtIVIAbstractFeature::DiscoveryResult discoveryResult() const;
    bool isValid() const;
    QtIVIAbstractFeature::Error error() const;
    QString errorMessage() const;

public Q_SLOTS:
    bool setServiceObject(QtIVIServiceObject *so);
    void setDiscoveryMode(QtIVIAbstractFeature::DiscoveryMode discoveryMode);
    QtIVIAbstractFeature::DiscoveryResult startAutoDiscovery();

Q_SIGNALS:
    void serviceObjectChanged();
    void discoveryModeChanged(QtIVIAbstractFeature::DiscoveryMode discoveryMode);
    void discoveryResultChanged(QtIVIAbstractFeature::DiscoveryResult discoveryResult);
    void isValidChanged(bool arg);
    void errorChanged(QtIVIAbstractFeature::Error error, const QString &message);

protected:
    QtIVIAbstractFeatureListModel(QtIVIAbstractFeatureListModelPrivate &dd, QObject *parent = Q_NULLPTR);

    virtual bool acceptServiceObject(QtIVIServiceObject*) = 0;
    virtual void connectToServiceObject(QtIVIServiceObject*) = 0;
    virtual void disconnectFromServiceObject(QtIVIServiceObject*) = 0;
    virtual void clearServiceObject() = 0;

    QString interfaceName() const;
    QString errorText() const;
    void setError(QtIVIAbstractFeature::Error error, const QString &message = QString());

private:
    Q_DECLARE_PRIVATE(QtIVIAbstractFeatureListModel)
    friend class QtIVIHelperFeature;
};

QT_END_NAMESPACE

#endif // QTIVIABSTRACTFEATURELISTMODEL_H
