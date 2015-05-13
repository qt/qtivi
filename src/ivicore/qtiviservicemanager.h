/************************************************************************************************
 * Copyright (c) 2012-2015 Pelagicore AG. All rights reserved.
 *
 * This software, including documentation, is protected by copyright controlled by Pelagicore AG.
 * All rights reserved. Copying, including reproducing, storing, adapting or translating, any or
 * all of this material requires prior written consent of Pelagicore AG Corporation. This material
 * also contains confidential information which may not be disclosed to others without the prior
 * written consent of Pelagicore AG.
 ************************************************************************************************/

#ifndef QTIVISERVICEMANAGER_H
#define QTIVISERVICEMANAGER_H

#include <QtCore/QAbstractListModel>

#include "qtiviglobal.h"

class QtIVIServiceObject;
class QtIVIServiceManagerPrivate;

//TODO Detect simulation plugins
//TODO make it possible to only search there e.g. by a SearchFlag BitField
class Q_QTIVICORE_EXPORT QtIVIServiceManager : public QAbstractListModel
{
    Q_OBJECT
public:
    static QtIVIServiceManager *instance();
    virtual ~QtIVIServiceManager();

    QList<QtIVIServiceObject*> findServiceByInterface(const QString &interface);
    bool hasInterface(const QString &interface) const;

    bool registerService(QObject *serviceBackendInterface, const QStringList &interfaces);
    void unloadAllBackends();

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role) const;
signals:

private:
    explicit QtIVIServiceManager();
    QtIVIServiceManagerPrivate * const d_ptr;
    Q_DECLARE_PRIVATE(QtIVIServiceManager)
};

#endif // QTIVISERVICEMANAGER_H
