/****************************************************************************
**
** Copyright (C) 2016 Pelagicore AG
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtIVI module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL-QTAS$
** Commercial License Usage
** Licensees holding valid commercial Qt Automotive Suite licenses may use
** this file in accordance with the commercial license agreement provided
** with the Software or, alternatively, in accordance with the terms
** contained in a written agreement between you and The Qt Company.  For
** licensing terms and conditions see https://www.qt.io/terms-conditions.
** For further information use the contact form at https://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl-3.0.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 2.0 or (at your option) the GNU General
** Public license version 3 or any later version approved by the KDE Free
** Qt Foundation. The licenses are as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL2 and LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-2.0.html and
** https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
** SPDX-License-Identifier: LGPL-3.0
**
****************************************************************************/

#ifndef QTIVIABSTRACTZONEDFEATURE_H
#define QTIVIABSTRACTZONEDFEATURE_H

#include <QtCore/QVariant>
#include <QtCore/QMap>
#include <QtIVICore/QtIVIAbstractFeature>
#include <QtIVICore/qtiviglobal.h>

QT_BEGIN_NAMESPACE

class QtIVIZonedFeatureInterface;
class QtIVIAbstractZonedFeaturePrivate;

class Q_QTIVICORE_EXPORT QtIVIAbstractZonedFeature : public QtIVIAbstractFeature
{
    Q_OBJECT

    Q_PROPERTY(QString zone READ zone WRITE setZone NOTIFY zoneChanged)
    Q_PROPERTY(QStringList availableZones READ availableZones NOTIFY availableZonesChanged)
    Q_PROPERTY(QVariantList zones READ zoneFeatureList NOTIFY zonesChanged)
    Q_PROPERTY(QVariantMap zoneAt READ zoneFeatureMap  NOTIFY zonesChanged)

public:

    explicit QtIVIAbstractZonedFeature(const QString &interface, const QString &zone = QString(), QObject *parent = 0);
    virtual ~QtIVIAbstractZonedFeature();

    QString zone() const;

    QStringList availableZones() const;

    QtIVIAbstractZonedFeature *zoneAt(const QString &zone) const;
    QList<QtIVIAbstractZonedFeature*> zones() const;

Q_SIGNALS:
    void availableZonesChanged(QStringList zones);
    void zoneChanged();
    void zonesChanged();

protected:
    QtIVIAbstractZonedFeature(QtIVIAbstractZonedFeaturePrivate &dd, QObject *parent = Q_NULLPTR);

    virtual QtIVIAbstractZonedFeature *createZoneFeature(const QString &zone) = 0;
    QtIVIZonedFeatureInterface *backend(const QString &interface = QString()) const;

    virtual bool acceptServiceObject(QtIVIServiceObject *serviceObject);
    virtual void connectToServiceObject(QtIVIServiceObject *serviceObject);
    virtual void clearServiceObject();

private Q_SLOTS:
    void setZone(const QString &zone);

private:
    void initializeZones();

    QVariantList attributeList() const;
    QVariantMap attributeMap() const;
    QVariantMap zoneFeatureMap() const;
    QVariantList zoneFeatureList() const;

    void resetAttributes();

private:
    Q_DECLARE_PRIVATE(QtIVIAbstractZonedFeature)
};

QT_END_NAMESPACE

#endif // QTIVIABSTRACTZONEDFEATURE_H
