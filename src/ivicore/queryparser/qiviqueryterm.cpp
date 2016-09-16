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

#include "qiviqueryterm.h"
#include "qiviqueryterm_p.h"

QIviConjunctionTermPrivate::QIviConjunctionTermPrivate()
    : m_conjunction(QIviConjunctionTerm::And)
{
}

QIviScopeTermPrivate::QIviScopeTermPrivate()
    : m_term(nullptr)
    , m_negated(false)
{
}

QIviFilterTermPrivate::QIviFilterTermPrivate()
    : m_negated(false)
{
}

QString QIviFilterTermPrivate::operatorToString() const
{
    switch (m_operator){
    case QIviFilterTerm::Equals: return QLatin1String("=");
    case QIviFilterTerm::EqualsCaseInsensitive: return QLatin1String("~=");
    case QIviFilterTerm::Unequals: return QLatin1String("!=");
    case QIviFilterTerm::GreaterThan: return QLatin1String(">");
    case QIviFilterTerm::GreaterEquals: return QLatin1String(">=");
    case QIviFilterTerm::LowerThan: return QLatin1String("<");
    case QIviFilterTerm::LowerEquals: return QLatin1String("<=");
    }

    return QLatin1String("unknown type");
}

QIviOrderTermPrivate::QIviOrderTermPrivate()
    : m_ascending(false)
{
}

QIviAbstractQueryTerm::~QIviAbstractQueryTerm()
{
}

QIviConjunctionTerm::QIviConjunctionTerm()
    : d_ptr(new QIviConjunctionTermPrivate)
{
}

QIviConjunctionTerm::~QIviConjunctionTerm()
{
    Q_D(QIviConjunctionTerm);
    qDeleteAll(d->m_terms);
    delete d_ptr;
}

QIviAbstractQueryTerm::Type QIviConjunctionTerm::type() const
{
    return QIviAbstractQueryTerm::ConjunctionTerm;
}

QString QIviConjunctionTerm::toString() const
{
    Q_D(const QIviConjunctionTerm);
    const QChar conjunction = d->m_conjunction == Or ? QLatin1Char('|') : QLatin1Char('&');

    QString string;
    if (!d->m_terms.empty()) {
        for (QIviAbstractQueryTerm *term : d->m_terms)
            string += term->toString() + QLatin1Char(' ') + conjunction + QLatin1Char(' ');
        string.chop(3); // remove trailing " & " or " | "
    }
    return string;
}

QIviConjunctionTerm::Conjunction QIviConjunctionTerm::conjunction() const
{
    Q_D(const QIviConjunctionTerm);
    return d->m_conjunction;
}

QList<QIviAbstractQueryTerm *> QIviConjunctionTerm::terms() const
{
    Q_D(const QIviConjunctionTerm);
    return d->m_terms;
}

QIviScopeTerm::QIviScopeTerm()
    : d_ptr(new QIviScopeTermPrivate)
{
}

QIviScopeTerm::~QIviScopeTerm()
{
    Q_D(QIviScopeTerm);
    delete d->m_term;
    delete d_ptr;
}

QIviAbstractQueryTerm::Type QIviScopeTerm::type() const
{
    return QIviAbstractQueryTerm::ScopeTerm;
}

QString QIviScopeTerm::toString() const
{
    Q_D(const QIviScopeTerm);
    QString string = QLatin1Literal("(") + d->m_term->toString() + QLatin1Literal(")");
    if (d->m_negated)
        string.prepend(QLatin1String("!"));

    return string;
}

bool QIviScopeTerm::isNegated() const
{
    Q_D(const QIviScopeTerm);
    return d->m_negated;
}

QIviAbstractQueryTerm *QIviScopeTerm::term() const
{
    Q_D(const QIviScopeTerm);
    return d->m_term;
}

QIviFilterTerm::QIviFilterTerm()
    : d_ptr(new QIviFilterTermPrivate)
{
}

QIviFilterTerm::~QIviFilterTerm()
{
    delete d_ptr;
}

QIviAbstractQueryTerm::Type QIviFilterTerm::type() const
{
    return QIviAbstractQueryTerm::FilterTerm;
}

QString QIviFilterTerm::toString() const
{
    Q_D(const QIviFilterTerm);
    QString string;

    string = d->m_property + d->operatorToString() + d->m_value.toString();

    if (d->m_negated)
        string.prepend(QLatin1String("!"));

    return string;
}

QIviFilterTerm::Operator QIviFilterTerm::operatorType() const
{
    Q_D(const QIviFilterTerm);
    return d->m_operator;
}

QVariant QIviFilterTerm::value() const
{
    Q_D(const QIviFilterTerm);
    return d->m_value;
}

QString QIviFilterTerm::propertyName() const
{
    Q_D(const QIviFilterTerm);
    return d->m_property;
}

bool QIviFilterTerm::isNegated() const
{
    Q_D(const QIviFilterTerm);
    return d->m_negated;
}

QIviOrderTerm::QIviOrderTerm()
    : d_ptr(new QIviOrderTermPrivate)
{
}

QIviOrderTerm::~QIviOrderTerm()
{
    delete d_ptr;
}

bool QIviOrderTerm::isAscending() const
{
    Q_D(const QIviOrderTerm);
    return d->m_ascending;
}

QString QIviOrderTerm::propertyName() const
{
    Q_D(const QIviOrderTerm);
    return d->m_propertyName;
}
