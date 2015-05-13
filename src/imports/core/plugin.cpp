/************************************************************************************************
 * Copyright (c) 2012-2015 Pelagicore AG. All rights reserved.
 *
 * This software, including documentation, is protected by copyright controlled by Pelagicore AG.
 * All rights reserved. Copying, including reproducing, storing, adapting or translating, any or
 * all of this material requires prior written consent of Pelagicore AG Corporation. This material
 * also contains confidential information which may not be disclosed to others without the prior
 * written consent of Pelagicore AG.
 ************************************************************************************************/

#include <QtQml/qqmlextensionplugin.h>
#include <qqml.h>

#include <QtIVICore/QtIVICore>

QT_BEGIN_NAMESPACE

class QtIVICorePlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QQmlExtensionInterface/1.0")
public:
    virtual void registerTypes(const char *uri)
    {
        Q_ASSERT(QLatin1String(uri) == QLatin1String("QtIVICore"));
        Q_UNUSED(uri);

        qmlRegisterUncreatableType<QtIVIServiceObject>(uri, 1, 0, "QtIVIServiceObject", "QtIVIServiceObject is not accessible directly");
    }
};

QT_END_NAMESPACE

#include "plugin.moc"
