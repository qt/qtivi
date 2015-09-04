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
    QtIVIAbstractZoneModelFeature *m_item;

    QMetaObject *m_metaObject;
};

QML_DECLARE_TYPEINFO(QtIVIZonesAttached, QML_HAS_ATTACHED_PROPERTIES)

#endif // QTIVIZONESATTACHED_H
