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

#include "qiviqueryterm.h"
#include "qiviqueryterm_p.h"

#include <QDataStream>
#include <QMetaEnum>
#include <QtDebug>

QT_BEGIN_NAMESPACE

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
    : m_operator(QIviFilterTerm::Equals)
    , m_negated(false)
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

QIviOrderTermPrivate::QIviOrderTermPrivate(const QIviOrderTermPrivate &other)
    : QSharedData(other)
    , m_ascending(other.m_ascending)
    , m_propertyName(other.m_propertyName)
{

}

/*!
    \class QIviAbstractQueryTerm
    \inmodule QtIviCore
    \brief The base class of all query terms.

    Following terms are supported:
    \annotatedlist qtivi_queryterms

    See \l {Qt IVI Query Language} for how it can be used.
*/

/*!
    \enum QIviAbstractQueryTerm::Type
    \value FilterTerm
           A filter term stands for a filter which checks a specific identifier against a given value.
    \value ConjunctionTerm
           A conjunction term can combine multiple terms together, either by a OR or an AND conjunction.
    \value ScopeTerm
           A scope term is used to group terms together, e.g. to apply a negation to a group.
*/

/*!
    \fn QIviAbstractQueryTerm::Type QIviAbstractQueryTerm::type() const

    Returns the type of this query term.
*/

/*!
    \fn QString QIviAbstractQueryTerm::toString() const

    Returns a string representation of the query.
*/

QIviAbstractQueryTerm::~QIviAbstractQueryTerm()
{
}

/*!
    \class QIviConjunctionTerm
    \inmodule QtIviCore
    \ingroup qtivi_queryterms
    \brief The QIviConjunctionTerm is the representation of a conjunction between two query terms.
*/

/*!
    \enum QIviConjunctionTerm::Conjunction
    \value And
           The AND conjunction combines the filters to only match when all supplied filters are \c true.
    \value Or
           The OR conjunction combines the filters to match when one of the supplied filters are \c true.
*/

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

/*!
    \reimp
*/
QIviAbstractQueryTerm::Type QIviConjunctionTerm::type() const
{
    return QIviAbstractQueryTerm::ConjunctionTerm;
}

/*!
    \reimp
*/
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

/*!
    Returns the type of the conjunction.
*/
QIviConjunctionTerm::Conjunction QIviConjunctionTerm::conjunction() const
{
    Q_D(const QIviConjunctionTerm);
    return d->m_conjunction;
}

/*!
    Returns the terms which are conjuncted together.
*/
QList<QIviAbstractQueryTerm *> QIviConjunctionTerm::terms() const
{
    Q_D(const QIviConjunctionTerm);
    return d->m_terms;
}

/*!
    \class QIviScopeTerm
    \inmodule QtIviCore
    \ingroup qtivi_queryterms
    \brief The QIviScopeTerm is the representation of a scope which can hold another term.
*/
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

/*!
    \reimp
*/
QIviAbstractQueryTerm::Type QIviScopeTerm::type() const
{
    return QIviAbstractQueryTerm::ScopeTerm;
}

/*!
    \reimp
*/
QString QIviScopeTerm::toString() const
{
    Q_D(const QIviScopeTerm);
    QString string = QLatin1String("(") + d->m_term->toString() + QLatin1String(")");
    if (d->m_negated)
        string.prepend(QLatin1String("!"));

    return string;
}

/*!
    Returns \c true when this term is negated, otherwise \c false
*/
bool QIviScopeTerm::isNegated() const
{
    Q_D(const QIviScopeTerm);
    return d->m_negated;
}

/*!
    Returns the term which is inside this scope.

    This term can be a conjunction term if there are multiple terms inside.
*/
QIviAbstractQueryTerm *QIviScopeTerm::term() const
{
    Q_D(const QIviScopeTerm);
    return d->m_term;
}

/*!
    \class QIviFilterTerm
    \inmodule QtIviCore
    \ingroup qtivi_queryterms
    \brief The QIviFilterTerm is the representation of a filter.

    The filter is either in the form:

    \code
    identifier operator value
    \endcode

    or:

    \code
    value operator identifier
    \endcode
*/

/*!
    \enum QIviFilterTerm::Operator
    \value Equals
           Tests whether the value from the identifier is the equal to the passed value. In case of a string the comparison is case-senstitive.
    \value EqualsCaseInsensitive
           Tests whether the value from the identifier is the equal to the passed value, but the comparison is done case-insensitive.
    \value Unequals
           Tests whether the value from the identifier is the unequal to the passed value. In case of a string the comparison is case-senstitive.
    \value GreaterThan
           Tests whether the value from the identifier is greater than the passed value. This does only work for numbers.
    \value GreaterEquals
           Tests whether the value from the identifier is greater than or equal to the passed value. This does only work for numbers.
    \value LowerThan
           Tests whether the value from the identifier is lower than the passed value. This does only work for numbers.
    \value LowerEquals
           Tests whether the value from the identifier is lower than or equal to the passed value. This does only work for numbers.
*/

QIviFilterTerm::QIviFilterTerm()
    : d_ptr(new QIviFilterTermPrivate)
{
}

QIviFilterTerm::~QIviFilterTerm()
{
    delete d_ptr;
}

/*!
    \reimp
*/
QIviAbstractQueryTerm::Type QIviFilterTerm::type() const
{
    return QIviAbstractQueryTerm::FilterTerm;
}

/*!
    \reimp
*/
QString QIviFilterTerm::toString() const
{
    Q_D(const QIviFilterTerm);
    QString string;

    string = d->m_property + d->operatorToString() + d->m_value.toString();

    if (d->m_negated)
        string.prepend(QLatin1String("!"));

    return string;
}

/*!
    Returns the operator of this filter.
*/
QIviFilterTerm::Operator QIviFilterTerm::operatorType() const
{
    Q_D(const QIviFilterTerm);
    return d->m_operator;
}

/*!
    Returns the value of the filter.
*/
QVariant QIviFilterTerm::value() const
{
    Q_D(const QIviFilterTerm);
    return d->m_value;
}

/*!
    Returns the property this filter should act on.
*/
QString QIviFilterTerm::propertyName() const
{
    Q_D(const QIviFilterTerm);
    return d->m_property;
}

/*!
    Returns \c true when this term is negated, otherwise \c false
*/
bool QIviFilterTerm::isNegated() const
{
    Q_D(const QIviFilterTerm);
    return d->m_negated;
}

/*!
    \class QIviOrderTerm
    \inmodule QtIviCore
    \brief The QIviOrderTerm is the representation of a scope which can hold another term.
*/
QIviOrderTerm::QIviOrderTerm()
    : d(new QIviOrderTermPrivate)
{
}

QIviOrderTerm::QIviOrderTerm(const QIviOrderTerm &other)
    : d(other.d)
{
}

QIviOrderTerm::~QIviOrderTerm()
{
}

QIviOrderTerm &QIviOrderTerm::operator =(const QIviOrderTerm &other)
{
    d = other.d;
    return *this;
}

/*!
    Returns \c true when it should be sorted in ascending order.
    Returns \c false when it should be sorted in descending order.
*/
bool QIviOrderTerm::isAscending() const
{
    return d->m_ascending;
}

/*!
    Returns the property which should be used for sorting.
*/
QString QIviOrderTerm::propertyName() const
{
    return d->m_propertyName;
}

QDataStream &operator<<(QDataStream &out, QIviConjunctionTerm::Conjunction var)
{
    out << int(var);
    return out;
}

QDataStream &operator>>(QDataStream &in, QIviConjunctionTerm::Conjunction &var)
{
    int val;
    in >> val;
    QMetaEnum metaEnum = QMetaEnum::fromType<QIviConjunctionTerm::Conjunction>();
    if (metaEnum.valueToKey(val) == nullptr)
        qWarning() << "Received an invalid enum value for type QIviConjunctionTerm::Conjunction, value =" << val;
    var = QIviConjunctionTerm::Conjunction(val);
    return in;
}

QDataStream &operator<<(QDataStream &out, QIviFilterTerm::Operator var)
{
    out << int(var);
    return out;
}

QDataStream &operator>>(QDataStream &in, QIviFilterTerm::Operator &var)
{
    int val;
    in >> val;
    QMetaEnum metaEnum = QMetaEnum::fromType<QIviFilterTerm::Operator>();
    if (metaEnum.valueToKey(val) == nullptr)
        qWarning() << "Received an invalid enum value for type QIviFilterTerm::Operator, value =" << val;
    var = QIviFilterTerm::Operator(val);
    return in;
}

QDataStream &operator<<(QDataStream &out, QIviAbstractQueryTerm *var)
{
    if (var->type() == QIviAbstractQueryTerm::FilterTerm) {
        auto *term = static_cast<QIviFilterTerm*>(var);
        out << QStringLiteral("filter");
        out << term->operatorType();
        out << term->value();
        out << term->propertyName();
        out << term->isNegated();
    } else if (var->type() == QIviAbstractQueryTerm::ScopeTerm) {
        auto *term = static_cast<QIviScopeTerm*>(var);
        out << QStringLiteral("scope");
        out << term->isNegated();
        out << term->term();
    } else {
        auto *term = static_cast<QIviConjunctionTerm*>(var);
        out << QStringLiteral("conjunction");
        out << term->conjunction();
        const auto subTerms = term->terms();
        out << subTerms.count();
        for (const auto subTerm : subTerms)
            out << subTerm;
    }
    return out;
}

QDataStream &operator>>(QDataStream &in, QIviAbstractQueryTerm **var)
{
    QString type;
    QIviAbstractQueryTerm *aTerm=nullptr;
    in >> type;
    if (type == QStringLiteral("filter")) {
        auto term = new QIviFilterTerm();
        aTerm = term;
        in >> term->d_ptr->m_operator;
        in >> term->d_ptr->m_value;
        in >> term->d_ptr->m_property;
        in >> term->d_ptr->m_negated;
    } else if (type == QStringLiteral("scope")) {
        auto term = new QIviScopeTerm();
        aTerm = term;
        in >> term->d_ptr->m_negated;
        in >> &term->d_ptr->m_term;
    } else {
        Q_ASSERT(type == QStringLiteral("conjunction"));
        auto term = new QIviConjunctionTerm();
        aTerm = term;
        int count = 0;
        in >> term->d_ptr->m_conjunction;
        in >> count;
        for (int i = 0; i < count; ++i) {
            QIviAbstractQueryTerm *subTerm=nullptr;
            in >> &subTerm;
            term->d_ptr->m_terms.append(subTerm);
        }
    }
    *var = aTerm;
    return in;
}

QDataStream &operator<<(QDataStream &out, const QIviOrderTerm &var)
{
    out << var.propertyName();
    out << var.isAscending();
    return out;
}

QDataStream &operator>>(QDataStream &in, QIviOrderTerm &var)
{
    in >> var.d->m_propertyName;
    in >> var.d->m_ascending;
    return in;
}

QT_END_NAMESPACE
