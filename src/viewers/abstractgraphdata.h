/*!
 * \file
 * \author Pavel Lakiza
 * \date July 2022
 * \brief Declaration of the AbstractGraphData class
 */

#ifndef ABSTRACTGRAPHDATA_H
#define ABSTRACTGRAPHDATA_H

#include <QVector>

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
        cNone,
        cSpacetime,
        cKinematics,
        cForce,
        cEnergy,
        cModal,
        cEstimation,
        MAX_CATEGORY
    };
    enum Direction
    {
        dNone,
        dFirst,
        dSecond,
        dThird,
        dFull
    };
    AbstractGraphData(Category category = cNone, Direction direction = dNone);
    virtual ~AbstractGraphData() = 0;
    virtual QVector<float> data(KLP::FrameCollection const& collection) = 0;

private:
    Category mCategory;
    Direction mDirection;
};

}

#endif // ABSTRACTGRAPHDATA_H
