#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <qt-easy-logger/logger.h>
#include "samplewindow.h"
#include "samplewindow2.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    qInstallMessageHandler(h::Logger::messageHandler);

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "qt-framless-window_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }

    qDebug() << "启动第一个例子";
    SampleWindow w;
    w.show();

    qWarning() << "启动第二个例子";
    SampleWindow2 w2;
    w2.show();

    return a.exec();
}
