#include "echobackend.h"
#include "echozonedbackend.h"
#include "echoplugin.h"

QT_BEGIN_NAMESPACE

extern QVector<QIviFeatureInterface *> echoInterfaceBuilder(echoPlugin *plugin)
{
    QVector<QIviFeatureInterface *> res;
    res << new EchoBackend(plugin);
    res << new EchoZonedBackend(plugin);
    return res;
}

QT_END_NAMESPACE
