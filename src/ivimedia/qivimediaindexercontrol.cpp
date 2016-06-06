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

#include "qivimediaindexercontrol.h"
#include "qivimediaindexercontrol_p.h"

#include <QIviServiceObject>
#include <QtDebug>

QIviMediaIndexerControlPrivate::QIviMediaIndexerControlPrivate(const QString &interface, QIviMediaIndexerControl *parent)
    : QIviAbstractFeaturePrivate(interface, parent)
    , q_ptr(parent)
    , m_progress(0)
    , m_state(QIviMediaIndexerControl::Idle)
{
}

void QIviMediaIndexerControlPrivate::init()
{
}

void QIviMediaIndexerControlPrivate::clearToDefaults()
{
    m_progress = 0;
    m_state = QIviMediaIndexerControl::Idle;
}

void QIviMediaIndexerControlPrivate::onProgressChanged(qreal progress)
{
    if (qFuzzyCompare(m_progress, progress))
        return;

    Q_Q(QIviMediaIndexerControl);
    m_progress = progress;
    emit q->progressChanged(progress);
}

void QIviMediaIndexerControlPrivate::onStateChanged(QIviMediaIndexerControl::State state)
{
    if (m_state == state)
        return;

    Q_Q(QIviMediaIndexerControl);
    m_state = state;
    emit q->stateChanged(state);
}

QIviMediaIndexerControlBackendInterface *QIviMediaIndexerControlPrivate::indexerBackend() const
{
    Q_Q(const QIviMediaIndexerControl);
    QIviServiceObject *so = q->serviceObject();
    if (so)
        return qobject_cast<QIviMediaIndexerControlBackendInterface*>(so->interfaceInstance(QIviStringMediaIndexerInterfaceName));

    return nullptr;
}

QIviMediaIndexerControl::QIviMediaIndexerControl(QObject *parent)
    : QIviAbstractFeature(*new QIviMediaIndexerControlPrivate(QIviStringMediaIndexerInterfaceName, this), parent)
{
    Q_D(QIviMediaIndexerControl);
    d->init();
}

qreal QIviMediaIndexerControl::progress() const
{
    Q_D(const QIviMediaIndexerControl);
    return d->m_progress;
}

QIviMediaIndexerControl::State QIviMediaIndexerControl::state() const
{
    Q_D(const QIviMediaIndexerControl);
    return d->m_state;
}

void QIviMediaIndexerControl::pause()
{
    Q_D(QIviMediaIndexerControl);
    QIviMediaIndexerControlBackendInterface *backend = d->indexerBackend();
    if (!backend) {
        qWarning("Can't pause without a connected backend");
        return;
    }

    backend->pause();
}

void QIviMediaIndexerControl::resume()
{
    Q_D(QIviMediaIndexerControl);
    QIviMediaIndexerControlBackendInterface *backend = d->indexerBackend();
    if (!backend) {
        qWarning("Can't resume without a connected backend");
        return;
    }

    backend->resume();
}

QIviMediaIndexerControl::QIviMediaIndexerControl(QIviMediaIndexerControlPrivate &dd, QObject *parent)
    : QIviAbstractFeature(dd, parent)
{
    Q_D(QIviMediaIndexerControl);
    d->init();
}

bool QIviMediaIndexerControl::acceptServiceObject(QIviServiceObject *serviceObject)
{
    return serviceObject->interfaces().contains(QIviStringMediaIndexerInterfaceName);
}

void QIviMediaIndexerControl::connectToServiceObject(QIviServiceObject *serviceObject)
{
    Q_UNUSED(serviceObject);

    Q_D(QIviMediaIndexerControl);

    QIviMediaIndexerControlBackendInterface *backend = d->indexerBackend();
    if (!backend)
        return;

    QObjectPrivate::connect(backend, &QIviMediaIndexerControlBackendInterface::progressChanged,
                            d, &QIviMediaIndexerControlPrivate::onProgressChanged);
    QObjectPrivate::connect(backend, &QIviMediaIndexerControlBackendInterface::stateChanged,
                            d, &QIviMediaIndexerControlPrivate::onStateChanged);

    backend->initialize();
}

void QIviMediaIndexerControl::disconnectFromServiceObject(QIviServiceObject *serviceObject)
{
    QIviMediaIndexerControlBackendInterface *backend = qobject_cast<QIviMediaIndexerControlBackendInterface*>(serviceObject->interfaceInstance(QIviStringMediaIndexerInterfaceName));

    if (backend)
        disconnect(backend, 0, this, 0);
}

void QIviMediaIndexerControl::clearServiceObject()
{
    Q_D(QIviMediaIndexerControl);
    d->clearToDefaults();
}

#include "moc_qivimediaindexercontrol.cpp"
