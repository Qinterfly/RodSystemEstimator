/*!
 * \file
 * \author Pavel Lakiza
 * \date July 2022
 * \brief Definition of the Support class
 */

#include "support.h"

using namespace RSE::Core;

Support::Support(double longitudinalStiffness, double lateralStiffness)
    : mLongitudinalStiffness(longitudinalStiffness), mVerticalStiffness(lateralStiffness) { }
