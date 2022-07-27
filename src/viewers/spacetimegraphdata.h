/*!
 * \file
 * \author Pavel Lakiza
 * \date July 2022
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
public:
    enum SpaceTimeType
    {
        stTime,
        stParameter,
        stNaturalLength,
        stAccumulatedNaturalLength,
        stCoordiante,
        MAX_SPACE_TIME
    };
    SpaceTimeGraphData(SpaceTimeType type, Direction direction = Direction::dFull);
    ~SpaceTimeGraphData();
    GraphDataset data(KLP::FrameCollection const& collection) override;

private:
    SpaceTimeType mType;
};

}

#endif // SPACETIMEGRAPHDATA_H
