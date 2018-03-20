TARGET = tst_basic
QMAKE_PROJECT_NAME = $$TARGET

QT += testlib core ivicore ivivehiclefunctions
CONFIG += c++11 ivigenerator testcase

QFACE_FORMAT = test
QFACE_SOURCES += ../../../../src/ivivehiclefunctions/ivivehiclefunctions.qface
