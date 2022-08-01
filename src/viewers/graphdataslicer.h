/*!
 * \file
 * \author Pavel Lakiza
 * \date August 2022
 * \brief Declaration of the GraphDataSlicer class
 */

#ifndef GRAPHDATASLICER_H
#define GRAPHDATASLICER_H

#include <QObject>
#include "aliasklp.h"
#include "aliasviewers.h"

namespace RSE::Viewers
{

class AbstractGraphData;

class GraphDataSlicer : public QObject
{
    Q_OBJECT

public:
    enum SliceType
    {
        sdX,
        sdY,
        sdZ
    };
    Q_ENUM(SliceType)
    GraphDataSlicer(SliceType type, AbstractGraphData const* pData, KLP::PointerResult pResult);
    // Getters
    SliceType type() const { return mType; }
    qint64 index() const { return mIndex; }
    float value() const { return mDataset[mIndex]; }
    std::pair<qint64, qint64> limitsIndices() const { return mLimitsIndices; }
    std::pair<float, float> limitsValues() const { return mLimitsValues; }
    bool isTime() const { return mIsTime; }
    // Setters
    void setResult(KLP::PointerResult pResult);
    void setIndex(qint64 index);
    float setClosestValue(float searchValue);

private:
    void setLimits();

private:
    SliceType const mType;
    AbstractGraphData const* mpData;
    qint64 mIndex;
    GraphDataset mDataset;
    std::pair<qint64, qint64> mLimitsIndices;
    std::pair<float, float> mLimitsValues;
    bool mIsTime;
};

}

#endif // GRAPHDATASLICER_H
