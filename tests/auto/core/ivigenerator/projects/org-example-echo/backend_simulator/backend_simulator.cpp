#include "echobackend.h"
#include "echozonedbackend.h"
#include "echomodulesimulatorplugin.h"

QT_BEGIN_NAMESPACE

extern QVector<QIviFeatureInterface *> echoInterfaceBuilder(EchomoduleSimulatorPlugin *plugin)
{
    QVector<QIviFeatureInterface *> res;
    res << new EchoBackend(plugin);
    res << new EchoZonedBackend(plugin);
    return res;
}

QT_END_NAMESPACE
