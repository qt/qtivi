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
