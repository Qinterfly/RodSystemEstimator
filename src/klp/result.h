/*!
 * \file
 * \author Pavel Lakiza
 * \date July 2022
 * \brief Declaration of the Result class
 */

#ifndef RESULT_H
#define RESULT_H

#include <QString>
#include "records.h"

namespace KLP
{

//! Class to aggregate all the output obtained via KLPALGSYS solver
class Result
{
public:
    Result(QString const& pathFile);
    ~Result() = default;

private:
    Records mRecords;
};

}

#endif // RESULT_H
