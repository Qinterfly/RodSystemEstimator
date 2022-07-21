/*!
 * \file
 * \author Pavel Lakiza
 * \date July 2022
 * \brief Collection of the data associated with the specified frame
 */


#ifndef FRAMECOLLECTION_H
#define FRAMECOLLECTION_H

#include "frameobject.h"

namespace KLP
{

const int kNumDirections = 3;

using FloatFrameObject = FrameObject<float>;

struct FrameCollection
{
    // Number of rods
    int numRods;
    // Parameter
    FloatFrameObject parameter;
    // Natural length
    FloatFrameObject naturalLength;
    FloatFrameObject accumulatedNaturalLength;
    // Coordinates
    FloatFrameObject coordinates[kNumDirections];
    // Displacements
    FloatFrameObject displacements[kNumDirections];
    // Rotations
    FloatFrameObject rotations[kNumDirections];
    // Forces
    FloatFrameObject forces[kNumDirections];
    // Moments
    FloatFrameObject moments[kNumDirections];
};

}



#endif // FRAMECOLLECTION_H
