#include "qtiviserviceobject.h"

#include <QUuid>

QtIVIServiceObject::QtIVIServiceObject(QObject *parent)
    : QObject(parent)
{

}

QtIVIServiceObject::~QtIVIServiceObject()
{

}

/*!
 * The id() function can be overloaded to control the generation of
 * the unique ID used by this Object.
 *
 * By default QUuid::createUuid() is used.
 */
QString QtIVIServiceObject::id() const
{
    static QUuid id = QUuid::createUuid();
    return id.toString();
}


