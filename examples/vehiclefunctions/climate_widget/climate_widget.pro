QT       += core gui ivicore ivivehiclefunctions

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = climate_widget
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui
