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

#include "qtiviqueryterm.h"
#include "qtiviqueryterm_p.h"

QtIVIConjunctionTermPrivate::QtIVIConjunctionTermPrivate()
    : m_conjunction(QtIVIConjunctionTerm::And)
{
}

QtIVIScopeTermPrivate::QtIVIScopeTermPrivate()
    : m_term(nullptr)
    , m_negated(false)
{
}

QtIVIFilterTermPrivate::QtIVIFilterTermPrivate()
    : m_negated(false)
{
}

QString QtIVIFilterTermPrivate::operatorToString() const
{
    switch (m_operator){
    case QtIVIFilterTerm::Equals: return QLatin1String("=");
    case QtIVIFilterTerm::EqualsCaseInsensitive: return QLatin1String("~=");
    case QtIVIFilterTerm::Unequals: return QLatin1String("!=");
    case QtIVIFilterTerm::GreaterThan: return QLatin1String(">");
    case QtIVIFilterTerm::GreaterEquals: return QLatin1String(">=");
    case QtIVIFilterTerm::LowerThan: return QLatin1String("<");
    case QtIVIFilterTerm::LowerEquals: return QLatin1String("<=");
    }

    return QLatin1String("unknown type");
}

QtIVIOrderTermPrivate::QtIVIOrderTermPrivate()
    : m_ascending(false)
{
}

QtIVIAbstractQueryTerm::~QtIVIAbstractQueryTerm()
{
}

QtIVIConjunctionTerm::QtIVIConjunctionTerm()
    : d_ptr(new QtIVIConjunctionTermPrivate)
{
}

QtIVIConjunctionTerm::~QtIVIConjunctionTerm()
{
    Q_D(QtIVIConjunctionTerm);
    qDeleteAll(d->m_terms);
    delete d_ptr;
}

QtIVIAbstractQueryTerm::Type QtIVIConjunctionTerm::type() const
{
    return QtIVIAbstractQueryTerm::ConjunctionTerm;
}

QString QtIVIConjunctionTerm::toString() const
{
    Q_D(const QtIVIConjunctionTerm);
    QString conjunction = QLatin1String("&");
    if (d->m_conjunction == Or)
        conjunction = QLatin1String("|");

    QString string;
    QListIterator<QtIVIAbstractQueryTerm*> it(d->m_terms);
    while (it.hasNext()) {
        string += it.next()->toString();
        if (it.hasNext())
            string += QLatin1Literal(" ") + conjunction + QLatin1Literal(" ");
    }

    return string;
}

QtIVIConjunctionTerm::Conjunction QtIVIConjunctionTerm::conjunction() const
{
    Q_D(const QtIVIConjunctionTerm);
    return d->m_conjunction;
}

QList<QtIVIAbstractQueryTerm *> QtIVIConjunctionTerm::terms() const
{
    Q_D(const QtIVIConjunctionTerm);
    return d->m_terms;
}

QtIVIScopeTerm::QtIVIScopeTerm()
    : d_ptr(new QtIVIScopeTermPrivate)
{
}

QtIVIScopeTerm::~QtIVIScopeTerm()
{
    Q_D(QtIVIScopeTerm);
    delete d->m_term;
    delete d_ptr;
}

QtIVIAbstractQueryTerm::Type QtIVIScopeTerm::type() const
{
    return QtIVIAbstractQueryTerm::ScopeTerm;
}

QString QtIVIScopeTerm::toString() const
{
    Q_D(const QtIVIScopeTerm);
    QString string = QLatin1Literal("(") + d->m_term->toString() + QLatin1Literal(")");
    if (d->m_negated)
        string.prepend(QLatin1String("!"));

    return string;
}

bool QtIVIScopeTerm::isNegated() const
{
    Q_D(const QtIVIScopeTerm);
    return d->m_negated;
}

QtIVIAbstractQueryTerm *QtIVIScopeTerm::term() const
{
    Q_D(const QtIVIScopeTerm);
    return d->m_term;
}

QtIVIFilterTerm::QtIVIFilterTerm()
    : d_ptr(new QtIVIFilterTermPrivate)
{
}

QtIVIFilterTerm::~QtIVIFilterTerm()
{
    delete d_ptr;
}

QtIVIAbstractQueryTerm::Type QtIVIFilterTerm::type() const
{
    return QtIVIAbstractQueryTerm::FilterTerm;
}

QString QtIVIFilterTerm::toString() const
{
    Q_D(const QtIVIFilterTerm);
    QString string;

    string = d->m_property + d->operatorToString() + d->m_value.toString();

    if (d->m_negated)
        string.prepend(QLatin1String("!"));

    return string;
}

QtIVIFilterTerm::Operator QtIVIFilterTerm::operatorType() const
{
    Q_D(const QtIVIFilterTerm);
    return d->m_operator;
}

QVariant QtIVIFilterTerm::value() const
{
    Q_D(const QtIVIFilterTerm);
    return d->m_value;
}

QString QtIVIFilterTerm::propertyName() const
{
    Q_D(const QtIVIFilterTerm);
    return d->m_property;
}

bool QtIVIFilterTerm::isNegated() const
{
    Q_D(const QtIVIFilterTerm);
    return d->m_negated;
}

QtIVIOrderTerm::QtIVIOrderTerm()
    : d_ptr(new QtIVIOrderTermPrivate)
{
}

QtIVIOrderTerm::~QtIVIOrderTerm()
{
    delete d_ptr;
}

bool QtIVIOrderTerm::isAscending() const
{
    Q_D(const QtIVIOrderTerm);
    return d->m_ascending;
}

QString QtIVIOrderTerm::propertyName() const
{
    Q_D(const QtIVIOrderTerm);
    return d->m_propertyName;
}
