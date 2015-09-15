/****************************************************************************
**
** Copyright (C) 2015 Pelagicore AG
** Contact: http://www.pelagicore.com/
**
** This file is part of Qt IVI.
**
** $QT_BEGIN_LICENSE:LGPL3$
** Commercial License Usage
** Licensees holding valid commercial Qt IVI licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Pelagicore. For licensing terms
** and conditions see http://www.pelagicore.com.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPLv3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

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
