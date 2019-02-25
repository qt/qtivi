/****************************************************************************
**
** Copyright (C) 2019 Luxoft Sweden AB
** Copyright (C) 2018 Pelagicore AG
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtIvi module of the Qt Toolkit.
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

#ifndef QIVIABSTRACTZONEDFEATURE_H
#define QIVIABSTRACTZONEDFEATURE_H

#include <QtCore/QMap>
#include <QtCore/QVariant>
#include <QtIviCore/QIviAbstractFeature>
#include <QtIviCore/qtiviglobal.h>

QT_BEGIN_NAMESPACE

class QIviZonedFeatureInterface;
class QIviAbstractZonedFeaturePrivate;

class Q_QTIVICORE_EXPORT QIviAbstractZonedFeature : public QIviAbstractFeature
{
    Q_OBJECT

    Q_PROPERTY(QString zone READ zone WRITE setZone NOTIFY zoneChanged)
    Q_PROPERTY(QStringList availableZones READ availableZones NOTIFY availableZonesChanged)
    Q_PROPERTY(QVariantList zones READ zoneFeatureList NOTIFY zonesChanged)
    Q_PROPERTY(QVariantMap zoneAt READ zoneFeatureMap  NOTIFY zonesChanged)

public:

    explicit QIviAbstractZonedFeature(const QString &interface, const QString &zone = QString(), QObject *parent = nullptr);

    QString zone() const;

    QStringList availableZones() const;

    QIviAbstractZonedFeature *zoneAt(const QString &zone) const;
    QList<QIviAbstractZonedFeature*> zones() const;

Q_SIGNALS:
    void availableZonesChanged(const QStringList &zones);
    void zoneChanged();
    void zonesChanged();

protected:
    QIviAbstractZonedFeature(QIviAbstractZonedFeaturePrivate &dd, QObject *parent = nullptr);

    virtual QIviAbstractZonedFeature *createZoneFeature(const QString &zone) = 0;
    QIviZonedFeatureInterface *backend(const QString &interface = QString()) const;

    bool acceptServiceObject(QIviServiceObject *serviceObject) override;
    void connectToServiceObject(QIviServiceObject *serviceObject) override;
    void clearServiceObject() override;

private Q_SLOTS:
    void setZone(const QString &zone);
    void initializeZones(const QStringList &zones);

private:
    QVariantList attributeList() const;
    QVariantMap attributeMap() const;
    QVariantMap zoneFeatureMap() const;
    QVariantList zoneFeatureList() const;

    void resetAttributes();

private:
    Q_DECLARE_PRIVATE(QIviAbstractZonedFeature)
};

QT_END_NAMESPACE

#endif // QIVIABSTRACTZONEDFEATURE_H
