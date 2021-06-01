/****************************************************************************
**
** Copyright (C) 2021 The Qt Company Ltd.
** Copyright (C) 2019 Luxoft Sweden AB
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtIvi module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
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
****************************************************************************/

#include "searchandbrowsemodel.h"
#include "qiviqmlconversion_helper.h"

#include <QTimer>
#include <QSettings>

Q_LOGGING_CATEGORY(qLcROQIviSearchAndBrowseModel, "qtivi.media.qivisearchandbrowsebackend.remoteobjects", QtInfoMsg)

QDataStream &operator<<(QDataStream &stream, const SearchAndBrowseItem &obj)
{
    stream << obj.name();
    stream << obj.type();
    stream << obj.url();
    stream << QVariant(obj.data());
    return stream;
}

QDataStream &operator>>(QDataStream &stream, SearchAndBrowseItem &obj)
{
    QString name;
    QString type;
    QUrl url;
    QVariant data;
    stream >> name;
    stream >> type;
    stream >> url;
    stream >> data;
    obj.setName(name);
    obj.setType(type);
    obj.setUrl(url);
    obj.setData(data.toMap());
    return stream;
}

SearchAndBrowseModel::SearchAndBrowseModel(QObject *parent, const QString& remoteObjectsLookupName)
    : QIviSearchAndBrowseModelInterface(parent)
    , m_remoteObjectsLookupName(remoteObjectsLookupName)
    , m_node(nullptr)
    , m_helper(new QIviRemoteObjectReplicaHelper(qLcROQIviSearchAndBrowseModel(), this))
{
    qRegisterMetaType<SearchAndBrowseItem>();
    qRegisterMetaType<QIviAudioTrackItem>();
}

void SearchAndBrowseModel::initialize()
{
    if (!connectToNode())
        return;

    if (m_replica->isInitialized()) {
        emit availableContentTypesChanged(m_replica->availableContentTypes());
        emit initializationDone();
    }

    QTimer::singleShot(3000, this, [this](){
        if (!m_replica->isInitialized())
            qCCritical(qLcROQIviSearchAndBrowseModel) << "QIviSearchAndBrowseModel wasn't initialized within the timeout period. Please make sure the server is running.";
    });
}

void SearchAndBrowseModel::registerInstance(const QUuid &identifier)
{
    m_replica->registerInstance(identifier);
}

void SearchAndBrowseModel::unregisterInstance(const QUuid &identifier)
{
    m_replica->unregisterInstance(identifier);
}

void SearchAndBrowseModel::fetchData(const QUuid &identifier, int start, int count)
{
    m_replica->fetchData(identifier, start, count);
}

void SearchAndBrowseModel::setContentType(const QUuid &identifier, const QString &contentType)
{
    m_replica->setContentType(identifier, contentType);
}

void SearchAndBrowseModel::setupFilter(const QUuid &identifier, QIviAbstractQueryTerm *term, const QList<QIviOrderTerm> &orderTerms)
{
    QByteArray data;
    QDataStream stream(&data, QIODevice::WriteOnly);
    if (term)
        stream << term;
    m_replica->setupFilter(identifier, QVariant(data), orderTerms);
}

QIviPendingReply<QString> SearchAndBrowseModel::goBack(const QUuid &identifier)
{
    QRemoteObjectPendingReply<QVariant> reply = m_replica->goBack(identifier);
    auto iviReply = m_helper->toQIviPendingReply<QString>(reply);

    //Pass an empty std::function to only handle errors.
    iviReply.then(std::function<void(QString)>(), [this]() {
        emit errorChanged(QIviAbstractFeature::InvalidOperation, QStringLiteral("remote call of method goBack failed"));
    });
    return iviReply;
}

QIviPendingReply<QString> SearchAndBrowseModel::goForward(const QUuid &identifier, int index)
{
    QRemoteObjectPendingReply<QVariant> reply = m_replica->goForward(identifier, index);

    //Pass an empty std::function to only handle errors.
    auto iviReply = m_helper->toQIviPendingReply<QString>(reply);
    iviReply.then(std::function<void(QString)>(), [this]() {
        emit errorChanged(QIviAbstractFeature::InvalidOperation, QStringLiteral("remote call of method goForward failed"));
    });
    return iviReply;
}

QIviPendingReply<void> SearchAndBrowseModel::insert(const QUuid &identifier, int index, const QVariant &item)
{
    QRemoteObjectPendingReply<QVariant> reply = m_replica->insert(identifier, index, item);

    //Pass an empty std::function to only handle errors.
    auto iviReply = m_helper->toQIviPendingReply<void>(reply);
    iviReply.then(std::function<void()>(), [this]() {
        emit errorChanged(QIviAbstractFeature::InvalidOperation, QStringLiteral("remote call of method insert failed"));
    });
    return iviReply;
}

QIviPendingReply<void> SearchAndBrowseModel::remove(const QUuid &identifier, int index)
{
    QRemoteObjectPendingReply<QVariant> reply = m_replica->remove(identifier, index);

    //Pass an empty std::function to only handle errors.
    auto iviReply = m_helper->toQIviPendingReply<void>(reply);
    iviReply.then(std::function<void()>(), [this]() {
        emit errorChanged(QIviAbstractFeature::InvalidOperation, QStringLiteral("remote call of method remove failed"));
    });
    return iviReply;
}

QIviPendingReply<void> SearchAndBrowseModel::move(const QUuid &identifier, int currentIndex, int newIndex)
{
    QRemoteObjectPendingReply<QVariant> reply = m_replica->move(identifier, currentIndex, newIndex);

    //Pass an empty std::function to only handle errors.
    auto iviReply = m_helper->toQIviPendingReply<void>(reply);
    iviReply.then(std::function<void()>(), [this]() {
        emit errorChanged(QIviAbstractFeature::InvalidOperation, QStringLiteral("remote call of method move failed"));
    });
    return iviReply;
}

QIviPendingReply<int> SearchAndBrowseModel::indexOf(const QUuid &identifier, const QVariant &item)
{
    QRemoteObjectPendingReply<QVariant> reply = m_replica->indexOf(identifier, item);

    //Pass an empty std::function to only handle errors.
    auto iviReply = m_helper->toQIviPendingReply<int>(reply);
    iviReply.then(std::function<void(int)>(), [this]() {
        emit errorChanged(QIviAbstractFeature::InvalidOperation, QStringLiteral("remote call of method indexOf failed"));
    });
    return iviReply;
}

bool SearchAndBrowseModel::connectToNode()
{
    static QString configPath;
    if (configPath.isEmpty()) {
        if (qEnvironmentVariableIsSet("SERVER_CONF_PATH")) {
            configPath = QString::fromLocal8Bit(qgetenv("SERVER_CONF_PATH"));
        } else {
            configPath = QStringLiteral("./server.conf");
            qCInfo(qLcROQIviSearchAndBrowseModel) << "Environment variable SERVER_CONF_PATH not defined, using " << configPath;
        }
    }

    QSettings settings(configPath, QSettings::IniFormat);
    settings.beginGroup(QStringLiteral("qtivimedia"));
    QUrl registryUrl = QUrl(settings.value(QStringLiteral("Registry"), QStringLiteral("local:qtivimedia")).toString());
    if (m_url != registryUrl) {
        m_url = registryUrl;
        // QtRO doesn't allow to change the URL without destroying the Node
        delete m_node;
        m_node = new QRemoteObjectNode(this);
        if (!m_node->connectToNode(m_url)) {
            qCCritical(qLcROQIviSearchAndBrowseModel) << "Connection to" << m_url << "failed!";
            m_replica.reset();
            return false;
        }
        qCInfo(qLcROQIviSearchAndBrowseModel) << "Connecting to" << m_url;
        m_replica.reset(m_node->acquire<QIviSearchAndBrowseModelReplica>(m_remoteObjectsLookupName));
        setupConnections();
    }
    return true;
}

void SearchAndBrowseModel::setupConnections()
{
    connect(m_node, &QRemoteObjectNode::error, m_helper, &QIviRemoteObjectReplicaHelper::onNodeError);
    connect(m_helper, &QIviRemoteObjectReplicaHelper::errorChanged, this, &QIviFeatureInterface::errorChanged);
    connect(m_replica.data(), &QRemoteObjectReplica::stateChanged, m_helper, &QIviRemoteObjectReplicaHelper::onReplicaStateChanged);
    connect(m_replica.data(), &QRemoteObjectReplica::initialized, this, &QIviFeatureInterface::initializationDone);
    connect(m_replica.data(), &QIviSearchAndBrowseModelReplica::pendingResultAvailable, m_helper, &QIviRemoteObjectReplicaHelper::onPendingResultAvailable);
    connect(m_replica.data(), &QIviSearchAndBrowseModelReplica::canGoBackChanged, this, &SearchAndBrowseModel::canGoBackChanged);
    connect(m_replica.data(), &QIviSearchAndBrowseModelReplica::canGoForwardChanged, this, &SearchAndBrowseModel::canGoForwardChanged);
    connect(m_replica.data(), &QIviSearchAndBrowseModelReplica::supportedCapabilitiesChanged, this, &SearchAndBrowseModel::supportedCapabilitiesChanged);
    connect(m_replica.data(), &QIviSearchAndBrowseModelReplica::queryIdentifiersChanged, this, &SearchAndBrowseModel::queryIdentifiersChanged);
    connect(m_replica.data(), &QIviSearchAndBrowseModelReplica::availableContentTypesChanged, this, &SearchAndBrowseModel::availableContentTypesChanged);
    connect(m_replica.data(), &QIviSearchAndBrowseModelReplica::contentTypeChanged, this, &SearchAndBrowseModel::contentTypeChanged);
    connect(m_replica.data(), &QIviSearchAndBrowseModelReplica::countChanged, this, &SearchAndBrowseModel::countChanged);
    connect(m_replica.data(), &QIviSearchAndBrowseModelReplica::dataFetched, this, &SearchAndBrowseModel::dataFetched);
    connect(m_replica.data(), &QIviSearchAndBrowseModelReplica::dataChanged, this, &SearchAndBrowseModel::dataChanged);
}
