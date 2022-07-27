/*!
 * \file
 * \author Pavel Lakiza
 * \date July 2022
 * \brief Declaration of the AbstractGraphData class
 */

#ifndef ABSTRACTGRAPHDATA_H
#define ABSTRACTGRAPHDATA_H

#include "aliasviewers.h"

namespace KLP
{
class FrameCollection;
}

namespace RSE::Viewers
{

class AbstractGraphData
{
public:
    enum Category
    {
        cSpaceTime,
        cKinematics,
        cForce,
        cEnergy,
        cModal,
        cEstimation,
        MAX_CATEGORY
    };
    enum Direction
    {
        dFirst,
        dSecond,
        dThird,
        dFull
    };
    AbstractGraphData(Category category, Direction direction);
    virtual ~AbstractGraphData() = 0;
    virtual GraphData data(KLP::FrameCollection const& collection) = 0;

protected:
    Category mCategory;
    Direction mDirection;
};

}

#endif // ABSTRACTGRAPHDATA_H
