/*!
 * \file
 * \author Pavel Lakiza
 * \date August 2022
 * \brief Definition of the GraphDataSlicer class
 */

#include "graphdataslicer.h"
#include "spacetimegraphdata.h"
#include "klp/result.h"

using namespace RSE::Viewers;

static const int skBaseTimeFrame = 0;

GraphDataSlicer::GraphDataSlicer(SliceType type, AbstractGraphData const* pData, KLP::PointerResult pResult)
    : mType(type), mpData(pData), mIndex(0)
{
    mIsTime = pData->category() == AbstractGraphData::cSpaceTime && pData->type() == SpaceTimeGraphData::stTime;
    setResult(pResult);
}

//! Specify a set of results
void GraphDataSlicer::setResult(KLP::PointerResult pResult)
{
    if (mIsTime)
    {
        mDataset = pResult->time();
    }
    else
    {
        KLP::FrameCollection const& collection = pResult->getFrameCollection(skBaseTimeFrame);
        mDataset = mpData->getDataset(collection);
    }
    setLimits();
    setIndex(mIndex);
}

//! Specify limits of values and indices for slicing
void GraphDataSlicer::setLimits()
{
    float minValue = std::numeric_limits<float>::max();
    float maxValue = std::numeric_limits<float>::lowest();
    for (auto value : mDataset)
    {
        if (value < minValue)
            minValue = value;
        if (value > maxValue)
            maxValue = value;
    }
    mLimitsIndices = { 0, mDataset.size() - 1 };
    mLimitsValues  = { minValue, maxValue };
}

//! Determine the boundaries of the dataset
void GraphDataSlicer::setIndex(qint64 index)
{
    if (index < mLimitsIndices.first)
        index = mLimitsIndices.first;
    if (index > mLimitsIndices.second)
        index = mLimitsIndices.second;
    mIndex = index;
}

//! Find the closest value to the required one in the dataset
float GraphDataSlicer::setClosestValue(float searchValue)
{
    float minDistance = std::numeric_limits<float>::max();
    float value, distance;
    int foundIndex = 0;
    for (int i = 0; i != mDataset.size(); ++i)
    {
        value = mDataset[i];
        distance = qAbs(value - searchValue);
        if (distance < minDistance)
        {
            foundIndex = i;
            minDistance = distance;
        }
    }
    mIndex = foundIndex;
    return mDataset[mIndex];
}
