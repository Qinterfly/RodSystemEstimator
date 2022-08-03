/*!
 * \file
 * \author Pavel Lakiza
 * \date August 2022
 * \brief Definition of the SpaceTimeGraphData class
 */

#include "spacetimegraphdata.h"
#include "klp/framecollection.h"

using namespace RSE::Viewers;

SpaceTimeGraphData::SpaceTimeGraphData(SpaceTimeType type, Direction direction)
    : AbstractGraphData(Category::cSpaceTime, direction)
    , mType(type)
{

}

//! Retrieve the data of the specified type and direction from a given frame
GraphDataset SpaceTimeGraphData::getDataset(KLP::FrameCollection const& collection, qint64 sliceIndex) const
{

    switch (mType)
    {
    case stTime:
        return GraphDataset(1, collection.time);
    case stParameter:
        return sliceByIndex(collection.parameter, sliceIndex);
    case stNaturalLength:
        return sliceByIndex(collection.naturalLength, sliceIndex);
    case stAccumulatedNaturalLength:
        return sliceByIndex(collection.accumulatedNaturalLength, sliceIndex);
    case stCoordinate:
        return sliceByDirectionAndIndex(collection.coordinates, mDirection, sliceIndex);
    default:
        break;
    }
    return GraphDataset();
}
