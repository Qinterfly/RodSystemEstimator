/*!
 * \file
 * \author Pavel Lakiza
 * \date August 2022
 * \brief Declaration of utilities targeted to numerical operations
 */

#ifndef NUMERICALUTILITIES_H
#define NUMERICALUTILITIES_H

#include <QtMath>

namespace RSE::Utilities::Numerical
{

//! Compare two floating-point numbers
inline bool fuzzyCompare(double p1, double p2, double eps)
{
    return (qAbs(p1 - p2) <= eps * qMin(qAbs(p1), qAbs(p2)));
}

}

#endif // NUMERICALUTILITIES_H
