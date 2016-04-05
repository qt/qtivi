# Qt Genivi Extras: Platform specific components for GENIVI

This repository is similar to qtwinextras or qtmacextras and contains specific components for GENIVI based Linux systems.

Currently the only component it contains is a Qt Categorized Logging Binding towards GENIVI DLT (Diagnostic Log and Trace)

## How to build

	qmake
	make
	make install
	
	
## Depedencies

This module has a dependency towards **dlt-daemon** (http://projects.genivi.org/diagnostic-log-trace/about)

The dlt-daemon needs to be installed into the system and needs to be detectable by pkg-config.

## Example Usage

As QDlt is a binding for Qt Categorized Logging you first need to create you logging categories in the header:

	#include <QLoggingCategory>

	Q_DECLARE_LOGGING_CATEGORY(FOO)
	Q_DECLARE_LOGGING_CATEGORY(BAR)

In the source file you need to define your logging categories, but instead of using the normal Q_LOGGING_CATEGORY() macro you can use QDLT_LOGGING_CATEGORY() instead.

	#include <QtGeniviExtras/QtDlt>

	QDLT_REGISTER_APPLICATION("APP1", "Description for APP")

	QDLT_LOGGING_CATEGORY(FOO, "com.pelagicore.foo", "FOO", "FOO CATEGORY")
	QDLT_LOGGING_CATEGORY(BAR, "com.pelagicore.bar", "BAR", "BAR CATEGORY")

	QDLT_FALLBACK_CATEGORY(FOO)

As DLT works for the whole system it needs to know also a little bit more about your application, which can be done by using QDLT_REGISTER_APPLICATION()

DLT is using 4 character based idenifiers, this identifier needs to be assigned to your application, as well as to every category you define.

By using QDLT_FALLBACK_CATEGORY() you can define which DLT category is used when no Qt Category was set for loogging a message e.g. when qWarning(), qDebug() or qCritical() was used
