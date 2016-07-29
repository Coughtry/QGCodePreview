#include <QtWidgets>

#include "mainwin.h"

#define APP_VERSION QString("2016")
#define APP_NAME QString("gcodepreview")
#define APP_NAME_FULL QString("QGCodePreview")
#define APP_ORGANIZATION QString("gcodepreview.koppi.github.com")

int main(int argv, char **args)
{
    QApplication app(argv, args);

    setlocale(LC_NUMERIC,"C");

    app.setStyleSheet(
            "QPlainTextEdit{ font-family: \"Courier\"; font-size: 9pt }"
        );

    QCoreApplication::setOrganizationName(APP_ORGANIZATION);
    QCoreApplication::setApplicationName(APP_NAME);
    QCoreApplication::setApplicationVersion(APP_VERSION);
    QCommandLineParser parser;
    parser.setApplicationDescription(QCoreApplication::applicationName());
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument("file", "The file to open.");
    parser.process(app);

    MainWindow win;
    win.setWindowTitle(APP_NAME_FULL);

    win.show();

    return app.exec();
}

