/*!
 * \file
 * \author Pavel Lakiza
 * \date July 2022
 * \brief Declaration of the AbstractGraphData class
 */

#ifndef ABSTRACTGRAPHDATA_H
#define ABSTRACTGRAPHDATA_H

#include <QObject>
#include "aliasviewers.h"
#include "framecollection.h"

namespace RSE::Viewers
{

class AbstractGraphData : public QObject
{
    Q_OBJECT

public:
    enum Category
    {
        cSpaceTime,
        cKinematics,
        cForce,
        cEnergy,
        cModal,
        cEstimation
    };
    enum Direction
    {
        dFirst,
        dSecond,
        dThird,
        dFull
    };
    Q_ENUM(Category)
    Q_ENUM(Direction)
    AbstractGraphData(Category category, Direction direction);
    virtual ~AbstractGraphData() = 0;
    virtual GraphDataset data(KLP::FrameCollection const& collection) = 0;

protected:
    GraphDataset getAbsoluteData(KLP::FloatFrameObject const components[]);
    GraphDataset sliceDataByDirection(KLP::FloatFrameObject const components[], Direction direction);

protected:
    Category mCategory;
    Direction mDirection;
};

}

#endif // ABSTRACTGRAPHDATA_H
