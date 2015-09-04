#include "qtiviabstractzonemodelfeature.h"

#include <QAbstractItemModel>

class QtIVIZoneModel : public QAbstractListModel
{
public:
    QtIVIZoneModel(QtIVIAbstractZoneModelFeature *parent);

    int rowCount(const QModelIndex &) const;
    QVariant data(const QModelIndex &, int) const;

    QHash<int, QByteArray> roleNames() const;

private:
    QtIVIAbstractZoneModelFeature *m_feature;
};

QtIVIZoneModel::QtIVIZoneModel(QtIVIAbstractZoneModelFeature *parent)
    : QAbstractListModel(parent)
    , m_feature(parent)
{
    connect(m_feature, SIGNAL(isValidChanged(bool)), this, SIGNAL(modelReset()));
}

int QtIVIZoneModel::rowCount(const QModelIndex &index) const
{
    int res = 0;

    if (!index.isValid())
        res = m_feature->zones().count();

    return res;
}

QVariant QtIVIZoneModel::data(const QModelIndex &index, int role) const
{
    QVariant res;

    if (index.isValid() &&
            !index.parent().isValid() &&
            index.column() == 0 &&
            index.row() >= 0 &&
            index.row() < m_feature->zones().count()) {
        switch(role)
        {
        case Qt::DisplayRole:
            res = m_feature->zones().at(index.row());
            break;
        case Qt::UserRole:
            res = QVariant::fromValue<QObject*>(m_feature->zoneObject(m_feature->zones().at(index.row())));
            break;
        }
    }

    return res;
}

QHash<int, QByteArray> QtIVIZoneModel::roleNames() const
{
    QHash<int, QByteArray> res;

    res[Qt::DisplayRole] = "zoneName";
    res[Qt::UserRole] = "zoneObject";

    return res;
}

QtIVIAbstractZoneModelFeature::QtIVIAbstractZoneModelFeature(const QString &interface, bool autoDiscovery, QObject *parent)
    : QtIVIAbstractFeature(interface, autoDiscovery, parent)
{
    m_model = new QtIVIZoneModel(this);
}

QAbstractItemModel *QtIVIAbstractZoneModelFeature::model()
{
    return m_model;
}
