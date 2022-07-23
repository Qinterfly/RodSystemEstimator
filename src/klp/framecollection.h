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
    //! Number of rods
    int numRods;
    //! Parameter
    FloatFrameObject parameter;
    //! Natural length
    FloatFrameObject naturalLength;
    FloatFrameObject accumulatedNaturalLength;
    //! Coordinates
    FloatFrameObject coordinates[kNumDirections];
    //! Regular state
    StateFrame state;
    //! Projected regular state
    StateFrame projectedState;
    //! First-order derivate of the state with respect to time
    StateFrame firstDerivativeState;
    //! Second-order derivate of the state with respect to time
    StateFrame secondDerivativeState;
    //! State error
    StateFrame errorState;
    //! Strain
    FloatFrameObject strain;
    //! Set of modal states
    std::vector<StateFrame> modalStates;
    //! Frequencies
    FloatFrameObject frequencies;
    //! Energy
    EnergyFrame energy;
};

}

#endif // FRAMECOLLECTION_H
