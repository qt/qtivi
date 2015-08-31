#ifndef QTIVIABSTRACTZONEMODELFEATURE_H
#define QTIVIABSTRACTZONEMODELFEATURE_H

#include <QtIVIAbstractFeature>

class QAbstractItemModel;

class QtIVIAbstractZoneModelFeature : public QtIVIAbstractFeature
{
    Q_OBJECT

    Q_PROPERTY(QAbstractItemModel* model READ model CONSTANT)

public:
    QtIVIAbstractZoneModelFeature(const QString &interface, bool autoDiscovery = true, QObject *parent = 0);

    QAbstractItemModel *model();

private:
    QAbstractItemModel *m_model;
};

#endif // QTIVIABSTRACTZONEMODELFEATURE_H
