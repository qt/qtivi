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

#include "qtiviabstractzonemodelfeature.h"

#include <QAbstractItemModel>

class QtIVIZoneModel : public QAbstractListModel
{
public:
    QtIVIZoneModel(QtIVIAbstractZoneModelFeature *parent);

    int rowCount(const QModelIndex &) const;
    QVariant data(const QModelIndex &, int) const;

    QHash<int, QByteArray> roleNames() const;

    void updateModel();

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

void QtIVIZoneModel::updateModel()
{
    beginResetModel();
    // TODO this needs to be fixed. Right now the model is based on the contents of the backend
    // and if the backend changes, we really do not know anything of the previous state, so the
    // about-signal is misleading.
    endResetModel();
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

void QtIVIAbstractZoneModelFeature::zonesChanged()
{
    m_model->updateModel();
}
