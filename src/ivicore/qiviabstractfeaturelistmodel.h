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

#ifndef QIVIABSTRACTFEATURELISTMODEL_H
#define QIVIABSTRACTFEATURELISTMODEL_H

#include <QAbstractListModel>
#include <QtIviCore/QIviAbstractFeature>

QT_BEGIN_NAMESPACE

class QIviAbstractFeatureListModelPrivate;

class Q_QTIVICORE_EXPORT QIviAbstractFeatureListModel : public QAbstractListModel, public QQmlParserStatus
{
    Q_OBJECT
    Q_INTERFACES(QQmlParserStatus)

    Q_PROPERTY(QIviAbstractFeature::DiscoveryMode discoveryMode READ discoveryMode WRITE setDiscoveryMode NOTIFY discoveryModeChanged)
    Q_PROPERTY(QIviAbstractFeature::DiscoveryResult discoveryResult READ discoveryResult NOTIFY discoveryResultChanged)
    Q_PROPERTY(QIviServiceObject *serviceObject READ serviceObject WRITE setServiceObject NOTIFY serviceObjectChanged)
    Q_PROPERTY(bool isValid READ isValid NOTIFY isValidChanged)
    Q_PROPERTY(bool isInitialized READ isInitialized NOTIFY isInitializedChanged)
    Q_PROPERTY(QString error READ errorMessage NOTIFY errorChanged)

public:

    explicit QIviAbstractFeatureListModel(const QString &interfaceName, QObject *parent = nullptr);

    QIviServiceObject *serviceObject() const;
    QIviAbstractFeature::DiscoveryMode discoveryMode() const;
    QIviAbstractFeature::DiscoveryResult discoveryResult() const;
    bool isValid() const;
    bool isInitialized() const;
    QIviAbstractFeature::Error error() const;
    QString errorMessage() const;

public Q_SLOTS:
    bool setServiceObject(QIviServiceObject *so);
    void setDiscoveryMode(QIviAbstractFeature::DiscoveryMode discoveryMode);
    QIviAbstractFeature::DiscoveryResult startAutoDiscovery();

Q_SIGNALS:
    void serviceObjectChanged();
    void discoveryModeChanged(QIviAbstractFeature::DiscoveryMode discoveryMode);
    void discoveryResultChanged(QIviAbstractFeature::DiscoveryResult discoveryResult);
    void isValidChanged(bool arg);
    void isInitializedChanged(bool isInitialized);
    void errorChanged(QIviAbstractFeature::Error error, const QString &message);

protected:
    QIviAbstractFeatureListModel(QIviAbstractFeatureListModelPrivate &dd, QObject *parent = nullptr);

    virtual bool acceptServiceObject(QIviServiceObject*);
    virtual void connectToServiceObject(QIviServiceObject*);
    virtual void disconnectFromServiceObject(QIviServiceObject*);
    virtual void clearServiceObject() = 0;

     void classBegin() override;
     void componentComplete() override;

    QString interfaceName() const;
    QString errorText() const;
    void setError(QIviAbstractFeature::Error error, const QString &message = QString());

protected Q_SLOTS:
    virtual void onErrorChanged(QIviAbstractFeature::Error error, const QString &message = QString());

private:
    Q_DECLARE_PRIVATE(QIviAbstractFeatureListModel)
    friend class QIviHelperFeature;
    friend class QIviFeatureTester;
};

QT_END_NAMESPACE

#endif // QIVIABSTRACTFEATURELISTMODEL_H
