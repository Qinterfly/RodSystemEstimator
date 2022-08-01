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
GraphDataset AbstractGraphData::getAbsoluteData(KLP::FloatFrameObject const components[], qint64 iStart, qint64 iEnd) const
{
    qint64 numData = iEnd - iStart + 1;
    GraphDataset absoluteData(numData);
    double tModule;
    int k = 0;
    for (int i = iStart; i <= iEnd; ++i)
    {
        tModule = 0.0;
        for (int j = 0; j != KLP::kNumDirections; ++j)
            tModule += qPow(*components[j][i], 2.0);
        absoluteData[k] = qSqrt(tModule);
        ++k;
    }
    return absoluteData;
}

//! Slice data through the specified direction and index
GraphDataset AbstractGraphData::sliceByDirectionAndIndex(KLP::FloatFrameObject const components[], Direction direction, qint64 index) const
{
    if (direction == dFull)
    {
        if (index < 0)
            return getAbsoluteData(components, 0, components[0].size() - 1);
        else
            return getAbsoluteData(components, index, index);
    }
    return sliceByIndex(components[direction], index);
}

//! Slice data by index
//! \return if index is positive, return the value located at the specified index. Otherwise, return the full set of values
GraphDataset AbstractGraphData::sliceByIndex(KLP::FloatFrameObject const& component, qint64 index) const
{
    if (index < 0)
        return GraphDataset(component.begin(), component.end());
    else
        return GraphDataset(1, *component[index]);
}
