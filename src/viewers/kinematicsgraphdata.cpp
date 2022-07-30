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
GraphDataset KinematicsGraphData::getDataset(KLP::FrameCollection const& collection, int sliceIndex)
{
    if (mType == kStrain)
        return sliceDataByIndex(collection.strain, sliceIndex);
    switch (mType)
    {
    case kDisplacement:
        return sliceDataByDirectionAndIndex(collection.state.displacements, mDirection, sliceIndex);
    case kRotation:
        return sliceDataByDirectionAndIndex(collection.state.rotations, mDirection, sliceIndex);
    case kSpeed:
        return sliceDataByDirectionAndIndex(collection.firstDerivativeState.displacements, mDirection, sliceIndex);
    case kAngularSpeed:
        return sliceDataByDirectionAndIndex(collection.firstDerivativeState.rotations, mDirection, sliceIndex);
    case kAcceleration:
        return sliceDataByDirectionAndIndex(collection.secondDerivativeState.displacements, mDirection, sliceIndex);
    case kAngularAcceleration:
        return sliceDataByDirectionAndIndex(collection.secondDerivativeState.rotations, mDirection, sliceIndex);
    default:
        break;
    }
    return GraphDataset();
}
