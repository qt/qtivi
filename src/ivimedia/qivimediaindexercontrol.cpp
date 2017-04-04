/****************************************************************************
**
** Copyright (C) 2017 Pelagicore AG
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

QT_BEGIN_NAMESPACE

QIviMediaIndexerControlPrivate::QIviMediaIndexerControlPrivate(const QString &interface, QIviMediaIndexerControl *parent)
    : QIviAbstractFeaturePrivate(interface, parent)
    , q_ptr(parent)
    , m_progress(0)
    , m_state(QIviMediaIndexerControl::Idle)
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
        return qobject_cast<QIviMediaIndexerControlBackendInterface*>(so->interfaceInstance(QLatin1String(QIviMediaIndexer_iid)));

    return nullptr;
}

/*!
    \class QIviMediaIndexerControl
    \inmodule QtIviMedia
    \brief Provides a interface to control the media indexer

    The QIviMediaIndexerControl controls the media indexer. It provides a way to temporarily pause the indexing
    and resume it, as well as to inquire about the current state and progress of the indexing operation.

    The QIviMediaIndexerControl expects a single backend to be available. It is recommended to use it
    with \l {QIviAbstractFeature::}{discoveryMode} set to \l QIviAbstractFeature::AutoDiscovery.
*/

/*!
    \qmltype MediaIndexerControl
    \instantiates QIviMediaIndexerControl
    \inqmlmodule QtIvi.Media
    \inherits AbstractFeature
    \brief Provides a interface to control the media indexer

    The MediaIndexerControl controls the media indexer. It provides a way to temporarily pause the indexing
    and resume it, as well as to inquire about the current state and progress of the indexing operation.

    The MediaIndexerControl expects a single backend to be available. It is recommended to use it
    with \l {AbstractFeature::}{discoveryMode} set to AbstractFeature.AutoDiscovery.
*/

/*!
    \enum QIviMediaIndexerControl::State
    \value Idle
           The indexer is currently idle and is waiting for new files to be indexed.
    \value Active
           The indexer is currently busy with indexing.
    \value Paused
           The indexer is paused, due to a call to pause().
    \value Error
           An error has occurred during the indexing operation.
*/

/*!
    Constructs a QIviMediaIndexerControl.

    The \a parent argument is passed on to the \l QIviAbstractFeature base class.
*/
QIviMediaIndexerControl::QIviMediaIndexerControl(QObject *parent)
    : QIviAbstractFeature(*new QIviMediaIndexerControlPrivate(QLatin1String(QIviMediaIndexer_iid), this), parent)
{
}

/*!
    \qmlproperty real MediaIndexerControl::progress
    \brief Holds the progress of the indexing operation.

    The value is between \e 0 and \e 1.
*/
/*!
    \property QIviMediaIndexerControl::progress
    \brief Holds the progress of the indexing operation.

    The value is between \e 0 and \e 1.
*/
qreal QIviMediaIndexerControl::progress() const
{
    Q_D(const QIviMediaIndexerControl);
    return d->m_progress;
}

/*!
    \qmlproperty enumeration MediaIndexerControl::state
    \brief Holds the current state of the indexer.

    It can be one of the following values:
    \value Idle
           The indexer is currently idle and is waiting for new files to be indexed.
    \value Active
           The indexer is currently busy with indexing.
    \value Paused
           The indexer is paused, due to a call to pause().
    \value Error
           An error has occurred during the indexing operation.
*/
/*!
    \property QIviMediaIndexerControl::state
    \brief Holds the current state of the indexer.
*/
QIviMediaIndexerControl::State QIviMediaIndexerControl::state() const
{
    Q_D(const QIviMediaIndexerControl);
    return d->m_state;
}

/*!
    \qmlmethod MediaIndexerControl::pause()

    Pauses the currently ongoing indexing operation.

    \sa resume() state
*/

/*!
    \fn void QIviMediaIndexerControl::pause()

    Pauses the currently ongoing indexing operation.

    \sa resume() state
*/
void QIviMediaIndexerControl::pause()
{
    Q_IVI_BACKEND(QIviMediaIndexerControl, d->indexerBackend(), "Can't pause without a connected backend");

    backend->pause();
}

/*!
    \qmlmethod MediaIndexerControl::resume()

    Resumes from the Paused state and resumes the indexing operation.

    \sa pause() state
*/

/*!
    \fn void QIviMediaIndexerControl::resume()

    Resumes from the Paused state and resumes the indexing operation.

    \sa pause() state
*/
void QIviMediaIndexerControl::resume()
{
    Q_IVI_BACKEND(QIviMediaIndexerControl, d->indexerBackend(), "Can't resume without a connected backend");

    backend->resume();
}

/*!
    \internal
*/
QIviMediaIndexerControl::QIviMediaIndexerControl(QIviMediaIndexerControlPrivate &dd, QObject *parent)
    : QIviAbstractFeature(dd, parent)
{
}

/*!
    \reimp
*/
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

    QIviAbstractFeature::connectToServiceObject(serviceObject);

    backend->initialize();
}

/*!
    \reimp
*/
void QIviMediaIndexerControl::clearServiceObject()
{
    Q_D(QIviMediaIndexerControl);
    d->clearToDefaults();
}

QT_END_NAMESPACE

#include "moc_qivimediaindexercontrol.cpp"
