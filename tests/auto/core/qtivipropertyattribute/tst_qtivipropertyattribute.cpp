/****************************************************************************
**
** Copyright (C) 2016 Pelagicore AG
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtIVI module of the Qt Toolkit.
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
#include <qtivipropertyattribute.h>

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
Q_DECLARE_METATYPE(QtIVIPropertyAttribute<int>)
Q_DECLARE_METATYPE(QtIVIPropertyAttribute<ComplexType>)

class tst_QtIVIPropertyAttribute : public QObject
{
    Q_OBJECT

public:
    tst_QtIVIPropertyAttribute();

private Q_SLOTS:
    void equal();
    void simple();
    void complex();
};

tst_QtIVIPropertyAttribute::tst_QtIVIPropertyAttribute()
{
    qRegisterMetaType<ComplexType>();
}

void tst_QtIVIPropertyAttribute::equal()
{
    QtIVIPropertyAttribute<int> minMax(5, 10);
    QtIVIPropertyAttribute<int> minMax2(5, 10);
    QCOMPARE(minMax, minMax2);

    QtIVIPropertyAttribute<int> noAtt(false);
    QtIVIPropertyAttribute<int> noAtt2(false);
    QCOMPARE(noAtt, noAtt2);

    QVector<int> list;
    list << 0 << 5 << 10 << 50;
    QtIVIPropertyAttribute<int> avVal(list);
    QtIVIPropertyAttribute<int> avVal2(list);
    QCOMPARE(avVal, avVal2);
}

void tst_QtIVIPropertyAttribute::simple()
{
    QtIVIPropertyAttribute<int> invalid;
    QCOMPARE(invalid.type(), QtIVIPropertyAttributeBase::Invalid);
    QCOMPARE(invalid.isAvailable(), false);
    QCOMPARE(invalid.minimumValue(), int());
    QCOMPARE(invalid.maximumValue(), int());
    QCOMPARE(invalid.availableValues(), QVector<int>());

    QtIVIPropertyAttribute<int> noAttribute(true);
    QCOMPARE(noAttribute.type(), QtIVIPropertyAttributeBase::NoAttributes);
    QCOMPARE(noAttribute.isAvailable(), true);
    QCOMPARE(noAttribute.minimumValue(), int());
    QCOMPARE(noAttribute.maximumValue(), int());
    QCOMPARE(noAttribute.availableValues(), QVector<int>());

    QtIVIPropertyAttribute<int> minimumMaximum(5, 10);
    QCOMPARE(minimumMaximum.type(), QtIVIPropertyAttributeBase::MinimumMaximum);
    QCOMPARE(minimumMaximum.isAvailable(), true);
    QCOMPARE(minimumMaximum.minimumValue(), 5);
    QCOMPARE(minimumMaximum.maximumValue(), 10);
    QCOMPARE(minimumMaximum.availableValues(), QVector<int>());

    QVector<int> list;
    list << 0 << 5 << 10 << 50;
    QtIVIPropertyAttribute<int> availableValues(list);
    QCOMPARE(availableValues.type(), QtIVIPropertyAttributeBase::AvailableValues);
    QCOMPARE(availableValues.isAvailable(), true);
    QCOMPARE(availableValues.minimumValue(), int());
    QCOMPARE(availableValues.maximumValue(), int());
    QCOMPARE(availableValues.availableValues(), list);
}

void tst_QtIVIPropertyAttribute::complex()
{
    QtIVIPropertyAttribute<ComplexType> invalid;
    QCOMPARE(invalid.type(), QtIVIPropertyAttributeBase::Invalid);
    QCOMPARE(invalid.isAvailable(), false);
    QCOMPARE(invalid.minimumValue(), ComplexType());
    QCOMPARE(invalid.maximumValue(), ComplexType());
    QCOMPARE(invalid.availableValues(), QVector<ComplexType>());

    QtIVIPropertyAttribute<ComplexType> noAttribute(true);
    QCOMPARE(noAttribute.type(), QtIVIPropertyAttributeBase::NoAttributes);
    QCOMPARE(noAttribute.isAvailable(), true);
    QCOMPARE(noAttribute.minimumValue(), ComplexType());
    QCOMPARE(noAttribute.maximumValue(), ComplexType());
    QCOMPARE(noAttribute.availableValues(), QVector<ComplexType>());

    ComplexType min(false, QStringLiteral("min"));
    ComplexType max(true, QStringLiteral("max"));
    QtIVIPropertyAttribute<ComplexType> minimumMaximum(min, max);
    QCOMPARE(minimumMaximum.type(), QtIVIPropertyAttributeBase::MinimumMaximum);
    QCOMPARE(minimumMaximum.isAvailable(), true);
    QCOMPARE(minimumMaximum.minimumValue(), min);
    QCOMPARE(minimumMaximum.maximumValue(), max);
    QCOMPARE(minimumMaximum.availableValues(), QVector<ComplexType>());

    QVector<ComplexType> list;
    list << ComplexType(false, "1") << ComplexType(true, "2") << ComplexType(true, "3") << ComplexType(false, "4");
    QtIVIPropertyAttribute<ComplexType> availableValues(list);
    QCOMPARE(availableValues.type(), QtIVIPropertyAttributeBase::AvailableValues);
    QCOMPARE(availableValues.isAvailable(), true);
    QCOMPARE(availableValues.minimumValue(), ComplexType());
    QCOMPARE(availableValues.maximumValue(), ComplexType());
    QCOMPARE(availableValues.availableValues(), list);
}

QTEST_APPLESS_MAIN(tst_QtIVIPropertyAttribute)

#include "tst_qtivipropertyattribute.moc"

