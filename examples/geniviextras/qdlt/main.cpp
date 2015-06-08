#include <QCoreApplication>
#include <QTimer>
#include <QtGeniviExtras/QtDlt>

#include "loggingcategories.h"

int main(int argc, char *argv[])
{
    qInstallMessageHandler(QDltRegistration::messageHandler);

    QCoreApplication a(argc, argv);

    QTimer timer;
    timer.connect(&timer, &QTimer::timeout, [] {
        static int counter = 0;
        counter++;
        qCCritical(FOO) << "FOO CATEGORY";
        qCWarning(BAR) << "BAR CATEGORY";
        qCritical() << "FALLBACK";
    });
    timer.setInterval(1000);
    timer.start();

    return a.exec();
}
