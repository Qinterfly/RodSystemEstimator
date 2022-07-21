/*!
 * \file
 * \author Pavel Lakiza
 * \date July 2022
 * \brief Definition of the Result class
 */

#include <QFile>
#include "result.h"

using namespace KLP;

//! Read the output file formatted in the KLP way
Result::Result(QString const& pathFile)
    : mRecords(pathFile)
{

}
