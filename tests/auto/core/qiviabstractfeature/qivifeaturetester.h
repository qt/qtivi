/****************************************************************************
**
** Copyright (C) 2017 Pelagicore AG
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtIvi module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:GPL-EXCEPT-QTAS$
** Commercial License Usage
** Licensees holding valid commercial Qt Automotive Suite licenses may use
** this file in accordance with the commercial license agreement provided
** with the Software or, alternatively, in accordance with the terms
** contained in a written agreement between you and The Qt Company.  For
** licensing terms and conditions see https://www.qt.io/terms-conditions.
** For further information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 as published by the Free Software
** Foundation with exceptions as appearing in the file LICENSE.GPL3-EXCEPT
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QIVIFEATURETESTER_H
#define QIVIFEATURETESTER_H

#include <QIviAbstractFeature>
#include <QIviAbstractFeatureListModel>

QT_BEGIN_NAMESPACE

class QIviFeatureTester : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QIviAbstractFeature::DiscoveryMode discoveryMode READ discoveryMode WRITE setDiscoveryMode NOTIFY discoveryModeChanged)
    Q_PROPERTY(QIviAbstractFeature::DiscoveryResult discoveryResult READ discoveryResult NOTIFY discoveryResultChanged)
    Q_PROPERTY(QIviServiceObject *serviceObject READ serviceObject WRITE setServiceObject NOTIFY serviceObjectChanged)
    Q_PROPERTY(bool isValid READ isValid NOTIFY isValidChanged)
    Q_PROPERTY(bool isInitialized READ isInitialized NOTIFY isInitializedChanged)
    Q_PROPERTY(QString error READ errorMessage NOTIFY errorChanged)

public:
    explicit QIviFeatureTester(QIviAbstractFeature *feature, QObject *parent = nullptr)
        : QObject(parent)
        , m_feature(feature)
        , m_featureListModel(nullptr)
    {
        connect(m_feature, &QIviAbstractFeature::discoveryModeChanged, this, &QIviFeatureTester::discoveryModeChanged);
        connect(m_feature, &QIviAbstractFeature::discoveryResultChanged, this, &QIviFeatureTester::discoveryResultChanged);
        connect(m_feature, &QIviAbstractFeature::serviceObjectChanged, this, &QIviFeatureTester::serviceObjectChanged);
        connect(m_feature, &QIviAbstractFeature::isValidChanged, this, &QIviFeatureTester::isValidChanged);
        connect(m_feature, &QIviAbstractFeature::isInitializedChanged, this, &QIviFeatureTester::isInitializedChanged);
        connect(m_feature, &QIviAbstractFeature::errorChanged, this, &QIviFeatureTester::errorChanged);
    }

    explicit QIviFeatureTester(QIviAbstractFeatureListModel *featureModel, QObject *parent = nullptr)
        : QObject(parent)
        , m_feature(nullptr)
        , m_featureListModel(featureModel)
    {
        connect(m_featureListModel, &QIviAbstractFeatureListModel::discoveryModeChanged, this, &QIviFeatureTester::discoveryModeChanged);
        connect(m_featureListModel, &QIviAbstractFeatureListModel::discoveryResultChanged, this, &QIviFeatureTester::discoveryResultChanged);
        connect(m_featureListModel, &QIviAbstractFeatureListModel::serviceObjectChanged, this, &QIviFeatureTester::serviceObjectChanged);
        connect(m_featureListModel, &QIviAbstractFeatureListModel::isValidChanged, this, &QIviFeatureTester::isValidChanged);
        connect(m_featureListModel, &QIviAbstractFeatureListModel::isInitializedChanged, this, &QIviFeatureTester::isInitializedChanged);
        connect(m_featureListModel, &QIviAbstractFeatureListModel::errorChanged, this, &QIviFeatureTester::errorChanged);
    }

    QIviServiceObject *serviceObject() const
    {
        return m_feature ? m_feature->serviceObject() : m_featureListModel->serviceObject();
    }

    QIviAbstractFeature::DiscoveryMode discoveryMode() const
    {
        return m_feature ? m_feature->discoveryMode() : m_featureListModel->discoveryMode();
    }

    QIviAbstractFeature::DiscoveryResult discoveryResult() const
    {
        return m_feature ? m_feature->discoveryResult() : m_featureListModel->discoveryResult();
    }

    bool isValid() const
    {
        return m_feature ? m_feature->isValid() : m_featureListModel->isValid();
    }

    bool isInitialized() const
    {
        return m_feature ? m_feature->isInitialized() : m_featureListModel->isInitialized();
    }

    QIviAbstractFeature::Error error() const
    {
        return m_feature ? m_feature->error() : m_featureListModel->error();
    }

    QString errorMessage() const
    {
        return m_feature ? m_feature->errorMessage() : m_featureListModel->errorMessage();
    }

    QString errorText() const
    {
        return m_feature ? m_feature->errorText() : m_featureListModel->errorText();
    }

public Q_SLOTS:
    bool setServiceObject(QIviServiceObject *so)
    {
        return m_feature ? m_feature->setServiceObject(so) : m_featureListModel->setServiceObject(so);
    }
    void setDiscoveryMode(QIviAbstractFeature::DiscoveryMode discoveryMode)
    {
        return m_feature ? m_feature->setDiscoveryMode(discoveryMode) : m_featureListModel->setDiscoveryMode(discoveryMode);
    }
    QIviAbstractFeature::DiscoveryResult startAutoDiscovery()
    {
        return m_feature ? m_feature->startAutoDiscovery() : m_featureListModel->startAutoDiscovery();
    }

Q_SIGNALS:
    void serviceObjectChanged();
    void discoveryModeChanged(QIviAbstractFeature::DiscoveryMode discoveryMode);
    void discoveryResultChanged(QIviAbstractFeature::DiscoveryResult discoveryResult);
    void isValidChanged(bool arg);
    void isInitializedChanged(bool isInitialized);
    void errorChanged(QIviAbstractFeature::Error error, const QString &message);

private:
    QIviAbstractFeature *m_feature;
    QIviAbstractFeatureListModel *m_featureListModel;
};

QT_END_NAMESPACE

#endif // QIVIFEATURETESTER_H
