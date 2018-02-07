/****************************************************************************
**
** Copyright (C) 2018 Pelagicore AG
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtIvi module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:GPL-EXCEPT-QTAS$
** Commercial License Usage
** Licensees holding valid commercial Qt Automotive Suite licenses may use
** this file in accordance with the commercial license agreement provided
** with the Software or, alternatively, in accordance with the terms
** contained in a written agreement between you and The Qt Company.  For
** licensing terms and conditions see https://www.qt.io/terms-conditions.
** For further information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 as published by the Free Software
** Foundation with exceptions as appearing in the file LICENSE.GPL3-EXCEPT
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/


#include <QtTest>
#include <qivipropertyattribute.h>

struct ComplexType
{
    ComplexType(bool b = false, const QString &string = QString())
        : m_bool(b)
        , m_string(string)
    {}

    bool operator==(const ComplexType &other) const
    {
        return (m_bool == other.m_bool &&
                m_string == other.m_string);
    }

    bool m_bool;
    QString m_string;
};
Q_DECLARE_METATYPE(ComplexType)
Q_DECLARE_METATYPE(QIviPropertyAttribute<ComplexType>)

class tst_QIviPropertyAttribute : public QObject
{
    Q_OBJECT

public:
    tst_QIviPropertyAttribute();

private Q_SLOTS:
    void equal();
    void simple();
    void complex();
};

tst_QIviPropertyAttribute::tst_QIviPropertyAttribute()
{
    qRegisterMetaType<ComplexType>();
}

void tst_QIviPropertyAttribute::equal()
{
    QIviPropertyAttribute<int> minMax(5, 10);
    QIviPropertyAttribute<int> minMax2(5, 10);
    QCOMPARE(minMax, minMax2);

    QIviPropertyAttribute<int> noAtt(false);
    QIviPropertyAttribute<int> noAtt2(false);
    QCOMPARE(noAtt, noAtt2);

    QVector<int> list;
    list << 0 << 5 << 10 << 50;
    QIviPropertyAttribute<int> avVal(list);
    QIviPropertyAttribute<int> avVal2(list);
    QCOMPARE(avVal, avVal2);
}

void tst_QIviPropertyAttribute::simple()
{
    QIviPropertyAttribute<int> invalid;
    QCOMPARE(invalid.type(), QIviPropertyAttributeBase::Invalid);
    QCOMPARE(invalid.isAvailable(), false);
    QCOMPARE(invalid.minimumValue(), int());
    QCOMPARE(invalid.maximumValue(), int());
    QCOMPARE(invalid.availableValues(), QVector<int>());

    QIviPropertyAttribute<int> noAttribute(true);
    QCOMPARE(noAttribute.type(), QIviPropertyAttributeBase::NoAttributes);
    QCOMPARE(noAttribute.isAvailable(), true);
    QCOMPARE(noAttribute.minimumValue(), int());
    QCOMPARE(noAttribute.maximumValue(), int());
    QCOMPARE(noAttribute.availableValues(), QVector<int>());

    QIviPropertyAttribute<int> minimumMaximum(5, 10);
    QCOMPARE(minimumMaximum.type(), QIviPropertyAttributeBase::MinimumMaximum);
    QCOMPARE(minimumMaximum.isAvailable(), true);
    QCOMPARE(minimumMaximum.minimumValue(), 5);
    QCOMPARE(minimumMaximum.maximumValue(), 10);
    QCOMPARE(minimumMaximum.availableValues(), QVector<int>());

    QVector<int> list;
    list << 0 << 5 << 10 << 50;
    QIviPropertyAttribute<int> availableValues(list);
    QCOMPARE(availableValues.type(), QIviPropertyAttributeBase::AvailableValues);
    QCOMPARE(availableValues.isAvailable(), true);
    QCOMPARE(availableValues.minimumValue(), int());
    QCOMPARE(availableValues.maximumValue(), int());
    QCOMPARE(availableValues.availableValues(), list);
}

void tst_QIviPropertyAttribute::complex()
{
    QIviPropertyAttribute<ComplexType> invalid;
    QCOMPARE(invalid.type(), QIviPropertyAttributeBase::Invalid);
    QCOMPARE(invalid.isAvailable(), false);
    QCOMPARE(invalid.minimumValue(), ComplexType());
    QCOMPARE(invalid.maximumValue(), ComplexType());
    QCOMPARE(invalid.availableValues(), QVector<ComplexType>());

    QIviPropertyAttribute<ComplexType> noAttribute(true);
    QCOMPARE(noAttribute.type(), QIviPropertyAttributeBase::NoAttributes);
    QCOMPARE(noAttribute.isAvailable(), true);
    QCOMPARE(noAttribute.minimumValue(), ComplexType());
    QCOMPARE(noAttribute.maximumValue(), ComplexType());
    QCOMPARE(noAttribute.availableValues(), QVector<ComplexType>());

    ComplexType min(false, QStringLiteral("min"));
    ComplexType max(true, QStringLiteral("max"));
    QIviPropertyAttribute<ComplexType> minimumMaximum(min, max);
    QCOMPARE(minimumMaximum.type(), QIviPropertyAttributeBase::MinimumMaximum);
    QCOMPARE(minimumMaximum.isAvailable(), true);
    QCOMPARE(minimumMaximum.minimumValue(), min);
    QCOMPARE(minimumMaximum.maximumValue(), max);
    QCOMPARE(minimumMaximum.availableValues(), QVector<ComplexType>());

    QVector<ComplexType> list;
    list << ComplexType(false, "1") << ComplexType(true, "2") << ComplexType(true, "3") << ComplexType(false, "4");
    QIviPropertyAttribute<ComplexType> availableValues(list);
    QCOMPARE(availableValues.type(), QIviPropertyAttributeBase::AvailableValues);
    QCOMPARE(availableValues.isAvailable(), true);
    QCOMPARE(availableValues.minimumValue(), ComplexType());
    QCOMPARE(availableValues.maximumValue(), ComplexType());
    QCOMPARE(availableValues.availableValues(), list);
}

QTEST_APPLESS_MAIN(tst_QIviPropertyAttribute)

#include "tst_qivipropertyattribute.moc"

