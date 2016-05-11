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

#ifndef QUERYTERM_H
#define QUERYTERM_H

#include <QObject>
#include <QVariant>

#include <QtIVICore/qtiviglobal.h>

QT_BEGIN_NAMESPACE

class Q_QTIVICORE_EXPORT QtIVIAbstractQueryTerm
{
public:
    enum Type {
        FilterTerm,
        ConjunctionTerm,
        ScopeTerm
    };

    virtual ~QtIVIAbstractQueryTerm();

    virtual QString toString() const = 0;
    virtual QtIVIAbstractQueryTerm::Type type() const = 0;
};

class QtIVIConjunctionTermPrivate;
class Q_QTIVICORE_EXPORT QtIVIConjunctionTerm : public QtIVIAbstractQueryTerm
{
public:
    enum Conjunction {
        And,
        Or
    };

    QtIVIConjunctionTerm();
    virtual ~QtIVIConjunctionTerm();

    QtIVIAbstractQueryTerm::Type type() const;
    QString toString() const;
    Conjunction conjunction() const;
    QList<QtIVIAbstractQueryTerm*> terms() const;

private:
    QtIVIConjunctionTermPrivate * d_ptr;
    Q_DECLARE_PRIVATE(QtIVIConjunctionTerm)
    friend class QtIVIQueryParser;
};

class QtIVIScopeTermPrivate;
class Q_QTIVICORE_EXPORT QtIVIScopeTerm : public QtIVIAbstractQueryTerm
{
public:

    explicit QtIVIScopeTerm();
    virtual ~QtIVIScopeTerm();

    QtIVIAbstractQueryTerm::Type type() const;
    QString toString() const;
    bool isNegated() const;
    QtIVIAbstractQueryTerm* term() const;

private:
    QtIVIScopeTermPrivate * d_ptr;
    Q_DECLARE_PRIVATE(QtIVIScopeTerm)
    friend class QtIVIQueryParser;
};

class QtIVIFilterTermPrivate;
class Q_QTIVICORE_EXPORT QtIVIFilterTerm : public QtIVIAbstractQueryTerm
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

    explicit QtIVIFilterTerm();
    virtual ~QtIVIFilterTerm();

    QtIVIAbstractQueryTerm::Type type() const;
    QString toString() const;
    Operator operatorType() const;
    QVariant value() const;
    QString propertyName() const;
    bool isNegated() const;

private:
    QtIVIFilterTermPrivate * d_ptr;
    Q_DECLARE_PRIVATE(QtIVIFilterTerm)
    friend class QtIVIQueryParser;
};

class QtIVIOrderTermPrivate;
class Q_QTIVICORE_EXPORT QtIVIOrderTerm
{
public:
    QtIVIOrderTerm();
    virtual ~QtIVIOrderTerm();

    bool isAscending() const;
    QString propertyName() const;

private:
    QtIVIOrderTermPrivate * d_ptr;
    Q_DECLARE_PRIVATE(QtIVIOrderTerm)
    friend class QtIVIQueryParser;
};

QT_END_NAMESPACE

#endif // QUERYTERM_H
