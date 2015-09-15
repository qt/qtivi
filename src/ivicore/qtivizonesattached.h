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

#ifndef QTIVIZONESATTACHED_H
#define QTIVIZONESATTACHED_H

#include <QObject>
#include <QtQml>

#include <QtIVICore/qtiviglobal.h>

class QtIVIAbstractZoneModelFeature;

class Q_QTIVICORE_EXPORT QtIVIZonesAttached : public QObject
{
// Start of Q_OBJECT macro
public:
    Q_OBJECT_CHECK
    QT_WARNING_PUSH
    Q_OBJECT_NO_OVERRIDE_WARNING
    static const QMetaObject staticMetaObject;
    virtual const QMetaObject *metaObject() const;
    virtual void *qt_metacast(const char *);
    virtual int qt_metacall(QMetaObject::Call, int, void **);
    QT_WARNING_POP
    QT_TR_FUNCTIONS
private: \
    Q_DECL_HIDDEN_STATIC_METACALL static void qt_static_metacall(QObject *, QMetaObject::Call, int, void **);
    struct QPrivateSignal {};
// End of Q_OBJECT macro
    virtual int qt_metacall_generated(QMetaObject::Call, int, void **);

public:
    explicit QtIVIZonesAttached(QObject *parent = 0);
    ~QtIVIZonesAttached();

    static QtIVIZonesAttached *qmlAttachedProperties(QObject*);

private slots:
    void onModelChanged();

private:
    QMetaObject *buildMetaObject(QtIVIAbstractZoneModelFeature*);

    QtIVIAbstractZoneModelFeature *m_item;

    QMetaObject *m_metaObject;
};

QML_DECLARE_TYPEINFO(QtIVIZonesAttached, QML_HAS_ATTACHED_PROPERTIES)

#endif // QTIVIZONESATTACHED_H
