#include <QtQml/qqmlextensionplugin.h>
#include <qqml.h>

#include <QtIVICore>

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
