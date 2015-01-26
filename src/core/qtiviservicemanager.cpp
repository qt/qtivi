#include "qtiviservicemanager.h"

QtIVIServiceManager::QtIVIServiceManager(QObject *parent) : QObject(parent)
{

}

QtIVIServiceManager *QtIVIServiceManager::instance()
{
    static QtIVIServiceManager* instance = new QtIVIServiceManager();
    return instance;
}

QtIVIServiceManager::~QtIVIServiceManager()
{

}

