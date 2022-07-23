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

//! Energy quantities associated with a frame
struct EnergyFrame
{
    FloatFrameObject kinetic;
    FloatFrameObject potential;
    FloatFrameObject full;
};

//! Kinematic and dynamic quantities associated with a frame
struct StateFrame
{
    FloatFrameObject displacements[kNumDirections];
    FloatFrameObject rotations[kNumDirections];
    FloatFrameObject forces[kNumDirections];
    FloatFrameObject moments[kNumDirections];
};

//! Set of all quantities belonged to a frame
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
    // Kinematic and dynamic quantities
    StateFrame state;
    // Frequencies
    FloatFrameObject frequencies;
    // Modal kinematic and dynamic quantities
    std::vector<StateFrame> modalStates;
    // Energy
    EnergyFrame energy;
};

}

#endif // FRAMECOLLECTION_H
