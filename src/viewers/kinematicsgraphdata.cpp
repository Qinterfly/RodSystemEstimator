/*!
 * \file
 * \author Pavel Lakiza
 * \date July 2022
 * \brief Definition of the KinematisGraphData class
 */

#include "kinematicsgraphdata.h"
#include "klp/framecollection.h"

using namespace RSE::Viewers;

KinematicsGraphData::KinematicsGraphData(KinematicsType type, Direction direction)
    : AbstractGraphData(Category::cKinematics, direction)
    , mType(type)
{

}

KinematicsGraphData::~KinematicsGraphData()
{

}

//! Retrieve the data of the specified type and direction from a given frame
GraphDataset KinematicsGraphData::data(KLP::FrameCollection const& collection)
{
    if (mType == kStrain)
        return GraphDataset(collection.strain.begin(), collection.strain.end());
    switch (mType)
    {
    case kDisplacement:
        return sliceDataByDirection(collection.state.displacements, mDirection);
    case kRotation:
        return sliceDataByDirection(collection.state.rotations, mDirection);
    case kSpeed:
        return sliceDataByDirection(collection.firstDerivativeState.displacements, mDirection);
    case kAngularSpeed:
        return sliceDataByDirection(collection.firstDerivativeState.rotations, mDirection);
    case kAcceleration:
        return sliceDataByDirection(collection.secondDerivativeState.displacements, mDirection);
    case kAngularAcceleration:
        return sliceDataByDirection(collection.secondDerivativeState.rotations, mDirection);
    default:
        break;
    }
    return GraphDataset();
}
