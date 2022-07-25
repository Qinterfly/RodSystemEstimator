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

//! Align the child widget to the center of the parent widget or, if not specified, to the screen center
void App::moveToCenter(QWidget* pChildWidget, QWidget* pParentWidget)
{
    QRect leadingGeometry;
    if (pParentWidget)
        leadingGeometry = pParentWidget->geometry();
    else
        leadingGeometry = qApp->primaryScreen()->availableGeometry();
    QPoint newPosition = pChildWidget->pos() + leadingGeometry.center() - pChildWidget->geometry().center();
    pChildWidget->move(newPosition);
}
