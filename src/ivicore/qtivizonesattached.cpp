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

#include "qtivizonesattached.h"

#include <QtCore/private/qmetaobjectbuilder_p.h>

#include <qtiviabstractzonemodelfeature.h>

QtIVIZonesAttached::QtIVIZonesAttached(QObject *parent) : QObject(parent)
{
    m_item = qobject_cast<QtIVIAbstractZoneModelFeature*>(parent);
    if (m_item && m_item->model())
    {
        m_item->startAutoDiscovery(); // TODO we probably do not want this here, as it is a potentially costly, blocking, call

        // Ensure that the model does not change
        connect(m_item->model(), &QAbstractItemModel::layoutChanged, this, &QtIVIZonesAttached::onModelChanged);
        connect(m_item->model(), &QAbstractItemModel::rowsInserted, this, &QtIVIZonesAttached::onModelChanged);
        connect(m_item->model(), &QAbstractItemModel::rowsRemoved, this, &QtIVIZonesAttached::onModelChanged);
        connect(m_item->model(), &QAbstractItemModel::dataChanged, this, &QtIVIZonesAttached::onModelChanged);
        connect(m_item->model(), &QAbstractItemModel::modelReset, this, &QtIVIZonesAttached::onModelChanged);
    }
    else
        qWarning() << "Could not attach Zones property to :" << parent << " is not a QtIVIAbstractZoneModelFeature";

    m_metaObject = buildMetaObject(m_item);
}

QtIVIZonesAttached::~QtIVIZonesAttached()
{
    free(m_metaObject);
    m_metaObject = 0;
}

QtIVIZonesAttached *QtIVIZonesAttached::qmlAttachedProperties(QObject *object)
{
    return new QtIVIZonesAttached(object);
}

void QtIVIZonesAttached::onModelChanged()
{
    qWarning() << "Attached Zones property does not support dynamically modifying models.";
}

QMetaObject *QtIVIZonesAttached::buildMetaObject(QtIVIAbstractZoneModelFeature *zoneModel)
{
    QMetaObjectBuilder builder;

    builder.setSuperClass(&QObject::staticMetaObject);
    builder.setClassName("QtIVIZonesAttached");
    builder.setFlags(QMetaObjectBuilder::DynamicMetaObject);

    QRegExp reProperty(QStringLiteral("^[a-z_][A-Za-z0-9_]*$"));

    if (zoneModel && zoneModel->model()) {
        int j = 0;
        QStringList names;
        for(int i=0; i<zoneModel->model()->rowCount(); ++i)
        {
            QModelIndex index = zoneModel->model()->index(i, 0);
            QString name = index.data(Qt::DisplayRole).toString();

            // Ensure that name is a valid, unique property name
            if (reProperty.indexIn(name) == -1 || names.contains(name))
                while (names.contains(name))
                    name = QStringLiteral("zone%1").arg(j++);

            QMetaPropertyBuilder pbuilder = builder.addProperty(name.toUtf8(), "QObject*", i);
            pbuilder.setReadable(true);
            pbuilder.setWritable(false);
            pbuilder.setConstant(true);

            names << name;
        }
    }

    return builder.toMetaObject();
}

const QMetaObject *QtIVIZonesAttached::metaObject() const { return m_metaObject; }

int QtIVIZonesAttached::qt_metacall(QMetaObject::Call c, int _id, void **argv)
{
    int id = QObject::qt_metacall(c, _id, argv);
    if (id < 0)
        return id;

    switch(c)
    {
    case QMetaObject::ReadProperty:
        if (id < m_item->model()->rowCount())
        {
            QObject *object = m_item->model()->index(id, 0).data(Qt::UserRole).value<QObject*>();
            QMetaType::construct(QMetaType::QObjectStar, argv[0], &object);
            return -1;
        }
        else
            return id-m_item->model()->rowCount();

    default: // We only support read-only properties
        break;
    }

    return qt_metacall_generated(c, _id, argv);
}




// Start of moc generated code

#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'qtivizonesattached.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_QtIVIZonesAttached_t {
    QByteArrayData data[3];
    char stringdata0[35];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_QtIVIZonesAttached_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_QtIVIZonesAttached_t qt_meta_stringdata_QtIVIZonesAttached = {
    {
QT_MOC_LITERAL(0, 0, 18), // "QtIVIZonesAttached"
QT_MOC_LITERAL(1, 19, 14), // "onModelChanged"
QT_MOC_LITERAL(2, 34, 0) // ""

    },
    "QtIVIZonesAttached\0onModelChanged\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_QtIVIZonesAttached[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   19,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,

       0        // eod
};

void QtIVIZonesAttached::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        QtIVIZonesAttached *_t = static_cast<QtIVIZonesAttached *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->onModelChanged(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject QtIVIZonesAttached::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_QtIVIZonesAttached.data,
      qt_meta_data_QtIVIZonesAttached,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};

void *QtIVIZonesAttached::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_QtIVIZonesAttached.stringdata0))
        return static_cast<void*>(const_cast< QtIVIZonesAttached*>(this));
    return QObject::qt_metacast(_clname);
}

int QtIVIZonesAttached::qt_metacall_generated(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 1;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
