
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

#ifndef QIVIMEDIAPLAYERQTROADAPTER_H
#define QIVIMEDIAPLAYERQTROADAPTER_H

#include "mediaplayerbackend.h"
#include "rep_qivimediaplayer_source.h"

template <class ObjectType>
struct QIviMediaPlayerAddressWrapper: public QIviMediaPlayerSourceAPI<ObjectType> {
    QIviMediaPlayerAddressWrapper(ObjectType *object, const QString &name = QStringLiteral("QtIviMedia.QIviMediaPlayer"))
        : QIviMediaPlayerSourceAPI<ObjectType>(object, name)
    {}
};

class QIviMediaPlayerQtRoAdapter : public QIviMediaPlayerSource
{
public:
    QIviMediaPlayerQtRoAdapter(MediaPlayerBackend *parent);

public:
    QIviMediaPlayer::PlayMode playMode() const override;
    QIviMediaPlayer::PlayState playState() const override;
    qint64 position() const override;
    qint64 duration() const override;
    QVariant currentTrack() const override;
    int currentIndex() const override;
    int volume() const override;
    bool muted() const override;
    bool canReportCount() const override;
    void setPlayMode(QIviMediaPlayer::PlayMode playMode) override;
    void setPosition(qint64 position) override;
    void setCurrentIndex(int currentIndex) override;
    void setVolume(int volume) override;
    void setMuted(bool muted) override;

public Q_SLOTS:
    void play() override;
    void pause() override;
    void stop() override;
    void seek(qint64 offset) override;
    void next() override;
    void previous() override;
    void fetchData(const QUuid &identifier, int start, int count) override;
    void insert(int index, const QVariant &item) override;
    void remove(int index) override;
    void move(int currentIndex, int newIndex) override;

private:
    MediaPlayerBackend *m_backend;
    quint64 m_replyCounter;
};

#endif // QIVIMEDIAPLAYERQTROADAPTER_H
