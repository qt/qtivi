#ifndef QTIVIABSTRACTFEATURE_H
#define QTIVIABSTRACTFEATURE_H

#include <QObject>
#include <QQmlParserStatus>

#include <qtiviglobal.h>

class QtIVIServiceObject;

class Q_QTIVICORE_EXPORT QtIVIAbstractFeature : public QObject, public QQmlParserStatus
{
    Q_OBJECT
    Q_INTERFACES(QQmlParserStatus)

    //This makes only sense if the Object is loaded directly
    //Maybe use a attached property instead ??? Feature.simulation = true; Feature.autoDiscovery = false
    //Q_PROPERTY(bool simulation READ simulation WRITE setSimulation NOTIFY simulationChanged)
    Q_PROPERTY(bool autoDiscovery READ autoDiscovery WRITE setAutoDiscovery NOTIFY autoDiscoveryChanged)
    Q_PROPERTY(QtIVIServiceObject* serviceObject READ serviceObject WRITE setServiceObject NOTIFY serviceObjectChanged)

public:

    explicit QtIVIAbstractFeature(const QString &interface, bool autoDiscovery = true, QObject *parent = 0);
    virtual ~QtIVIAbstractFeature();

    QtIVIServiceObject *serviceObject() const;
    bool autoDiscovery() const;

public slots:
    void setServiceObject(QtIVIServiceObject *so);
    void setAutoDiscovery(bool autoDiscovery);

signals:
    void serviceObjectChanged();
    void autoDiscoveryChanged(bool autoDiscovery);

protected:

    //USE acceptService instead of the interface constructor, also in the simplediscoverymodel
    virtual bool acceptServiceObject(QtIVIServiceObject*) = 0;
    virtual void connectToServiceObject(QtIVIServiceObject*) = 0;
    virtual void disconnectFromServiceObject(QtIVIServiceObject*) = 0;
    virtual void clearServiceObject() = 0;

    //TODO This doesn't work for the C++ usecases we should use the constructor there instead
    // Also this means a qml dependency in the core, do we want that ?
    virtual void classBegin();
    virtual void componentComplete();

private slots:
    void serviceObjectDestroyed();

private:

    QString m_interface;
    QtIVIServiceObject* m_serviceObject;
    bool m_autoDiscovery;
};

#endif // QTIVIABSTRACTFEATURE_H
