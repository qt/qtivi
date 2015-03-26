#include <QtQml/qqmlextensionplugin.h>
#include <qqml.h>

#include <QtIVIClimateControl>

QT_BEGIN_NAMESPACE

class QtIVIVehicleFunctionsPlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QQmlExtensionInterface/1.0")
public:
    virtual void registerTypes(const char *uri)
    {
        Q_ASSERT(QLatin1String(uri) == QLatin1String("QtIVIVehicleFunctions"));
        Q_UNUSED(uri);

        qmlRegisterType<QtIVIClimateControl>(uri, 1, 0, "ClimateControl");
        qmlRegisterUncreatableType<QtIVIClimateZone>(uri, 1, 0, "ClimateZone", "ClimateZone only accessible from ClimateControl");
    }
};

QT_END_NAMESPACE

#include "plugin.moc"
