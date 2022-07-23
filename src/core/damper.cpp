/*!
 * \file
 * \author Pavel Lakiza
 * \date July 2022
 * \brief Definition of the Damper class
 */

#include "damper.h"
#include "constants.h"

using namespace RSE::Core;
using namespace RSE::Constants;

Damper::Damper(double massCable, double massCableLoaded, double workingLength, double bouncerLength,
               double springLength, double springStiffness)
    : mMassCable(massCable), mMassLoadedCable(massCableLoaded), mWorkingLength(workingLength), mBouncerLength(bouncerLength)
    , mSpringLength(springLength), mSpringStiffness(springStiffness)
{

}

//! Compute parameters of a spring belonged to a damper
void Damper::computeSpring()
{
    mSpringLength = (mMassLoadedCable * mWorkingLength - mMassCable * mBouncerLength) / (mMassLoadedCable - mMassCable);
    mSpringStiffness = (mMassCable * kGravitationalAcceleration) / (mSpringLength - mWorkingLength);
}
