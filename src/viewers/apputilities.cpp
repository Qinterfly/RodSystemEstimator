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
    qputenv("QSG_RHI_BACKEND", "opengl");
    QFontDatabase::addApplicationFont(":/fonts/SourceSansPro-Regular.ttf");
    qApp->setStyle("Fusion");
    qApp->setFont(QFont("Source Sans Pro", kFontSize));
    qApp->setStyleSheet(File::loadFileContent(":/styles/modern.qss"));
}

//! Align the child widget to the center of the leading widget or, if not specified, to the screen center
void App::moveToCenter(QWidget* pChildWidget, QWidget* pLeadingWidget)
{
    QRect leadingGeometry;
    if (pLeadingWidget)
        leadingGeometry = pLeadingWidget->geometry();
    else
        leadingGeometry = qApp->primaryScreen()->availableGeometry();
    QPoint newPosition = pChildWidget->pos() + leadingGeometry.center() - pChildWidget->geometry().center();
    pChildWidget->move(newPosition);
}

//! Retrieve a set of names of standard colors
QStringList App::standardColorNames()
{
    QStringList colors =
    {
        "red", "green", "blue",
        "black", "orange", "cyan",
        "magenta", "gray", "purple", "darkyellow"
    };
    return colors;
}
