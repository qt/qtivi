/****************************************************************************
**
** Copyright (C) 2016 Pelagicore AG
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

#include "qivisearchandbrowsemodelitem.h"

QT_BEGIN_NAMESPACE

class QIviSearchAndBrowseModelItemPrivate : public QSharedData
{
public:
    QIviSearchAndBrowseModelItemPrivate()
    {}

    QIviSearchAndBrowseModelItemPrivate(const QIviSearchAndBrowseModelItemPrivate &other)
        : QSharedData(other)
        , m_id(other.m_id)
    {}

    QString m_id;
};

QT_END_NAMESPACE

/*!
    \class QIviSearchAndBrowseModelItem
    \inmodule QtIviCore
    \brief The QIviSearchAndBrowseModelItem is the base class of a row in the QIviSearchAndBrowseModel model.
*/

/*!
    \qmltype SearchAndBrowseModelItem
    \qmlabstract
    \instantiates QIviSearchAndBrowseModelItem
    \inqmlmodule QtIvi
    \brief The SearchAndBrowseModelItem is the base class of a row in the SearchAndBrowseModel model.

    \note This item is not creatable from QML.
*/

/*!
    \qmlproperty string SearchAndBrowseModelItem::id
    A unique identifier, which can be used to identify this item.

    This is mainly used by the backend to implement filtering or browsing.
*/

/*!
    \property QIviSearchAndBrowseModelItem::id
    A unique identifier, which can be used to identify this item.

    This is mainly used by the backend to implement filtering or browsing.
*/

/*!
    \qmlproperty string SearchAndBrowseModelItem::name
    The name of the item. E.g. The name of a contact in a addressbook, or the artist-name in a list of artists.
*/

/*!
    \property QIviSearchAndBrowseModelItem::name
    The name of the item. E.g. The name of a contact in a addressbook, or the artist-name in a list of artists.
*/

/*!
    \qmlproperty string SearchAndBrowseModelItem::type
    The type of the item. E.g. "artist", "track", "contact".
*/

/*!
    \property QIviSearchAndBrowseModelItem::type
    The type of the item. E.g. "artist", "track", "contact".
*/


QIviSearchAndBrowseModelItem::QIviSearchAndBrowseModelItem()
    : d(new QIviSearchAndBrowseModelItemPrivate)
{

}

QIviSearchAndBrowseModelItem::QIviSearchAndBrowseModelItem(const QIviSearchAndBrowseModelItem &rhs)
    : d(rhs.d)
{

}

QIviSearchAndBrowseModelItem &QIviSearchAndBrowseModelItem::operator=(const QIviSearchAndBrowseModelItem &rhs)
{
    if (this != &rhs)
        d.operator=(rhs.d);
    return *this;
}

QIviSearchAndBrowseModelItem::~QIviSearchAndBrowseModelItem()
{
}

QString QIviSearchAndBrowseModelItem::id() const
{
    return d->m_id;
}

void QIviSearchAndBrowseModelItem::setId(const QString &id)
{
    d->m_id = id;
}

QString QIviSearchAndBrowseModelItem::name() const
{
    return QString();
}

QString QIviSearchAndBrowseModelItem::type() const
{
    return QString();
}

/*!
    Returns \e true if this item is equal to \a other; otherwise returns false.

    \sa operator!=()
*/
bool QIviSearchAndBrowseModelItem::operator==(const QIviSearchAndBrowseModelItem &other)
{
    return (d->m_id == other.d->m_id);
}

/*!
    \fn bool QIviSearchAndBrowseModelItem::operator!=(const QIviSearchAndBrowseModelItem &other)

    Returns \e true if this item is not equal to \a other; otherwise returns false.

    \sa operator==()
*/
