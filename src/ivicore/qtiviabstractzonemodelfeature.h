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

#ifndef QTIVIABSTRACTZONEMODELFEATURE_H
#define QTIVIABSTRACTZONEMODELFEATURE_H

#include <QtIVIAbstractFeature>

#include <QtIVICore/qtiviglobal.h>

class QAbstractItemModel;

class QtIVIZoneModel;

class Q_QTIVICORE_EXPORT QtIVIAbstractZoneModelFeature : public QtIVIAbstractFeature
{
    Q_OBJECT

    Q_PROPERTY(QAbstractItemModel* model READ model CONSTANT)

public:
    QtIVIAbstractZoneModelFeature(const QString &interface, bool autoDiscovery = true, QObject *parent = 0);

    virtual QStringList zones() const = 0;
    virtual QObject *zoneObject(const QString &z) const = 0;

    QAbstractItemModel *model();

protected:
    void zonesChanged();

private:
    QtIVIZoneModel *m_model;
};

#endif // QTIVIABSTRACTZONEMODELFEATURE_H
