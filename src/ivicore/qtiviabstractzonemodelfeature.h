#ifndef QTIVIABSTRACTZONEMODELFEATURE_H
#define QTIVIABSTRACTZONEMODELFEATURE_H

#include <QtIVIAbstractFeature>

#include <QtIVICore/qtiviglobal.h>

class QAbstractItemModel;

class Q_QTIVICORE_EXPORT QtIVIAbstractZoneModelFeature : public QtIVIAbstractFeature
{
    Q_OBJECT

    Q_PROPERTY(QAbstractItemModel* model READ model CONSTANT)

public:
    QtIVIAbstractZoneModelFeature(const QString &interface, bool autoDiscovery = true, QObject *parent = 0);

    virtual QStringList zones() const = 0;
    virtual QObject *zoneObject(const QString &z) const = 0;

    QAbstractItemModel *model();

private:
    QAbstractItemModel *m_model;
};

#endif // QTIVIABSTRACTZONEMODELFEATURE_H
