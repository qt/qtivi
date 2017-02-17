/****************************************************************************
**
** Copyright (C) 2017 Pelagicore AG
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

#ifndef MEDIAPLAYERBACKEND_H
#define MEDIAPLAYERBACKEND_H

#include <QtIviMedia/QIviMediaPlayerBackendInterface>

#include <QSqlDatabase>

QT_FORWARD_DECLARE_CLASS(QMediaPlayer);
QT_FORWARD_DECLARE_CLASS(QMediaPlaylist);

class MediaPlayerBackend : public QIviMediaPlayerBackendInterface
{
    Q_OBJECT

public:
    enum OperationType {
        Select,
        Insert,
        Remove,
        Move,
        SetIndex
    };
    Q_ENUM(OperationType)

    MediaPlayerBackend(const QSqlDatabase &database, QObject *parent = Q_NULLPTR);

    virtual void initialize() Q_DECL_OVERRIDE;
    virtual void play() Q_DECL_OVERRIDE;
    virtual void pause() Q_DECL_OVERRIDE;
    virtual void stop() Q_DECL_OVERRIDE;
    virtual void seek(qint64 offset) Q_DECL_OVERRIDE;
    virtual void next() Q_DECL_OVERRIDE;
    virtual void previous() Q_DECL_OVERRIDE;
    virtual void setPlayMode(QIviMediaPlayer::PlayMode playMode) Q_DECL_OVERRIDE;
    virtual void setPosition(qint64 position) Q_DECL_OVERRIDE;
    virtual void setCurrentIndex(int index) Q_DECL_OVERRIDE;

    virtual bool canReportCount() Q_DECL_OVERRIDE;
    virtual void fetchData(int start, int count) Q_DECL_OVERRIDE;

    virtual void insert(int index, const QIviPlayableItem *item) Q_DECL_OVERRIDE;
    virtual void remove(int index) Q_DECL_OVERRIDE;
    virtual void move(int cur_index, int new_index) Q_DECL_OVERRIDE;

public Q_SLOTS:
    void doSqlOperation(MediaPlayerBackend::OperationType type, const QStringList &queries, int start, int count);
private:

    int m_count;
    int m_currentIndex;
    QMediaPlayer *m_player;
    QSqlDatabase m_db;
};

#endif // MEDIAPLAYERBACKEND_H
