/****************************************************************************
**
** Copyright (C) 2020 The Qt Company Ltd.
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
#include <QQmlEngine>
#include <QQmlContext>
#include <QQmlComponent>
#include <QScopedPointer>
#include <QJsonDocument>

#include <private/qivisimulationglobalobject_p.h>

class InvalidStruct {
    Q_GADGET
    Q_PROPERTY(int intProperty READ intProperty)

public:
    int intProperty() const { return m_intProperty; }

private:
    int m_intProperty = -1;
};


class InvalidStruct2 {
    Q_GADGET
    Q_PROPERTY(int intProperty READ intProperty)

public:
    int intProperty() const { return m_intProperty; }

private:
    int m_intProperty = -1;
};

class TestStruct {
    Q_GADGET
    Q_PROPERTY(int intProperty READ intProperty)
    Q_PROPERTY(bool boolProperty READ boolProperty)

public:
    TestStruct(int intProperty = -1, bool boolProperty = false)
        : m_intProperty(intProperty)
        , m_boolProperty(boolProperty)
    {
    }

    int intProperty() const { return m_intProperty; }
    bool boolProperty() const { return m_boolProperty; }

    bool operator==(const TestStruct& other) const {
        return (m_intProperty == other.m_intProperty &&
                m_boolProperty == other.m_boolProperty);
    }

protected:
    Q_INVOKABLE void fromJSON(const QVariant &variant) {
        QVariant value = qtivi_convertFromJSON(variant);
        // First try to convert the values to a Map or a List
        // This is needed as it could also store a QStringList or a Hash
        if (value.canConvert(QMetaType::fromType<QVariantMap>()))
            value.convert(QMetaType::fromType<QVariantMap>());
        if (value.canConvert(QMetaType::fromType<QVariantList>()))
            value.convert(QMetaType::fromType<QVariantList>());

        if (value.metaType() == QMetaType::fromType<QVariantMap>()) {
            QVariantMap map = value.toMap();
            if (map.contains(QStringLiteral("intProperty")))
                m_intProperty = map.value(QStringLiteral("intProperty")).value<int>();
            if (map.contains(QStringLiteral("boolProperty")))
                m_boolProperty = map.value(QStringLiteral("boolProperty")).value<bool>();
        } else if (value.metaType() == QMetaType::fromType<QVariantList>()) {
            QVariantList values = value.toList();
            m_intProperty = values.value(0).value<int>();
            m_boolProperty = values.value(1).value<bool>();
        }
    }

private:
    int m_intProperty = -1;
    bool m_boolProperty = false;
};

class TestStructModelBackend : public QIviPagingModelInterface
{
    Q_OBJECT
    Q_PROPERTY(int count READ count NOTIFY countChanged)

public:
    TestStructModelBackend(QObject* parent)
        : QIviPagingModelInterface(parent)
    {
        qRegisterMetaType<QIviPagingModelInterface*>();
    }

    void initialize() override {
        Q_EMIT initializationDone();
    }

    void registerInstance(const QUuid &identifier) override {
        Q_EMIT countChanged(identifier, m_list.count());
    }

    void unregisterInstance(const QUuid &identifier) override {
        Q_UNUSED(identifier)
    }

    void fetchData(const QUuid &identifier, int start, int count) override {
        Q_UNUSED(identifier)
        Q_UNUSED(start)
        Q_UNUSED(count)
    }

    Q_INVOKABLE void insert(int index, const TestStruct &item) {
        m_list.insert(index, item);

        Q_EMIT dataChanged(QUuid(), { QVariant::fromValue(item) }, index, 0);
        Q_EMIT countChanged(QUuid(), m_list.count());
    }

    int count() const {
        return m_list.count();
    }

private:
    QList<TestStruct> m_list;
};

class SimpleAPI: public QObject
{
    Q_OBJECT
    Q_PROPERTY(int testProperty READ testProperty WRITE setTestProperty NOTIFY testPropertyChanged)
    Q_PROPERTY(QIviPagingModelInterface *modelProperty READ modelProperty CONSTANT)

public:
    enum TestEnum {
        EnumValue0 = 0,
        EnumValue1 = 1,
        EnumValue2 = 2
    };
    Q_ENUM(TestEnum)

    int testProperty() const { return m_testProperty; }

    QIviPagingModelInterface *modelProperty() const { return m_modelProperty; }

public slots:
    void setTestProperty(int testProperty)
    {
        if (m_testProperty == testProperty)
            return;

        m_testProperty = testProperty;
        emit testPropertyChanged(testProperty);
    }

signals:
    void testPropertyChanged(int testProperty);

public:
    int m_testProperty = -1;
    QIviPagingModelInterface *m_modelProperty = new TestStructModelBackend(this);
};

class Zone: public QObject
{
    Q_OBJECT
    Q_PROPERTY(int testProperty READ testProperty WRITE setTestProperty NOTIFY testPropertyChanged)

public:
    Zone(QObject *parent = nullptr)
        : QObject(parent)
    {
    }

    int testProperty() const { return m_testProperty; }

public slots:
    void setTestProperty(int testProperty)
    {
        if (m_testProperty == testProperty)
            return;

        m_testProperty = testProperty;
        emit testPropertyChanged(testProperty);
    }

signals:
    void testPropertyChanged(int testProperty);

public:
    int m_testProperty = -1;
};

class ZonedAPI: public QObject
{
    Q_OBJECT
    Q_PROPERTY(QQmlPropertyMap *zones READ zones CONSTANT)

public:
    ZonedAPI()
    {
        m_zones->insert("leftZone", QVariant::fromValue(new Zone(this)));
    }

    QQmlPropertyMap *zones() const { return m_zones; }

public:
    QQmlPropertyMap *m_zones = new QQmlPropertyMap(this);
};

class tst_QIviSimulationGlobalObject : public QObject
{
    Q_OBJECT

    QVariant parseJson(const QString &json, QString &error) const;

private Q_SLOTS:
    void testFindData_data();
    void testFindData();
    void testConvertFromJSONErrors_data();
    void testConvertFromJSONErrors();
    void testParseDomainValue_data();
    void testParseDomainValue();
    void testInitializeDefault();
    void testCheckSettings_data();
    void testCheckSettings();
    void testConstraint_data();
    void testConstraint();
};

QVariant tst_QIviSimulationGlobalObject::parseJson(const QString &json, QString& error) const
{
    QJsonParseError pe;
    QVariantMap data = QJsonDocument::fromJson(json.toUtf8(), &pe).toVariant().toMap();
    if (pe.error != QJsonParseError::NoError)
        error = pe.errorString();

    return data;
}
void tst_QIviSimulationGlobalObject::testFindData_data()
{
    QTest::addColumn<QString>("searchString");
    QTest::addColumn<QVariantMap>("expectedResult");
    QTest::newRow("simple") << "Dummy" << QVariantMap({{"test", QVariant(true)}});
    QTest::newRow("fully-qualified") << "qt-project.ClimateControl" << QVariantMap({{"test", QVariant(false)}});
    QTest::newRow("partial match") << "org.qt-project.ClimateControl" << QVariantMap({{"test", QVariant(false)}});
    QTest::newRow("not data") << "AddressBook" << QVariantMap();
    QTest::newRow("not data complex") << "org.qt-project.AddressBook" << QVariantMap();
}

void tst_QIviSimulationGlobalObject::testFindData()
{
    QFETCH(QString, searchString);
    QFETCH(QVariantMap, expectedResult);

    QString error;
    QVariant data = parseJson("{ \"Dummy\": { \"test\": true }, \"qt-project.ClimateControl\": { \"test\": false } }", error);
    QVERIFY2(error.isEmpty(), qPrintable(error));

    QIviSimulationGlobalObject globalObject;

    QVariantMap foundData = globalObject.findData(data.toMap(), searchString);
    QCOMPARE(foundData, expectedResult);
}

void tst_QIviSimulationGlobalObject::testConvertFromJSONErrors_data()
{
    qRegisterMetaType<SimpleAPI*>();
    qRegisterMetaType<InvalidStruct2>();

    QTest::addColumn<QString>("json");
    QTest::addColumn<QStringList>("warnings");

    QTest::newRow("invalid Struct") << "{ \"type\": \"InvalidStruct\", \"value\": [ 100, true ] }" <<
                                       QStringList({"Couldn't retrieve MetaObject for struct parsing: *",
                                                    "Please make sure InvalidStruct is registered in *"});

    QTest::newRow("no fromJSON") << "{ \"type\": \"InvalidStruct2\", \"value\": [ 100, true ] }" <<
                                    QStringList("Couldn't find method: InvalidStruct2::fromJSON(QVariant)*");

    QTest::newRow("invalid Enum") << "{ \"type\": \"enum\", \"value\": \"InvalidEnum::Foo\" }" <<
                                       QStringList({"Couldn't retrieve MetaObject for enum parsing: *",
                                                    "Please make sure InvalidEnum\\* is registered in *"});

    QTest::newRow("invalid Enum Value") << "{ \"type\": \"enum\", \"value\": \"SimpleAPI::Foo\" }" <<
                                       QStringList("Couldn't parse the enum definition *");
}

void tst_QIviSimulationGlobalObject::testConvertFromJSONErrors()
{
    QFETCH(QString, json);
    QFETCH(QStringList, warnings);

    QString error;
    QVariant data = parseJson(json, error);
    QVERIFY2(error.isEmpty(), qPrintable(error));

    for (const QString &warning : warnings)
        QTest::ignoreMessage(QtWarningMsg, QRegularExpression(warning));
    QVariant result = qtivi_convertFromJSON(data);
}

void tst_QIviSimulationGlobalObject::testParseDomainValue_data()
{
    qRegisterMetaType<SimpleAPI*>();

    QTest::addColumn<QString>("json");
    QTest::addColumn<QVariant>("expectedResult");
    QTest::newRow("float") << "15.6" << QVariant(15.6);
    QTest::newRow("string") << "\"testString\"" << QVariant("testString");
    QTest::newRow("int list") << "[ 1, 2, 3]" << QVariant(QVariantList({ 1, 2, 3}));
    QTest::newRow("map") << "{ \"key1\": 1, \"key2\": \"string\" }" << QVariant(QVariantMap({ {"key1", 1}, {"key2", "string"} }));
    QTest::newRow("TestEnum") << "{ \"type\": \"enum\", \"value\": \"SimpleAPI::EnumValue1\" }"
                              << QVariant::fromValue(SimpleAPI::EnumValue1);
    QTest::newRow("TestStruct by list") << "{ \"type\": \"TestStruct\", \"value\": [ 100, true ] }"
                                        << QVariant::fromValue(TestStruct(100, true));
    QTest::newRow("TestStruct by map") << "{ \"type\": \"TestStruct\", \"value\": { \"intProperty\": 100, \"boolProperty\": true } }"
                                        << QVariant::fromValue(TestStruct(100, true));
}

void tst_QIviSimulationGlobalObject::testParseDomainValue()
{
    QFETCH(QString, json);
    QFETCH(QVariant, expectedResult);

    QIviSimulationGlobalObject globalObject;

    // Test normal unzoned data
    QString error;
    QVariant data = parseJson(QString("{ \"default\": %1 }").arg(json), error);
    QVERIFY2(error.isEmpty(), qPrintable(error));

    QVariant foundData = globalObject.parseDomainValue(data.toMap(), "default");
    QCOMPARE(foundData, expectedResult);

    // Test zoned data
    data = parseJson(QString("{\"default\": { \"leftZone\": %1, \"=\": %1 }}").arg(json), error);
    QVERIFY2(error.isEmpty(), qPrintable(error));

    // Left zone
    foundData = globalObject.parseDomainValue(data.toMap(), "default", "leftZone");
    QCOMPARE(foundData, expectedResult);

    // General zone
    foundData = globalObject.parseDomainValue(data.toMap(), "default");
    QCOMPARE(foundData, expectedResult);
}

void tst_QIviSimulationGlobalObject::testInitializeDefault()
{
    QIviSimulationGlobalObject globalObject;
    SimpleAPI simple;
    ZonedAPI zoned;

    //Simple Property
    QString error;
    QVariant data = parseJson(QString("{ \"testProperty\": { \"default\": 100 } }"), error);
    QVERIFY2(error.isEmpty(), qPrintable(error));

    QSignalSpy spy(&simple, &SimpleAPI::testPropertyChanged);
    globalObject.initializeDefault(data.toMap(), &simple);
    QVERIFY(spy.count());
    QVERIFY(spy.at(0).count());
    QCOMPARE(spy.at(0).at(0).toInt(), 100);

    //Model Property
    data = parseJson(QString("{ \"modelProperty\": { \"default\": [{ \"type\": \"TestStruct\", \"value\": [ 100, true ] }]  } }"), error);
    QVERIFY2(error.isEmpty(), qPrintable(error));

    QSignalSpy modelspy(simple.modelProperty(), &TestStructModelBackend::countChanged);
    globalObject.initializeDefault(data.toMap(), &simple);
    QVERIFY(modelspy.count());
    QCOMPARE(modelspy.at(0).count(), 2);
    QCOMPARE(modelspy.at(0).at(1).toInt(), 1);

    //Zoned Property

    // This is the simplified version which would apply the same default value to all available
    // zones within the 'zones' list
    data = parseJson(QString(" { \"zones\": [\"leftZone\"], \"testProperty\": { \"default\": 100 } }"), error);
    QVERIFY2(error.isEmpty(), qPrintable(error));

    Zone *leftZone = zoned.zones()->value("leftZone").value<Zone*>();
    QVERIFY(leftZone);

    QSignalSpy zoneSpy(leftZone, &Zone::testPropertyChanged);
    globalObject.initializeDefault(data.toMap(), &zoned);
    QVERIFY(zoneSpy.count());
    QVERIFY(zoneSpy.at(0).count());
    QCOMPARE(zoneSpy.at(0).at(0).toInt(), 100);
}

void tst_QIviSimulationGlobalObject::testCheckSettings_data()
{
    QTest::addColumn<QString>("json");
    QTest::addColumn<QVariant>("value");
    QTest::addColumn<bool>("expectedResult");
    QTest::newRow("unsupported") << "{ \"unsupported\": true }" << QVariant(1) << false;
    QTest::newRow("unsupported false") << "{ \"unsupported\": false }" << QVariant(1) << true;
    QTest::newRow("minDomain") << "{ \"minimum\": 10 }" << QVariant(11) << true;
    QTest::newRow("minDomain false") << "{ \"minimum\": 10 }" << QVariant(1) << false;
    QTest::newRow("maxDomain") << "{ \"maximum\": 10 }" << QVariant(10) << true;
    QTest::newRow("maxDomain false") << "{ \"maximum\": 10 }" << QVariant(11) << false;
    QTest::newRow("range") << "{ \"range\": [0, 10] }" << QVariant(5) << true;
    QTest::newRow("range low") << "{ \"range\": [0, 10] }" << QVariant(-5) << false;
    QTest::newRow("range high") << "{ \"range\": [0, 10] }" << QVariant(15) << false;
    QTest::newRow("min, max") << "{ \"minimum\": 0, \"maximum\": 10 }" << QVariant(5) << true;
    QTest::newRow("min, max low") << "{ \"minimum\": 0, \"maximum\": 10 }" << QVariant(-5) << false;
    QTest::newRow("min, max high") << "{ \"minimum\": 0, \"maximum\": 10 }" << QVariant(15) << false;
    QTest::newRow("domain") << "{ \"domain\": [\"string1\", \"string2\"] }" << QVariant("string1") << true;
    QTest::newRow("domain false") << "{ \"domain\": [\"string1\", \"string2\"] }" << QVariant("invalid") << false;
}

void tst_QIviSimulationGlobalObject::testCheckSettings()
{
    QFETCH(QString, json);
    QFETCH(QVariant, value);
    QFETCH(bool, expectedResult);

    QIviSimulationGlobalObject globalObject;

    QString error;
    QVariant data = parseJson(json, error);
    QVERIFY2(error.isEmpty(), qPrintable(error));

    bool result = globalObject.checkSettings(data.toMap(), value);
    QCOMPARE(result, expectedResult);
}

void tst_QIviSimulationGlobalObject::testConstraint_data()
{
    QTest::addColumn<QString>("json");
    QTest::addColumn<QString>("expectedResult");
    QTest::newRow("unsupported") << "{ \"unsupported\": true }" << "unsupported";
    QTest::newRow("minDomain") << "{ \"minimum\": 10 }" << ">= 10";
    QTest::newRow("maxDomain") << "{ \"maximum\": 10 }" << "<= 10";
    QTest::newRow("range") << "{ \"range\": [0, 10] }" << "[0-10]";
    QTest::newRow("min, max") << "{ \"minimum\": 0, \"maximum\": 10 }" << "[0-10]";
    QTest::newRow("domain") << "{ \"domain\": [\"string1\", \"string2\"] }" << "[\"string1\",\"string2\"]";
}

void tst_QIviSimulationGlobalObject::testConstraint()
{
    QFETCH(QString, json);
    QFETCH(QString, expectedResult);

    QIviSimulationGlobalObject globalObject;

    QString error;
    QVariant data = parseJson(json, error);
    QVERIFY2(error.isEmpty(), qPrintable(error));

    QString result = globalObject.constraint(data.toMap());
    QCOMPARE(result, expectedResult);
}

QTEST_MAIN(tst_QIviSimulationGlobalObject)

#include "tst_qivisimulationglobalobject.moc"

