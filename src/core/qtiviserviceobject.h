#ifndef QTIVISERVICEOBJECT_H
#define QTIVISERVICEOBJECT_H

#include <QObject>

#include "qtiviserviceinterface.h"
#include "qtiviglobal.h"

class Q_QTIVICORE_EXPORT QtIVIServiceObject : public QObject, public QtIVIServiceInterface
{
    Q_OBJECT
    Q_INTERFACES(QtIVIServiceInterface)

    Q_PROPERTY(QString id READ id CONSTANT)

public:
    explicit QtIVIServiceObject(QObject *parent = 0);
    virtual ~QtIVIServiceObject();

    virtual QString id() const;

private:
};

#endif // QTIVISERVICEOBJECT_H
