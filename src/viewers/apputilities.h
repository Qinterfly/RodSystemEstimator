/*!
 * \file
 * \author Pavel Lakiza
 * \date July 2022
 * \brief Declaration of utilities targeted to working with application data
 */

#ifndef APPUTILITIES_H
#define APPUTILITIES_H

#include <QWidget>

namespace RSE
{

namespace Utilities
{

namespace App
{

void setStyle();
void moveToCenter(QWidget* pChildWidget, QWidget* pLeadingWidget = nullptr);
QStringList standardColorNames();

}

}

}

#endif // APPUTILITIES_H
