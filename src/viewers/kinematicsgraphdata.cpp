/*!
 * \file
 * \author Pavel Lakiza
 * \date August 2022
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

//! Retrieve the data of the specified type and direction from a given frame
GraphDataset KinematicsGraphData::getDataset(KLP::FrameCollection const& collection, qint64 sliceIndex) const
{
    if (mType == kStrain)
        return sliceByIndex(collection.strain, sliceIndex);
    switch (mType)
    {
    case kDisplacement:
        return sliceByDirectionAndIndex(collection.state.displacements, mDirection, sliceIndex);
    case kRotation:
        return sliceByDirectionAndIndex(collection.state.rotations, mDirection, sliceIndex);
    case kSpeed:
        return sliceByDirectionAndIndex(collection.firstDerivativeState.displacements, mDirection, sliceIndex);
    case kAngularSpeed:
        return sliceByDirectionAndIndex(collection.firstDerivativeState.rotations, mDirection, sliceIndex);
    case kAcceleration:
        return sliceByDirectionAndIndex(collection.secondDerivativeState.displacements, mDirection, sliceIndex);
    case kAngularAcceleration:
        return sliceByDirectionAndIndex(collection.secondDerivativeState.rotations, mDirection, sliceIndex);
    default:
        break;
    }
    return GraphDataset();
}
