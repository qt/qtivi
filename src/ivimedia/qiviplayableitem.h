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

#ifndef QIVIPLAYABLEITEM_H
#define QIVIPLAYABLEITEM_H

#include <QtIviMedia/qtivimediaglobal.h>
#include <QtIviCore/QIviSearchAndBrowseModel>
#include <QtIviCore/QIviStandardItem>
#include <QtCore/QUrl>

QT_BEGIN_NAMESPACE

class QIviPlayableItemPrivate;
class QIviAudioTrackItemPrivate;

class Q_QTIVIMEDIA_EXPORT QIviPlayableItem : public QIviStandardItem
{
    Q_GADGET

    Q_PROPERTY(QUrl url READ url WRITE setUrl)

public:
    explicit QIviPlayableItem();
    explicit QIviPlayableItem(const QIviPlayableItem &);
    QIviPlayableItem &operator=(const QIviPlayableItem &);
    ~QIviPlayableItem();

    virtual QUrl url() const;
    virtual void setUrl(const QUrl &url);
    virtual QString type() const override;

    bool operator==(const QIviPlayableItem &other) const;
    inline bool operator!=(const QIviPlayableItem &other) const { return !(*this == other); }

private:
    QSharedDataPointer<QIviPlayableItemPrivate> d;
};
Q_DECLARE_TYPEINFO(QIviPlayableItem, Q_MOVABLE_TYPE);

class Q_QTIVIMEDIA_EXPORT QIviAudioTrackItem : public QIviPlayableItem
{
    Q_GADGET

    Q_PROPERTY(QString title READ title WRITE setTitle)
    Q_PROPERTY(QString artist READ artist WRITE setArtist)
    Q_PROPERTY(QString album READ album WRITE setAlbum)
    Q_PROPERTY(QString genre READ genre WRITE setGenre)
    Q_PROPERTY(int year READ year WRITE setYear)
    Q_PROPERTY(int trackNumber READ trackNumber WRITE setTrackNumber)
    Q_PROPERTY(qint64 duration READ duration WRITE setDuration)
    Q_PROPERTY(QUrl coverArtUrl READ coverArtUrl WRITE setCoverArtUrl)
    Q_PROPERTY(int rating READ rating WRITE setRating)

public:
    QIviAudioTrackItem();
    QIviAudioTrackItem(const QIviAudioTrackItem &);
    QIviAudioTrackItem &operator=(const QIviAudioTrackItem &);
    ~QIviAudioTrackItem();

    virtual QString title() const;
    virtual void setTitle(const QString &title);
    virtual QString artist() const;
    virtual void setArtist(const QString &artist);
    virtual QString album() const;
    virtual void setAlbum(const QString &album);
    virtual QString genre() const;
    virtual void setGenre(const QString &genre);
    virtual int year() const;
    virtual void setYear(int year);
    virtual int trackNumber() const;
    virtual void setTrackNumber(int trackNumber);
    virtual qint64 duration() const;
    virtual void setDuration(qint64 duration);
    virtual QUrl coverArtUrl() const;
    virtual void setCoverArtUrl(const QUrl &url);
    virtual int rating() const;
    virtual void setRating(int rating);
    virtual QString name() const override;
    virtual QString type() const override;

    bool operator==(const QIviAudioTrackItem &other) const;
    inline bool operator!=(const QIviAudioTrackItem &other) const { return !(*this == other); }

private:
    QSharedDataPointer<QIviAudioTrackItemPrivate> d;
};
Q_DECLARE_TYPEINFO(QIviAudioTrackItem, Q_MOVABLE_TYPE);

Q_QTIVIMEDIA_EXPORT QDataStream &operator<<(QDataStream &stream, const QIviAudioTrackItem &obj);
Q_QTIVIMEDIA_EXPORT QDataStream &operator>>(QDataStream &stream, QIviAudioTrackItem &obj);

QT_END_NAMESPACE

Q_DECLARE_METATYPE(QIviPlayableItem)
Q_DECLARE_METATYPE(QIviAudioTrackItem)

#endif // QIVIPLAYABLEITEM_H
