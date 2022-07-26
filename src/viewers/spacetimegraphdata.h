/*!
 * \file
 * \author Pavel Lakiza
 * \date August 2022
 * \brief Declaration of the SpaceTimeGraphData class
 */

#ifndef SPACETIMEGRAPHDATA_H
#define SPACETIMEGRAPHDATA_H

#include "abstractgraphdata.h"

namespace RSE::Viewers
{

//! Class to deal with spacetime KLP-data
class SpaceTimeGraphData : public AbstractGraphData
{
    Q_OBJECT

public:
    enum SpaceTimeType
    {
        stTime,
        stParameter,
        stNaturalLength,
        stAccumulatedNaturalLength,
        stCoordinate
    };
    Q_ENUM(SpaceTimeType)
    SpaceTimeGraphData(SpaceTimeType type, Direction direction = Direction::dFull);
    ~SpaceTimeGraphData() = default;
    GraphDataset getDataset(KLP::FrameCollection const& collection, qint64 sliceIndex) const override;
    int type() const override { return mType; }

private:
    SpaceTimeType mType;
};

}

#endif // SPACETIMEGRAPHDATA_H
