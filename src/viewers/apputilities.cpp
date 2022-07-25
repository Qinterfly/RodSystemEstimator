/*!
 * \file
 * \author Pavel Lakiza
 * \date July 2022
 * \brief Definition of utilites targeted to working with application data
 */

#include <QApplication>
#include <QFontDatabase>
#include <QScreen>
#include <QWidget>
#include "apputilities.h"
#include "fileutilities.h"

using namespace RSE::Utilities;

//! Assign style features to the application
void App::setStyle()
{
    const uint kFontSize = 10;
    QFontDatabase::addApplicationFont(":/fonts/SourceSansPro-Regular.ttf");
    qApp->setStyle("Fusion");
    qApp->setFont(QFont("Source Sans Pro", kFontSize));
    qApp->setStyleSheet(File::loadFileContent(":/styles/modern.qss"));
}

//! Align the center of the widget with the center of the screen
void App::centerWidget(QWidget* pWidget)
{
    QSize screenSize = qApp->primaryScreen()->availableGeometry().size();
    int shiftX = (screenSize.width() - pWidget->width()) / 2;
    int shiftY = (screenSize.height() - pWidget->height()) / 2;
    pWidget->move(shiftX, shiftY);
}
