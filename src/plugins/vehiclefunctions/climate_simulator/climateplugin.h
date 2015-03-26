#ifndef CLIMATEPLUGIN_H
#define CLIMATEPLUGIN_H

#include <QtIVICore/QtIVIServiceInterface>

class ClimateControlBackend;

class ClimatePlugin : public QObject, QtIVIServiceInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "com.pelagicore.QtIVIServiceInterface" FILE "climate_simulator.json")
    Q_INTERFACES(QtIVIServiceInterface)

public:
    ClimatePlugin(QObject *parent = 0);

    QStringList interfaces() const;
    QObject* interfaceInstance(const QString& interface) const;

private:
    ClimateControlBackend* m_climate;
};

#endif // CLIMATEPLUGIN_H
