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

#ifndef QIVIPLAYABLEITEM_H
#define QIVIPLAYABLEITEM_H

#include <QtIviMedia/qtivimediaglobal.h>
#include <QtIviCore/QIviSearchAndBrowseModel>
#include <QtCore/QUrl>

QT_BEGIN_NAMESPACE

class Q_QTIVIMEDIA_EXPORT QIviPlayableItem : public QIviSearchAndBrowseListItem
{
    Q_GADGET

    Q_PROPERTY(QUrl url READ url)

public:
    QIviPlayableItem();
    virtual ~QIviPlayableItem();
    virtual QUrl url() const { return QUrl(); }
};

class Q_QTIVIMEDIA_EXPORT QIviAudioTrackItem : public QIviPlayableItem
{
    Q_GADGET

    Q_PROPERTY(QString title READ title)
    Q_PROPERTY(QString artist READ artist)
    Q_PROPERTY(QString album READ album)
    Q_PROPERTY(QString genre READ genre)
    Q_PROPERTY(int year READ year) //FIXME What type should we use here ?
    Q_PROPERTY(int trackNumber READ trackNumber)
    Q_PROPERTY(int duration READ duration)
    Q_PROPERTY(QString coverArt READ coverArt) //FIXME How to best serve this ?
    Q_PROPERTY(int rating READ rating)

public:
    QIviAudioTrackItem();
    virtual ~QIviAudioTrackItem();

    virtual QString title() { return QString(); }
    virtual QString artist() { return QString(); }
    virtual QString album() { return QString(); }
    virtual QString genre() { return QString(); }
    virtual int year() { return -1; }
    virtual int trackNumber() { return -1; }
    virtual int duration() { return -1; }
    virtual QString coverArt() { return QString(); }
    virtual int rating() { return -1; }
};

QT_END_NAMESPACE

Q_DECLARE_METATYPE(QIviPlayableItem)
Q_DECLARE_METATYPE(QIviAudioTrackItem)

#endif // QIVIPLAYABLEITEM_H
