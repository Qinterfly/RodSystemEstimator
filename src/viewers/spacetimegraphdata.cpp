/*!
 * \file
 * \author Pavel Lakiza
 * \date July 2022
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

SpaceTimeGraphData::~SpaceTimeGraphData()
{

}

//! Retrieve the data of the specified type and direction from a given frame
GraphDataset SpaceTimeGraphData::getDataset(KLP::FrameCollection const& collection, qint64 sliceIndex)
{

    switch (mType)
    {
    case stTime:
        return GraphDataset(collection.time);
    case stParameter:
        return sliceDataByIndex(collection.parameter, sliceIndex);
    case stNaturalLength:
        return sliceDataByIndex(collection.naturalLength, sliceIndex);
    case stAccumulatedNaturalLength:
        return sliceDataByIndex(collection.accumulatedNaturalLength, sliceIndex);
    case stCoordiante:
        return sliceDataByDirectionAndIndex(collection.coordinates, mDirection, sliceIndex);
    default:
        break;
    }
    return GraphDataset();
}
