TARGET = tst_org-example-echo-simulator
QMAKE_PROJECT_NAME = $$TARGET
DESTDIR = ../

QT += testlib core ivicore
CONFIG += c++11 ivigenerator testcase

INCLUDEPATH += $$OUT_PWD/../frontend
LIBS += -L$$OUT_PWD/.. -l$$qtLibraryTarget(echo_simulator_frontend)

QMAKE_RPATHDIR += $$OUT_PWD/..

QFACE_FORMAT = test
QFACE_SOURCES = ../../../org.example.echo.simulator.qface
