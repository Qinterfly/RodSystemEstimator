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
GraphDataset AbstractGraphData::getAbsoluteData(KLP::FloatFrameObject const components[], int iStart, int iEnd)
{
    int numData = iEnd - iStart + 1;
    GraphDataset absoluteData(numData);
    float tModule;
    for (int i = iStart; i <= iEnd; ++i)
    {
        tModule = 0.0;
        for (int j = 0; j != KLP::kNumDirections; ++j)
            tModule += qPow(*components[j][i], 2.0);
        absoluteData[i] = qSqrt(tModule);
    }
    return absoluteData;
}

//! Slice data through the specified direction and index
GraphDataset AbstractGraphData::sliceDataByDirectionAndIndex(KLP::FloatFrameObject const components[], Direction direction, int index)
{
    if (direction == dFull)
    {
        if (index < 0)
            return getAbsoluteData(components, 0, components[0].size() - 1);
        else
            return getAbsoluteData(components, index, index);
    }
    return sliceDataByIndex(components[direction], index);
}

//! Slice data by index
//! \return if index is positive, return the value located at the specified index. Otherwise, return the full set of values
GraphDataset AbstractGraphData::sliceDataByIndex(KLP::FloatFrameObject const& component, int index)
{
    if (index < 0)
        return GraphDataset(component.begin(), component.end());
    else
        return GraphDataset(1, *component[index]);
}
