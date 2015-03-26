#include <QtTest>
#include "qtiviservicemanager.h"
#include "qtiviclimatecontrol.h"
#include "qtiviclimatecontrolbackendinterface.h"

class ClimateControlTest : public QObject
{
    Q_OBJECT

public:
    ClimateControlTest();

private slots:
    void cleanup();

    void testNoBackend();

private:
    QtIVIServiceManager *manager;
};

ClimateControlTest::ClimateControlTest()
    : QObject()
{
    manager = QtIVIServiceManager::instance();
}

void ClimateControlTest::cleanup()
{
    manager->unloadAllBackends();
}

void ClimateControlTest::testNoBackend()
{
    QtIVIClimateControl cc;
    QVERIFY(cc.driver() != 0);
}

QTEST_APPLESS_MAIN(ClimateControlTest)

#include "tst_climatecontroltest.moc"
