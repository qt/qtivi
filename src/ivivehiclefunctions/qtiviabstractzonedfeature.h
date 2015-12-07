/****************************************************************************
**
** Copyright (C) 2015 Pelagicore AG
** Contact: http://www.qt.io/ or http://www.pelagicore.com/
**
** This file is part of the QtIVI module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL3-PELAGICORE$
** Commercial License Usage
** Licensees holding valid commercial Qt IVI licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Pelagicore. For licensing terms
** and conditions, contact us at http://www.pelagicore.com.
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
** SPDX-License-Identifier: LGPL-3.0
**
****************************************************************************/

#ifndef QTIVIABSTRACTZONEDFEATURE_H
#define QTIVIABSTRACTZONEDFEATURE_H

#include <QtIVICore/QtIVIAbstractFeature>
#include <QtIVIVehicleFunctions/qtivivehiclefunctionsglobal.h>
#include <QVariant>
#include <QMap>

QT_BEGIN_NAMESPACE

class QtIVIVehicleBackendInterface;

class Q_QTIVIVEHICLEFUNCTIONS_EXPORT QtIVIAbstractZonedFeature : public QtIVIAbstractFeature
{
    Q_OBJECT

    Q_PROPERTY(QString zone READ zone WRITE setZone NOTIFY zoneChanged)
    Q_PROPERTY(QStringList availableZones READ availableZones NOTIFY availableZonesChanged)
    Q_PROPERTY(QVariantList zones READ zoneFeatureList NOTIFY zonesChanged)
    Q_PROPERTY(QVariantMap zoneAt READ zoneFeatureMap  NOTIFY zonesChanged)
    Q_PROPERTY(QString error READ errorMessage NOTIFY errorChanged)
    Q_ENUMS(Error)

public:

    enum Error {
        NoError,
        PermissionDenied,
        InvalidOperation,
        Timeout,
        InvalidZone,
        Unknown
    };

    explicit QtIVIAbstractZonedFeature(const QString &interface, const QString &zone = QString(), bool autoDiscovery = true, QObject *parent = 0);
    virtual ~QtIVIAbstractZonedFeature();

    QString zone() const;

    QStringList availableZones() const;

    QtIVIAbstractZonedFeature *zoneAt(const QString &zone) const;
    QList<QtIVIAbstractZonedFeature*> zones() const;

    QtIVIAbstractZonedFeature::Error error() const;
    QString errorMessage() const;

Q_SIGNALS:
    void availableZonesChanged(QStringList zones);
    void zoneChanged();
    void zonesChanged();
    void errorChanged(QtIVIAbstractZonedFeature::Error error, const QString &message);

protected:
    virtual QtIVIAbstractZonedFeature *createZoneFeature(const QString &zone) = 0;
    void setError(QtIVIAbstractZonedFeature::Error error, const QString &message = QString());
    QtIVIVehicleBackendInterface *backend() const;

    virtual void connectToServiceObject(QtIVIServiceObject *serviceObject);
    virtual void disconnectFromServiceObject(QtIVIServiceObject *serviceObject);
    virtual void clearServiceObject();

protected Q_SLOTS:
    virtual void onErrorChanged(QtIVIAbstractZonedFeature::Error error, const QString &message = QString());

private Q_SLOTS:
    void setZone(const QString &zone);

private:
    virtual bool acceptServiceObject(QtIVIServiceObject *serviceObject);

    void initializeZones();

    QVariantList attributeList() const;
    QVariantMap attributeMap() const;
    QVariantMap zoneFeatureMap() const;
    QVariantList zoneFeatureList() const;

    void resetAttributes();

    QString errorText() const;

private:
    QString m_zone;
    QList<QtIVIAbstractZonedFeature*> m_zoneFeatures;
    QVariantMap m_zoneFeatureMap;
    QVariantList m_zoneFeatureList;
    QString m_errorMessage;
    QtIVIAbstractZonedFeature::Error m_error;
};

QT_END_NAMESPACE

#endif // QTIVIABSTRACTZONEDFEATURE_H
