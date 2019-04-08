/****************************************************************************
**
** Copyright (C) 2019 Luxoft Sweden AB
** Copyright (C) 2018 Pelagicore AG
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

#ifndef QIVIMEDIAINDEXERCONTROL_H
#define QIVIMEDIAINDEXERCONTROL_H

#include <QtIviCore/QIviAbstractFeature>
#include <QtIviMedia/qtivimediaglobal.h>

QT_BEGIN_NAMESPACE

class QIviMediaIndexerControlPrivate;

class Q_QTIVIMEDIA_EXPORT QIviMediaIndexerControl : public QIviAbstractFeature
{
    Q_OBJECT

    Q_PROPERTY(qreal progress READ progress NOTIFY progressChanged)
    Q_PROPERTY(QIviMediaIndexerControl::State state READ state NOTIFY stateChanged)
public:

    enum State {
        Idle,
        Active,
        Paused,
        Error
    };
    Q_ENUM(State)

    explicit QIviMediaIndexerControl(QObject *parent = nullptr);

    qreal progress() const;
    QIviMediaIndexerControl::State state() const;

public Q_SLOTS:
    void pause();
    void resume();

Q_SIGNALS:
    void progressChanged(qreal progress);
    void stateChanged(QIviMediaIndexerControl::State state);

protected:
    QIviMediaIndexerControl(QIviMediaIndexerControlPrivate &dd, QObject *parent = nullptr);

    virtual void connectToServiceObject(QIviServiceObject *serviceObject) override;
    virtual void clearServiceObject() override;

private:
    Q_DECLARE_PRIVATE(QIviMediaIndexerControl)
    Q_PRIVATE_SLOT(d_func(), void onProgressChanged(qreal progress))
    Q_PRIVATE_SLOT(d_func(), void onStateChanged(QIviMediaIndexerControl::State state))
};

Q_QTIVIMEDIA_EXPORT QDataStream &operator<<(QDataStream &out, QIviMediaIndexerControl::State var);
Q_QTIVIMEDIA_EXPORT QDataStream &operator>>(QDataStream &in, QIviMediaIndexerControl::State &var);

QT_END_NAMESPACE

#endif // QIVIMEDIAINDEXERCONTROL_H
