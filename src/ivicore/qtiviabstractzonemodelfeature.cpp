#include "qtiviabstractzonemodelfeature.h"

QtIVIAbstractZoneModelFeature::QtIVIAbstractZoneModelFeature(const QString &interface, bool autoDiscovery, QObject *parent)
    : QtIVIAbstractFeature(interface, autoDiscovery, parent)
{
}

QAbstractItemModel *QtIVIAbstractZoneModelFeature::model()
{
    return 0;
}
