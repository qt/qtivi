/****************************************************************************
**
** Copyright (C) 2019 Luxoft Sweden AB
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

#include "searchandbrowsemodel.h"
#include "qiviqmlconversion_helper.h"

#include <QTimer>

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

SearchAndBrowseModel::SearchAndBrowseModel(QRemoteObjectNode *node, QObject *parent)
    : QIviSearchAndBrowseModelInterface(parent)
{
    qRegisterMetaType<SearchAndBrowseItem>();
    qRegisterMetaTypeStreamOperators<SearchAndBrowseItem>();
    qRegisterMetaType<QIviAudioTrackItem>();
    qRegisterMetaTypeStreamOperators<QIviAudioTrackItem>();

    m_replica.reset(node->acquire<QIviSearchAndBrowseModelReplica>(QStringLiteral("QIviSearchAndBrowseModel")));

    connect(node, &QRemoteObjectNode::error, this, &SearchAndBrowseModel::onNodeError);
    connect(m_replica.data(), &QRemoteObjectReplica::initialized, this, &QIviFeatureInterface::initializationDone);
    connect(m_replica.data(), &QRemoteObjectReplica::stateChanged, this, &SearchAndBrowseModel::onReplicaStateChanged);
    connect(m_replica.data(), &QIviSearchAndBrowseModelReplica::pendingResultAvailable, this, &SearchAndBrowseModel::onPendingResultAvailable);
    connect(m_replica.data(), &QIviSearchAndBrowseModelReplica::canGoBackChanged, this, &SearchAndBrowseModel::canGoBackChanged);
    connect(m_replica.data(), &QIviSearchAndBrowseModelReplica::canGoForwardChanged, this, &SearchAndBrowseModel::canGoForwardChanged);
    connect(m_replica.data(), &QIviSearchAndBrowseModelReplica::supportedCapabilitiesChanged, this, &SearchAndBrowseModel::supportedCapabilitiesChanged);
    connect(m_replica.data(), &QIviSearchAndBrowseModelReplica::queryIdentifiersChanged, this, &SearchAndBrowseModel::queryIdentifiersChanged);
    connect(m_replica.data(), &QIviSearchAndBrowseModelReplica::availableContentTypesChanged, this, &SearchAndBrowseModel::availableContentTypesChanged);
    connect(m_replica.data(), &QIviSearchAndBrowseModelReplica::contentTypeChanged, this, &SearchAndBrowseModel::contentTypeChanged);
    connect(m_replica.data(), &QIviSearchAndBrowseModelReplica::countChanged, this, &SearchAndBrowseModel::countChanged);
    connect(m_replica.data(), &QIviSearchAndBrowseModelReplica::dataFetched, this, &SearchAndBrowseModel::dataFetched);
    connect(m_replica.data(), &QIviSearchAndBrowseModelReplica::dataChanged, this, &SearchAndBrowseModel::dataChanged);

    QTimer::singleShot(3000, this, [this](){
        if (!m_replica->isInitialized())
            qCCritical(qLcROQIviSearchAndBrowseModel) << "QIviSearchAndBrowseModel wasn't initialized within the timeout period. Please make sure the server is running.";
    });
}

void SearchAndBrowseModel::initialize()
{
    if (m_replica->isInitialized()) {
        emit availableContentTypesChanged(m_replica->availableContentTypes());
        emit initializationDone();
    }
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
    QIviPendingReply<QString> iviReply;
    QRemoteObjectPendingReply<QVariant> reply = m_replica->goBack(identifier);
    auto watcher = new QRemoteObjectPendingCallWatcher(reply);
    connect(watcher, &QRemoteObjectPendingCallWatcher::finished, this, [this, iviReply](QRemoteObjectPendingCallWatcher *self) mutable {
        if (self->error() == QRemoteObjectPendingCallWatcher::NoError) {
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 1)
            QVariant value = self->returnValue();
#else
            QVariant value = self->returnValue().value<QVariant>();
#endif
            if (value.canConvert<QIviSearchAndBrowseModelPendingResult>()) {
                QIviSearchAndBrowseModelPendingResult result = value.value<QIviSearchAndBrowseModelPendingResult>();
                if (result.failed()) {
                    qCDebug(qLcROQIviSearchAndBrowseModel) << "Pending Result with id:" << result.id() << "failed";
                    iviReply.setFailed();
                } else {
                    qCDebug(qLcROQIviSearchAndBrowseModel) << "Result not available yet. Waiting for id:" << result.id();
                    m_pendingReplies.insert(result.id(), iviReply);
                }
            } else {
                qCDebug(qLcROQIviSearchAndBrowseModel) << "Got the value right away:" << value.value<QString>();
                iviReply.setSuccess(value.toString());
            }
        } else {
            iviReply.setFailed();
            emit errorChanged(QIviAbstractFeature::InvalidOperation, QStringLiteral("remote call of method insert failed"));
        }
        self->deleteLater();
    });
    return iviReply;
}

QIviPendingReply<QString> SearchAndBrowseModel::goForward(const QUuid &identifier, int index)
{
    QIviPendingReply<QString> iviReply;
    QRemoteObjectPendingReply<QVariant> reply = m_replica->goForward(identifier, index);
    auto watcher = new QRemoteObjectPendingCallWatcher(reply);
    connect(watcher, &QRemoteObjectPendingCallWatcher::finished, this, [this, iviReply](QRemoteObjectPendingCallWatcher *self) mutable {
        if (self->error() == QRemoteObjectPendingCallWatcher::NoError) {
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 1)
            QVariant value = self->returnValue();
#else
            QVariant value = self->returnValue().value<QVariant>();
#endif
            if (value.canConvert<QIviSearchAndBrowseModelPendingResult>()) {
                QIviSearchAndBrowseModelPendingResult result = value.value<QIviSearchAndBrowseModelPendingResult>();
                if (result.failed()) {
                    qCDebug(qLcROQIviSearchAndBrowseModel) << "Pending Result with id:" << result.id() << "failed";
                    iviReply.setFailed();
                } else {
                    qCDebug(qLcROQIviSearchAndBrowseModel) << "Result not available yet. Waiting for id:" << result.id();
                    m_pendingReplies.insert(result.id(), iviReply);
                }
            } else {
                qCDebug(qLcROQIviSearchAndBrowseModel) << "Got the value right away:" << value.value<QString>();
                iviReply.setSuccess(value.toString());
            }
        } else {
            iviReply.setFailed();
            emit errorChanged(QIviAbstractFeature::InvalidOperation, QStringLiteral("remote call of method insert failed"));
        }
        self->deleteLater();
    });
    return iviReply;
}

QIviPendingReply<void> SearchAndBrowseModel::insert(const QUuid &identifier, int index, const QVariant &item)
{
    QIviPendingReply<void> iviReply;
    QRemoteObjectPendingReply<QVariant> reply = m_replica->insert(identifier, index, item);
    auto watcher = new QRemoteObjectPendingCallWatcher(reply);
    connect(watcher, &QRemoteObjectPendingCallWatcher::finished, this, [this, iviReply](QRemoteObjectPendingCallWatcher *self) mutable {
        if (self->error() == QRemoteObjectPendingCallWatcher::NoError) {
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 1)
            QVariant value = self->returnValue();
#else
            QVariant value = self->returnValue().value<QVariant>();
#endif
            if (value.canConvert<QIviSearchAndBrowseModelPendingResult>()) {
                QIviSearchAndBrowseModelPendingResult result = value.value<QIviSearchAndBrowseModelPendingResult>();
                if (result.failed()) {
                    qCDebug(qLcROQIviSearchAndBrowseModel) << "Pending Result with id:" << result.id() << "failed";
                    iviReply.setFailed();
                } else {
                    qCDebug(qLcROQIviSearchAndBrowseModel) << "Result not available yet. Waiting for id:" << result.id();
                    m_pendingReplies.insert(result.id(), iviReply);
                }
            } else {
                qCDebug(qLcROQIviSearchAndBrowseModel) << "Got the value right away: void";
                iviReply.setSuccess();
            }
        } else {
            iviReply.setFailed();
            emit errorChanged(QIviAbstractFeature::InvalidOperation, QStringLiteral("remote call of method insert failed"));
        }
        self->deleteLater();
    });
    return iviReply;
}

QIviPendingReply<void> SearchAndBrowseModel::remove(const QUuid &identifier, int index)
{
    QIviPendingReply<void> iviReply;
    QRemoteObjectPendingReply<QVariant> reply = m_replica->remove(identifier, index);
    auto watcher = new QRemoteObjectPendingCallWatcher(reply);
    connect(watcher, &QRemoteObjectPendingCallWatcher::finished, this, [this, iviReply](QRemoteObjectPendingCallWatcher *self) mutable {
        if (self->error() == QRemoteObjectPendingCallWatcher::NoError) {
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 1)
            QVariant value = self->returnValue();
#else
            QVariant value = self->returnValue().value<QVariant>();
#endif
            if (value.canConvert<QIviSearchAndBrowseModelPendingResult>()) {
                QIviSearchAndBrowseModelPendingResult result = value.value<QIviSearchAndBrowseModelPendingResult>();
                if (result.failed()) {
                    qCDebug(qLcROQIviSearchAndBrowseModel) << "Pending Result with id:" << result.id() << "failed";
                    iviReply.setFailed();
                } else {
                    qCDebug(qLcROQIviSearchAndBrowseModel) << "Result not available yet. Waiting for id:" << result.id();
                    m_pendingReplies.insert(result.id(), iviReply);
                }
            } else {
                qCDebug(qLcROQIviSearchAndBrowseModel) << "Got the value right away: void";
                iviReply.setSuccess();
            }
        } else {
            iviReply.setFailed();
            emit errorChanged(QIviAbstractFeature::InvalidOperation, QStringLiteral("remote call of method remove failed"));
        }
        self->deleteLater();
    });
    return iviReply;
}

QIviPendingReply<void> SearchAndBrowseModel::move(const QUuid &identifier, int currentIndex, int newIndex)
{
    QIviPendingReply<void> iviReply;
    QRemoteObjectPendingReply<QVariant> reply = m_replica->move(identifier, currentIndex, newIndex);
    auto watcher = new QRemoteObjectPendingCallWatcher(reply);
    connect(watcher, &QRemoteObjectPendingCallWatcher::finished, this, [this, iviReply](QRemoteObjectPendingCallWatcher *self) mutable {
        if (self->error() == QRemoteObjectPendingCallWatcher::NoError) {
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 1)
            QVariant value = self->returnValue();
#else
            QVariant value = self->returnValue().value<QVariant>();
#endif
            if (value.canConvert<QIviSearchAndBrowseModelPendingResult>()) {
                QIviSearchAndBrowseModelPendingResult result = value.value<QIviSearchAndBrowseModelPendingResult>();
                if (result.failed()) {
                    qCDebug(qLcROQIviSearchAndBrowseModel) << "Pending Result with id:" << result.id() << "failed";
                    iviReply.setFailed();
                } else {
                    qCDebug(qLcROQIviSearchAndBrowseModel) << "Result not available yet. Waiting for id:" << result.id();
                    m_pendingReplies.insert(result.id(), iviReply);
                }
            } else {
                qCDebug(qLcROQIviSearchAndBrowseModel) << "Got the value right away: void";
                iviReply.setSuccess();
            }
        } else {
            iviReply.setFailed();
            emit errorChanged(QIviAbstractFeature::InvalidOperation, QStringLiteral("remote call of method move failed"));
        }
        self->deleteLater();
    });
    return iviReply;
}

QIviPendingReply<int> SearchAndBrowseModel::indexOf(const QUuid &identifier, const QVariant &item)
{
    QIviPendingReply<int> iviReply;
    QRemoteObjectPendingReply<QVariant> reply = m_replica->indexOf(identifier, item);
    auto watcher = new QRemoteObjectPendingCallWatcher(reply);
    connect(watcher, &QRemoteObjectPendingCallWatcher::finished, this, [this, iviReply](QRemoteObjectPendingCallWatcher *self) mutable {
        if (self->error() == QRemoteObjectPendingCallWatcher::NoError) {
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 1)
            QVariant value = self->returnValue();
#else
            QVariant value = self->returnValue().value<QVariant>();
#endif
            if (value.canConvert<QIviSearchAndBrowseModelPendingResult>()) {
                QIviSearchAndBrowseModelPendingResult result = value.value<QIviSearchAndBrowseModelPendingResult>();
                if (result.failed()) {
                    qCDebug(qLcROQIviSearchAndBrowseModel) << "Pending Result with id:" << result.id() << "failed";
                    iviReply.setFailed();
                } else {
                    qCDebug(qLcROQIviSearchAndBrowseModel) << "Result not available yet. Waiting for id:" << result.id();
                    m_pendingReplies.insert(result.id(), iviReply);
                }
            } else {
                qCDebug(qLcROQIviSearchAndBrowseModel) << "Got the value right away:" << value.value<int>();
                iviReply.setSuccess(value.value<int>());
            }
        } else {
            iviReply.setFailed();
            emit errorChanged(QIviAbstractFeature::InvalidOperation, QStringLiteral("remote call of method remove failed"));
        }
        self->deleteLater();
    });
    return iviReply;
}

void SearchAndBrowseModel::onReplicaStateChanged(QRemoteObjectReplica::State newState, QRemoteObjectReplica::State oldState)
{
    Q_UNUSED(oldState)

    if (newState == QRemoteObjectReplica::Suspect) {
        qCWarning(qLcROQIviSearchAndBrowseModel) << "QRemoteObjectReplica error, connection to the source lost";
        emit errorChanged(QIviAbstractFeature::Unknown,
                        "QRemoteObjectReplica error, connection to the source lost");
    } else if (newState == QRemoteObjectReplica::SignatureMismatch) {
        qCWarning(qLcROQIviSearchAndBrowseModel) << "QRemoteObjectReplica error, signature mismatch";
        emit errorChanged(QIviAbstractFeature::Unknown,
                        "QRemoteObjectReplica error, signature mismatch");
    } else if (newState == QRemoteObjectReplica::Valid) {
        emit errorChanged(QIviAbstractFeature::NoError, "");
    }
}

void SearchAndBrowseModel::onNodeError(QRemoteObjectNode::ErrorCode code)
{
    qCWarning(qLcROQIviSearchAndBrowseModel) << "QRemoteObjectNode error, code: " << code;
    emit errorChanged(QIviAbstractFeature::Unknown, "QRemoteObjectNode error, code: " + code);
}

void SearchAndBrowseModel::onPendingResultAvailable(quint64 id, bool isSuccess, const QVariant &value)
{
    qCDebug(qLcROQIviSearchAndBrowseModel) << "pending result available" << id;
    if (!m_pendingReplies.contains(id)) {
        qCDebug(qLcROQIviSearchAndBrowseModel) << "Received a result for an unexpected id:" << id << ". Ignoring!";
        return;
    }

    QIviPendingReplyBase iviReply = m_pendingReplies.take(id);

    if (isSuccess)
        iviReply.setSuccess(value);
    else
        iviReply.setFailed();
}

