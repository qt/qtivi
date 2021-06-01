----------------------------------------------------------------------------
--
-- Copyright (C) 2021 The Qt Company Ltd.
-- Copyright (C) 2018 Pelagicore AG
-- Contact: https://www.qt.io/licensing/
--
-- This file is part of the QtIvi module of the Qt Toolkit.
--
-- $QT_BEGIN_LICENSE:LGPL$
-- Commercial License Usage
-- Licensees holding valid commercial Qt licenses may use this file in
-- accordance with the commercial license agreement provided with the
-- Software or, alternatively, in accordance with the terms contained in
-- a written agreement between you and The Qt Company. For licensing terms
-- and conditions see https://www.qt.io/terms-conditions. For further
-- information use the contact form at https://www.qt.io/contact-us.
--
-- GNU Lesser General Public License Usage
-- Alternatively, this file may be used under the terms of the GNU Lesser
-- General Public License version 3 as published by the Free Software
-- Foundation and appearing in the file LICENSE.LGPL3 included in the
-- packaging of this file. Please review the following information to
-- ensure the GNU Lesser General Public License version 3 requirements
-- will be met: https://www.gnu.org/licenses/lgpl-3.0.html.
--
-- GNU General Public License Usage
-- Alternatively, this file may be used under the terms of the GNU
-- General Public License version 2.0 or (at your option) the GNU General
-- Public license version 3 or any later version approved by the KDE Free
-- Qt Foundation. The licenses are as published by the Free Software
-- Foundation and appearing in the file LICENSE.GPL2 and LICENSE.GPL3
-- included in the packaging of this file. Please review the following
-- information to ensure the GNU General Public License requirements will
-- be met: https://www.gnu.org/licenses/gpl-2.0.html and
-- https://www.gnu.org/licenses/gpl-3.0.html.
--
-- $QT_END_LICENSE$
--
----------------------------------------------------------------------------

--
--  W A R N I N G
--  -------------
--
-- This file is not part of the Qt API.  It exists purely as an
-- implementation detail. This header file may change from version to
-- version without notice, or even be removed.
--
-- We mean it.
--

%parser QIviQueryParserTable
%merged_output qiviqueryparser_p.h

%token AND_OP2 "&"
%token AND_OP "&&"
%token OR_OP2 "|"
%token OR_OP "||"
%token BANG "!"
%token EQ_OP "=="
%token EQ_OP2 "="
%token IC_EQ_OP "~="
%token GE_OP ">="
%token GT_OP ">"
%token LE_OP "<="
%token LT_OP "<"
%token NE_OP "!="
%token LEFT_PAREN "("
%token RIGHT_PAREN ")"
%token ASCENDING "/"
%token DESCENDING "\\"
%token LEFT_BRACKET "["
%token RIGHT_BRACKET "]"
%token INTCONSTANT "integer"
%token FLOATCONSTANT "float"
%token IDENTIFIER "identifier"
%token STRING "string"
%token SPACE
%token ERROR


%start translation_unit

/:

#include <QtCore>
#include <QtIviCore/QIviAbstractQueryTerm>
#include <QtIviCore/private/qiviqueryterm_p.h>

QT_BEGIN_NAMESPACE

//TODO Find a better way of doing it, this is not reentrant
QString* currentQuery = nullptr;
unsigned int *currentOffset = nullptr;
void readQueryBuffer(char *buffer, unsigned int &numBytesRead,int maxBytesToRead)
{
    if (!currentQuery) {
        numBytesRead=0;
        return;
    }

    int numBytesToRead = maxBytesToRead;
    int bytesRemaining = currentQuery->count()-(*currentOffset);
    int i;
    if ( numBytesToRead > bytesRemaining )
        numBytesToRead = bytesRemaining;


    for (i = 0; i < numBytesToRead; i++) {
        buffer[i] = currentQuery->toLatin1().at(*currentOffset+i);
    }

    numBytesRead = numBytesToRead;
    *currentOffset += numBytesToRead;
}

class QIviQueryParser: protected $table
{
public:
    union Value {
      int i;
      float f;
      const QString *s;
    };

public:
    QIviQueryParser();
    ~QIviQueryParser();

    QIviAbstractQueryTerm *parse();

    void setQuery(const QString& query)
    {
        m_query = query;
    }

    QString lastError() {
        return m_error;
    }

    void setAllowedIdentifiers(const QSet<QString> &list)
    {
        m_identifierList = list;
    }

    QList<QIviOrderTerm> orderTerms() const
    {
        return m_orderList;
    }

protected:
    inline void reallocateStack();

    inline QVariant &sym(int index)
    {
        return sym_stack [tos + index - 1];
    }

    void initBuffer()
    {
        currentQuery = &m_query;
        currentOffset = &m_offset;
    }

    void setErrorString(const QString &error);

    void calcCurrentColumn();

    int nextToken();

    void handleConjunction(bool bangOperator);
    void handleScope(bool bang);

    void negateLeftMostTerm(QIviAbstractQueryTerm *term);

    bool checkIdentifier(const QString &identifer);

protected:
    QString m_query;
    unsigned int m_offset;
    QString m_error;
    QSet<QString> m_identifierList;

    int column;
    int tos;
    QVector<QVariant> sym_stack;
    QVector<int> state_stack;
    QVariant yylval;

    QStack<QIviAbstractQueryTerm*> m_termStack;
    QStack<QIviFilterTerm::Operator> m_operatorStack;
    QStack<QIviConjunctionTerm::Conjunction> m_conjunctionStack;
    QList<QIviOrderTerm> m_orderList;
};

inline void QIviQueryParser::reallocateStack()
{
    int size = state_stack.size();
    if (size == 0)
        size = 128;
    else
        size <<= 1;

    sym_stack.resize(size);
    state_stack.resize(size);
}

:/


/.

QIviQueryParser::QIviQueryParser():
    m_offset(0),
    column(0),
    tos(0)
{
      reallocateStack();
}

//This is needed to comparison warning in the generated flex code.
#define YY_TYPEDEF_YY_SIZE_T
typedef int yy_size_t;

#include "qiviqueryparser_flex_p.h"

QIviQueryParser::~QIviQueryParser()
{
    currentOffset = nullptr;
    currentQuery = nullptr;

    //We need to reset the lexer to reinitialize it when needed
    yy_init = nullptr;

    //Get rid of the unused warning
    if (0)
        yyunput(0, 0);}

void QIviQueryParser::calcCurrentColumn()
{
    column += yyleng;
}

void QIviQueryParser::negateLeftMostTerm(QIviAbstractQueryTerm *term)
{
    if (term->type() == QIviAbstractQueryTerm::ConjunctionTerm) {
        QIviConjunctionTerm* conjunction = static_cast<QIviConjunctionTerm*>(term);
        negateLeftMostTerm(conjunction->terms().at(0));
    } else if (term->type() == QIviAbstractQueryTerm::ScopeTerm) {
        QIviScopeTerm* scopeTerm = static_cast<QIviScopeTerm*>(term);
        scopeTerm->d_func()->m_negated = true;
    } else if (term->type() == QIviAbstractQueryTerm::FilterTerm) {
        QIviFilterTerm* filterTerm = static_cast<QIviFilterTerm*>(term);
        filterTerm->d_func()->m_negated = true;
    } else {
        qCritical() << "New Term type added but not handled in" << Q_FUNC_INFO;
    }

    return;
}

void QIviQueryParser::handleConjunction(bool bangOperator)
{
    QList<QIviAbstractQueryTerm*> list;
    list.prepend(m_termStack.pop());
    list.prepend(m_termStack.pop());

    QIviConjunctionTerm *conjunction1 = nullptr;
    QIviConjunctionTerm *conjunction2 = nullptr;
    int i = 0;
    for (QIviAbstractQueryTerm *term : list) {
        if (term->type() == QIviAbstractQueryTerm::ConjunctionTerm) {
            QIviConjunctionTerm *conj = static_cast<QIviConjunctionTerm*>(term);
            if (conj->conjunction() == m_conjunctionStack.top()) {
                if (i == 0)
                    conjunction1 = conj;
                else
                    conjunction2 = conj;
            }
        }
        i++;
    }

    //Handle the bang Operator
    if (bangOperator)
        negateLeftMostTerm(list.at(1));

    QIviConjunctionTerm::Conjunction conjunction = m_conjunctionStack.pop();
    //Both are conjunctions, we can sum it together into one.
    if (conjunction1 && conjunction2) {
        conjunction1->d_func()->m_terms += conjunction2->d_func()->m_terms;
        conjunction2->d_func()->m_terms.clear();
        delete conjunction2;
        m_termStack.push(conjunction1);
    } else if (conjunction1) {
        conjunction1->d_func()->m_terms.prepend(list.at(1));
        m_termStack.push(conjunction1);
    } else if (conjunction2) {
        conjunction2->d_func()->m_terms.prepend(list.at(0));
        m_termStack.push(conjunction2);
    } else {
        QIviConjunctionTerm *term = new QIviConjunctionTerm();
        term->d_func()->m_conjunction = conjunction;
        term->d_func()->m_terms = list;
        m_termStack.push(term);
    }
}

void QIviQueryParser::handleScope(bool bangOperator)
{
    QIviAbstractQueryTerm *term = m_termStack.pop();

    if (bangOperator)
        negateLeftMostTerm(term);

    QIviScopeTerm *scopeTerm = new QIviScopeTerm();
    scopeTerm->d_func()->m_term = term;
    m_termStack.push(scopeTerm);
}

bool QIviQueryParser::checkIdentifier(const QString &identifer)
{
    if (!m_identifierList.isEmpty() && !m_identifierList.contains(identifer)) {
        QString errorMessage = QString(QLatin1String("Got %1 but expected on of the following identifiers:\n")).arg(identifer);
        for (const QString &ident : qAsConst(m_identifierList))
            errorMessage.append(QString(QLatin1String("     %1\n")).arg(ident));

        setErrorString(errorMessage);

        qDeleteAll(m_termStack);

        return false;
    }

    return true;
}

QIviAbstractQueryTerm *QIviQueryParser::parse()
{
    const int INITIAL_STATE = 0;

    int yytoken = -1;

    tos = 0;
    m_offset = 0;
    column = 0;
    state_stack[++tos] = INITIAL_STATE;
    m_termStack.clear();
    m_orderList.clear();

    yyrestart(yyin);

    while (true)
    {
        const int state = state_stack.at(tos);
        if (yytoken == -1 && - TERMINAL_COUNT != action_index [state])
            yytoken = nextToken();

        if (yytoken == ERROR) {
            setErrorString(QString(QLatin1String("Unrecognized token '%1'\n")).arg(QLatin1String(yytext)));
            qDeleteAll(m_termStack);
            return 0;
        }

        if (yytoken == SPACE)
            yytoken = nextToken();

        int act = t_action (state, yytoken);

        if (act == ACCEPT_STATE) {
#ifdef PARSER_DEBUG
            qDebug() << "Representation finished. ToString" << m_termStack.top()->toString();
#endif
            return m_termStack.pop();
        } else if (act > 0) {
            if (++tos == state_stack.size())
                reallocateStack();

            sym_stack [tos] = yylval;
            state_stack [tos] = act;
            yytoken = -1;
        } else if (act < 0) {
            int r = - act - 1;

#ifdef PARSER_DEBUG
            int ridx = rule_index [r];
            qDebug ("*** reduce using rule %d %s :::=", r + 1, spell[rule_info [ridx]]);
            ++ridx;
            for (int i = ridx; i < ridx + rhs [r]; ++i)
            {
                int symbol = rule_info [i];
                if (const char *name = spell [symbol])
                    qDebug (" %s", name);
                else
                    qDebug (" #%d", symbol);
            }
#endif

            tos -= rhs [r];
            act = state_stack.at(tos++);

            switch (r) {
./


translation_unit ::= bang_clause order_term;
translation_unit ::= bang_clause;

order_term ::= LEFT_BRACKET order_clauses RIGHT_BRACKET;

order_clauses ::= order_clause;
order_clauses ::= order_clause order_clauses;

order_clause ::= ASCENDING IDENTIFIER;
/.
              case $rule_number: {
                    QIviOrderTerm order;
                    order.d->m_ascending = true;
                    order.d->m_propertyName = sym(2).toString();
                    m_orderList.append(order);
              } break;
./
order_clause ::= DESCENDING IDENTIFIER;
/.
              case $rule_number: {
                    QIviOrderTerm order;
                    order.d->m_ascending = false;
                    order.d->m_propertyName = sym(2).toString();
                    m_orderList.append(order);
              } break;
./

bang_clause ::= BANG complex_clause ;
/.
              case $rule_number: {
                QIviAbstractQueryTerm *term = m_termStack.top();

                negateLeftMostTerm(term);
              } break;
./
bang_clause ::= complex_clause ;

complex_clause ::= parenthesed_clause complex_operator complex_clause ;
/.
              case $rule_number: {
                    handleConjunction(false);
              } break;
./
complex_clause ::= parenthesed_clause complex_operator BANG complex_clause ;
/.
              case $rule_number: {
                  handleConjunction(true);
              } break;
./
complex_clause ::= parenthesed_clause ;

parenthesed_clause ::= LEFT_PAREN BANG complex_clause RIGHT_PAREN;
/.
              case $rule_number: {
                  handleScope(true);
              } break;
./
parenthesed_clause ::= LEFT_PAREN complex_clause RIGHT_PAREN;
/.
              case $rule_number: {
                  handleScope(false);
              } break;
./
parenthesed_clause ::= clause ;

complex_operator ::= OR_OP ;
/.
              case $rule_number: {
                  m_conjunctionStack.push(QIviConjunctionTerm::Or);
              } break;
./
complex_operator ::= OR_OP2 ;
/.
              case $rule_number: {
                  m_conjunctionStack.push(QIviConjunctionTerm::Or);
              } break;
./
complex_operator ::= AND_OP ;
/.
              case $rule_number: {
                  m_conjunctionStack.push(QIviConjunctionTerm::And);
              } break;
./
complex_operator ::= AND_OP2 ;
/.
              case $rule_number: {
                  m_conjunctionStack.push(QIviConjunctionTerm::And);
              } break;
./

clause ::= IDENTIFIER number_operator literal ;
/.
              case $rule_number: {
                    if (!checkIdentifier(sym(1).toString()))
                        return 0;
                    QIviFilterTerm *term = new QIviFilterTerm();
                    term->d_func()->m_property = sym(1).toString();
                    term->d_func()->m_operator = m_operatorStack.pop();
                    term->d_func()->m_value = sym(3);
                    m_termStack.push(term);
              } break;
./
clause ::= IDENTIFIER string_operator STRING ;
/.
              case $rule_number: {
                    if (!checkIdentifier(sym(1).toString()))
                        return 0;
                    QIviFilterTerm *term = new QIviFilterTerm();
                    term->d_func()->m_property = sym(1).toString();
                    term->d_func()->m_operator = m_operatorStack.pop();
                    term->d_func()->m_value = sym(3);
                    m_termStack.push(term);
              } break;
./
clause ::= STRING string_operator IDENTIFIER ;
/.
              case $rule_number: {
                    if (!checkIdentifier(sym(3).toString()))
                        return 0;
                    QIviFilterTerm *term = new QIviFilterTerm();
                    term->d_func()->m_property = sym(3).toString();
                    term->d_func()->m_operator = m_operatorStack.pop();
                    term->d_func()->m_value = sym(1);
                    m_termStack.push(term);
              } break;
./
clause ::= literal number_operator IDENTIFIER ;
/.
              case $rule_number: {
                    if (!checkIdentifier(sym(3).toString()))
                        return 0;

                    QIviFilterTerm::Operator op = m_operatorStack.pop();

                    switch (op) {
                        case QIviFilterTerm::GreaterEquals: op = QIviFilterTerm::LowerEquals; break;
                        case QIviFilterTerm::GreaterThan: op = QIviFilterTerm::LowerThan; break;
                        case QIviFilterTerm::LowerEquals: op = QIviFilterTerm::GreaterEquals; break;
                        case QIviFilterTerm::LowerThan: op = QIviFilterTerm::GreaterThan; break;
                        default: qFatal("The Grammer was changed but not all logic was ported properly");
                    }

                    QIviFilterTerm *term = new QIviFilterTerm();
                    term->d_func()->m_property = sym(3).toString();
                    term->d_func()->m_operator = op;
                    term->d_func()->m_value = sym(1);
                    m_termStack.push(term);
              } break;
./

literal ::= INTCONSTANT ;
literal ::= FLOATCONSTANT ;

number_operator ::= GE_OP ;
/.
              case $rule_number: {
                  m_operatorStack.push(QIviFilterTerm::GreaterEquals);
              } break;
./
number_operator ::= GT_OP ;
/.
              case $rule_number: {
                  m_operatorStack.push(QIviFilterTerm::GreaterThan);
              } break;
./
number_operator ::= LE_OP ;
/.
              case $rule_number: {
                  m_operatorStack.push(QIviFilterTerm::LowerEquals);
              } break;
./
number_operator ::= LT_OP ;
/.
              case $rule_number: {
                  m_operatorStack.push(QIviFilterTerm::LowerThan);
              } break;
./
number_operator ::= multi_operator ;

string_operator ::= multi_operator ;
string_operator ::= IC_EQ_OP ;
/.
              case $rule_number: {
                  m_operatorStack.push(QIviFilterTerm::EqualsCaseInsensitive);
              } break;
./

multi_operator ::= EQ_OP ;
/.
              case $rule_number: {
                  m_operatorStack.push(QIviFilterTerm::Equals);
              } break;
./
multi_operator ::= EQ_OP2 ;
/.
              case $rule_number: {
                  m_operatorStack.push(QIviFilterTerm::Equals);
              } break;
./
multi_operator ::= NE_OP ;
/.
              case $rule_number: {
                  m_operatorStack.push(QIviFilterTerm::Unequals);
              } break;
./

/.
            } // switch

            state_stack [tos] = nt_action (act, lhs [r] - TERMINAL_COUNT);
        } else {
            int ers = state;
            int shifts = 0;
            int reduces = 0;
            QList<int> expectedTokens;
            for (int tk = 0; tk < TERMINAL_COUNT; ++tk) {

                int k = t_action(ers, tk);


                if (! k)
                    continue;
                else if (k < 0)
                    ++reduces;
                else if (spell[tk]) {
                    if (shifts < 7)
                        expectedTokens.append(tk);
                    ++shifts;
                }
            }

            QString errorMessage = QString(QLatin1String("Got %1 but expected on of the following types:\n")).arg(QLatin1String(spell[yytoken]));
            for (int token : expectedTokens)
                errorMessage.append(QString(QLatin1String("     %1\n")).arg(QLatin1String(spell[token])));

            setErrorString(errorMessage);

            qDeleteAll(m_termStack);

            return 0;
        }
    }

    return 0;
}

void QIviQueryParser::setErrorString(const QString &error)
{
    int err_col = column - yyleng;

    m_error = error;

    m_error.append(m_query).append(QLatin1String("\n"));
    QString marker(QLatin1String("^"));

    for (int i=0; i<err_col; i++)
        marker.prepend(QLatin1String(" "));

    for (int i=0; i<yyleng - 1; i++)
        marker.append(QLatin1String("-"));

    m_error.append(marker);
}

QT_END_NAMESPACE

./
