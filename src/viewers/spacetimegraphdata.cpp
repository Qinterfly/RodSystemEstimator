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
GraphDataset SpaceTimeGraphData::data(KLP::FrameCollection const& collection)
{

    switch (mType)
    {
    case stTime:
        return GraphDataset(collection.time);
    case stParameter:
        return GraphDataset(collection.parameter.begin(), collection.parameter.end());
    case stNaturalLength:
        return GraphDataset(collection.naturalLength.begin(), collection.naturalLength.end());
    case stAccumulatedNaturalLength:
        return GraphDataset(collection.accumulatedNaturalLength.begin(), collection.accumulatedNaturalLength.end());
    case stCoordiante:
        return sliceDataByDirection(collection.coordinates, mDirection);
    default:
        break;
    }
    return GraphDataset();
}
