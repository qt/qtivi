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

#ifndef QUERYTERM_H
#define QUERYTERM_H

#include <QObject>
#include <QVariant>
#include <QSharedData>

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
public:
    enum Conjunction {
        And,
        Or
    };

    QIviConjunctionTerm();
    virtual ~QIviConjunctionTerm();

    QIviAbstractQueryTerm::Type type() const Q_DECL_OVERRIDE;
    QString toString() const Q_DECL_OVERRIDE;
    Conjunction conjunction() const;
    QList<QIviAbstractQueryTerm*> terms() const;

private:
    Q_DISABLE_COPY(QIviConjunctionTerm)
    QIviConjunctionTermPrivate * d_ptr;
    Q_DECLARE_PRIVATE(QIviConjunctionTerm)
    friend class QIviQueryParser;
};

class QIviScopeTermPrivate;
class Q_QTIVICORE_EXPORT QIviScopeTerm : public QIviAbstractQueryTerm
{
public:

    explicit QIviScopeTerm();
    virtual ~QIviScopeTerm();

    QIviAbstractQueryTerm::Type type() const Q_DECL_OVERRIDE;
    QString toString() const Q_DECL_OVERRIDE;
    bool isNegated() const;
    QIviAbstractQueryTerm* term() const;

private:
    Q_DISABLE_COPY(QIviScopeTerm)
    QIviScopeTermPrivate * d_ptr;
    Q_DECLARE_PRIVATE(QIviScopeTerm)
    friend class QIviQueryParser;
};

class QIviFilterTermPrivate;
class Q_QTIVICORE_EXPORT QIviFilterTerm : public QIviAbstractQueryTerm
{
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

    explicit QIviFilterTerm();
    virtual ~QIviFilterTerm();

    QIviAbstractQueryTerm::Type type() const Q_DECL_OVERRIDE;
    QString toString() const Q_DECL_OVERRIDE;
    Operator operatorType() const;
    QVariant value() const;
    QString propertyName() const;
    bool isNegated() const;

private:
    Q_DISABLE_COPY(QIviFilterTerm)
    QIviFilterTermPrivate * d_ptr;
    Q_DECLARE_PRIVATE(QIviFilterTerm)
    friend class QIviQueryParser;
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
};

Q_DECLARE_TYPEINFO(QIviOrderTerm, Q_MOVABLE_TYPE);

QT_END_NAMESPACE

#endif // QUERYTERM_H
