/*!
 * \file
 * \author Pavel Lakiza
 * \date July 2022
 * \brief Startup
 */

#include <QFile>
#include <QApplication>
#include <QFontDatabase>
#include "mainwindow.h"
#include "utilities.h"

//! Startup point
int main(int argc, char* argv[])
{
    QCoreApplication::setOrganizationName(APP_AUTHOR);
    QCoreApplication::setApplicationName(APP_NAME);
    QCoreApplication::setApplicationVersion(APP_VERSION);
    QApplication app(argc, argv);
    QFontDatabase::addApplicationFont(":/fonts/SourceSansPro-Regular.ttf");
    uint fontSize = 10;
    qApp->setFont(QFont("Source Sans Pro", fontSize));
    qApp->setStyle("Fusion");
    qApp->setStyleSheet(RSE::Utilities::File::loadFileContent(":/styles/modern.qss"));
    RSE::App::MainWindow window;
    window.show();
    return app.exec();
}

