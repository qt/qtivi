/****************************************************************************
**
** Copyright (C) 2019 Luxoft Sweden AB
** Copyright (C) 2018 Pelagicore AG
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

#ifndef QUERYTERM_H
#define QUERYTERM_H

#include <QObject>
#include <QSharedData>
#include <QVariant>

#include <QtIviCore/qtiviglobal.h>

QT_BEGIN_NAMESPACE

class Q_QTIVICORE_EXPORT QIviAbstractQueryTerm
{
public:
    enum Type {
        FilterTerm,
        ConjunctionTerm,
        ScopeTerm
    };

    virtual ~QIviAbstractQueryTerm();

    virtual QString toString() const = 0;
    virtual QIviAbstractQueryTerm::Type type() const = 0;
};

class QIviConjunctionTermPrivate;
class Q_QTIVICORE_EXPORT QIviConjunctionTerm : public QIviAbstractQueryTerm
{
    Q_GADGET
public:
    enum Conjunction {
        And,
        Or
    };
    Q_ENUM(Conjunction)

    explicit QIviConjunctionTerm();
    ~QIviConjunctionTerm() override;

    QIviAbstractQueryTerm::Type type() const override;
    QString toString() const override;
    Conjunction conjunction() const;
    QList<QIviAbstractQueryTerm*> terms() const;

private:
    Q_DISABLE_COPY(QIviConjunctionTerm)
    QIviConjunctionTermPrivate * d_ptr;
    Q_DECLARE_PRIVATE(QIviConjunctionTerm)
    friend class QIviQueryParser;
    friend Q_QTIVICORE_EXPORT QDataStream &operator>>(QDataStream &in, QIviAbstractQueryTerm** var);
};

class QIviScopeTermPrivate;
class Q_QTIVICORE_EXPORT QIviScopeTerm : public QIviAbstractQueryTerm
{
    Q_GADGET
public:

    explicit QIviScopeTerm();
    ~QIviScopeTerm() override;

    QIviAbstractQueryTerm::Type type() const override;
    QString toString() const override;
    bool isNegated() const;
    QIviAbstractQueryTerm* term() const;

private:
    Q_DISABLE_COPY(QIviScopeTerm)
    QIviScopeTermPrivate * d_ptr;
    Q_DECLARE_PRIVATE(QIviScopeTerm)
    friend class QIviQueryParser;
    friend Q_QTIVICORE_EXPORT QDataStream &operator>>(QDataStream &in, QIviAbstractQueryTerm** var);
};

class QIviFilterTermPrivate;
class Q_QTIVICORE_EXPORT QIviFilterTerm : public QIviAbstractQueryTerm
{
    Q_GADGET
public:
    enum Operator {
        Equals,
        EqualsCaseInsensitive,
        Unequals,
        GreaterThan,
        GreaterEquals,
        LowerThan,
        LowerEquals
    };
    Q_ENUM(Operator)

    explicit QIviFilterTerm();
    ~QIviFilterTerm() override;

    QIviAbstractQueryTerm::Type type() const override;
    QString toString() const override;
    Operator operatorType() const;
    QVariant value() const;
    QString propertyName() const;
    bool isNegated() const;

private:
    Q_DISABLE_COPY(QIviFilterTerm)
    QIviFilterTermPrivate * d_ptr;
    Q_DECLARE_PRIVATE(QIviFilterTerm)
    friend class QIviQueryParser;
    friend Q_QTIVICORE_EXPORT QDataStream &operator>>(QDataStream &in, QIviAbstractQueryTerm** var);
};

class QIviOrderTermPrivate;
class Q_QTIVICORE_EXPORT QIviOrderTerm
{
public:
    QIviOrderTerm();
    QIviOrderTerm(const QIviOrderTerm &other);
    virtual ~QIviOrderTerm();
    QIviOrderTerm& operator =(const QIviOrderTerm &other);

    bool isAscending() const;
    QString propertyName() const;

private:
    QSharedDataPointer<QIviOrderTermPrivate> d;
    friend class QIviQueryParser;
    friend Q_QTIVICORE_EXPORT QDataStream &operator>>(QDataStream &in, QIviOrderTerm &var);
};

Q_DECLARE_TYPEINFO(QIviOrderTerm, Q_MOVABLE_TYPE);

Q_QTIVICORE_EXPORT QDataStream &operator<<(QDataStream &out, QIviConjunctionTerm::Conjunction var);
Q_QTIVICORE_EXPORT QDataStream &operator>>(QDataStream &in, QIviConjunctionTerm::Conjunction &var);
Q_QTIVICORE_EXPORT QDataStream &operator<<(QDataStream &out, QIviFilterTerm::Operator var);
Q_QTIVICORE_EXPORT QDataStream &operator>>(QDataStream &in, QIviFilterTerm::Operator &var);
Q_QTIVICORE_EXPORT QDataStream &operator<<(QDataStream &out, QIviAbstractQueryTerm *var);
Q_QTIVICORE_EXPORT QDataStream &operator>>(QDataStream &in, QIviAbstractQueryTerm **var);
Q_QTIVICORE_EXPORT QDataStream &operator<<(QDataStream &out, const QIviOrderTerm &var);
Q_QTIVICORE_EXPORT QDataStream &operator>>(QDataStream &in, QIviOrderTerm &var);

QT_END_NAMESPACE

Q_DECLARE_METATYPE(QIviOrderTerm)

#endif // QUERYTERM_H
