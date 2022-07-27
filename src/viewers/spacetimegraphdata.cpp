/*!
 * \file
 * \author Pavel Lakiza
 * \date July 2022
 * \brief Definition of the SpaceTimeGraphData class
 */

#include "spacetimegraphdata.h"
#include "klp/framecollection.h"

using namespace RSE::Viewers;

SpaceTimeGraphData::SpaceTimeGraphData(Direction direction, Type type)
    : AbstractGraphData(Category::cSpaceTime, direction)
    , mType(type)
{

}

SpaceTimeGraphData::~SpaceTimeGraphData()
{

}

//! Retrieve the data of the specified type and direction from a given frame
GraphData SpaceTimeGraphData::data(KLP::FrameCollection const& collection)
{

    switch (mType)
    {
    case stTime:
        return GraphData(collection.time);
    case stParameter:
        return GraphData(collection.parameter.begin(), collection.parameter.end());
    case stNaturalLength:
        return GraphData(collection.naturalLength.begin(), collection.naturalLength.end());
    case stAccumulatedNaturalLength:
        return GraphData(collection.accumulatedNaturalLength.begin(), collection.accumulatedNaturalLength.end());
    case stCoordiante:
        if (mDirection < Direction::dFull)
        {
            KLP::FloatFrameObject const& coordinate = collection.coordinates[mDirection];
            return GraphData(coordinate.begin(), coordinate.end());
        }
        break;
    default:
        break;
    }
    return GraphData();
}
