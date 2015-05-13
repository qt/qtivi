/************************************************************************************************
 * Copyright (c) 2012-2015 Pelagicore AG. All rights reserved.
 *
 * This software, including documentation, is protected by copyright controlled by Pelagicore AG.
 * All rights reserved. Copying, including reproducing, storing, adapting or translating, any or
 * all of this material requires prior written consent of Pelagicore AG Corporation. This material
 * also contains confidential information which may not be disclosed to others without the prior
 * written consent of Pelagicore AG.
 ************************************************************************************************/

#ifndef QTIVIABSTRACTFEATURE_H
#define QTIVIABSTRACTFEATURE_H

#include <QObject>
#include <QQmlParserStatus>

#include <QtIVICore/qtiviglobal.h>

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
    Q_PROPERTY(bool isValid READ isValid NOTIFY isValidChanged)

public:

    explicit QtIVIAbstractFeature(const QString &interface, bool autoDiscovery = true, QObject *parent = 0);
    virtual ~QtIVIAbstractFeature();

    QtIVIServiceObject *serviceObject() const;
    bool autoDiscovery() const;
    bool isValid() const;

    void startAutoDiscovery();

public slots:
    void setServiceObject(QtIVIServiceObject *so);
    void setAutoDiscovery(bool autoDiscovery);

signals:
    void serviceObjectChanged();
    void autoDiscoveryChanged(bool autoDiscovery);
    void isValidChanged(bool arg);

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
