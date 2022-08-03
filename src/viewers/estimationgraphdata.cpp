/*!
 * \file
 * \author Pavel Lakiza
 * \date August 2022
 * \brief Declaration of the EstimationGraphData class
 */

#include "estimationgraphdata.h"

using namespace RSE::Viewers;

EstimationGraphData::EstimationGraphData(EstimationType type, Direction direction)
    : AbstractGraphData(Category::cEstimation, direction)
    , mType(type)
{

}

//! Retrieve the data of the specified type and direction from a given frame
GraphDataset EstimationGraphData::getDataset(KLP::FrameCollection const& collection, qint64 sliceIndex) const
{

    switch (mType)
    {
    case esDisplacement:
        return sliceByDirectionAndIndex(collection.errorState.displacements, mDirection, sliceIndex);
    case esRotation:
        return sliceByDirectionAndIndex(collection.errorState.rotations, mDirection, sliceIndex);
    case esForce:
        return sliceByDirectionAndIndex(collection.errorState.forces, mDirection, sliceIndex);
    case esMoment:
        return sliceByDirectionAndIndex(collection.errorState.moments, mDirection, sliceIndex);
    default:
        break;
    }
    return GraphDataset();
}
