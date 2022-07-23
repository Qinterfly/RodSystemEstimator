/*!
 * \file
 * \author Pavel Lakiza
 * \date July 2022
 * \brief Definition of the SolutionOptions class
 */

#include "solutionoptions.h"

using namespace RSE::Solution;

SolutionOptions::SolutionOptions(int numCalcModes, int numDampModes, int stepModes, double tolTrunc)
    : mNumCalcModes(numCalcModes), mNumDampModes(numDampModes), mStepModes(stepModes), mTolTrunc(tolTrunc)
{

}
