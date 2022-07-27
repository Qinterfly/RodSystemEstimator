/*!
 * \file
 * \author Pavel Lakiza
 * \date July 2022
 * \brief Definition of the AbstractGraphData class
 */

#include "abstractgraphdata.h"

using namespace RSE::Viewers;

AbstractGraphData::AbstractGraphData(Category category, Direction direction)
    : mCategory(category), mDirection(direction)
{

}

AbstractGraphData::~AbstractGraphData()
{

}

//! Compute the module of specified components
GraphDataset AbstractGraphData::getAbsoluteData(KLP::FloatFrameObject const components[])
{
    qint64 numData = components[0].size();
    GraphDataset absoluteData(numData);
    float tModule;
    for (int i = 0; i != numData; ++i)
    {
        tModule = 0.0;
        for (int j = 0; j != KLP::kNumDirections; ++j)
            tModule += qPow(*components[j][i], 2.0);
        absoluteData[i] = qSqrt(tModule);
    }
    return absoluteData;
}

//! Slice data through the specified direction
GraphDataset AbstractGraphData::sliceDataByDirection(KLP::FloatFrameObject const components[], Direction direction)
{
    if (direction == dFull)
        return getAbsoluteData(components);
    auto const& object = components[direction];
    return GraphDataset(object.begin(), object.end());
}
