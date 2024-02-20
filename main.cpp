#include <QApplication>
#include <QLocale>
#include <QTranslator>

#include "samplewindow.h"
#include "samplewindow2.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "qt-framless-window_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }

    SampleWindow w;
    w.show();

    SampleWindow2 w2;
    w2.show();

    return a.exec();
}
