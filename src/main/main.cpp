/*!
 * \file
 * \author Pavel Lakiza
 * \date July 2022
 * \brief Startup
 */

#include <QFile>
#include <QApplication>
#include "central/mainwindow.h"
#include "viewers/apputilities.h"

//! Startup point
int main(int argc, char* argv[])
{
    QCoreApplication::setOrganizationName(APP_AUTHOR);
    QCoreApplication::setApplicationName(APP_NAME);
    QCoreApplication::setApplicationVersion(APP_VERSION);
    QApplication app(argc, argv);
    RSE::Utilities::App::setStyle();
    RSE::App::MainWindow window;
    window.show();
    return app.exec();
}

