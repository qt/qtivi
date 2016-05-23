QT       += core geniviextras
QT       -= gui

TARGET = qdlt
CONFIG   += console
CONFIG   -= app_bundle
CONFIG += c++11

TEMPLATE = app

SOURCES += main.cpp \
    loggingcategories.cpp \

HEADERS += \
    loggingcategories.h \

# install
target.path = $$[QT_INSTALL_EXAMPLES]/geniviextras/qdlt
INSTALLS += target
