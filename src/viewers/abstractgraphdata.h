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
    virtual GraphDataset getDataset(KLP::FrameCollection const& collection, qint64 sliceIndex = -1) const = 0;
    virtual int type() const = 0;
    Category category() const { return mCategory; }
    Direction direction() const { return mDirection; }

protected:
    GraphDataset getAbsoluteData(KLP::FloatFrameObject const components[], qint64 iStart, qint64 iEnd) const;
    GraphDataset sliceByIndex(KLP::FloatFrameObject const& component, qint64 index) const;
    GraphDataset sliceByDirectionAndIndex(KLP::FloatFrameObject const components[], Direction direction, qint64 index) const;

protected:
    Category mCategory;
    Direction mDirection;
};

}

#endif // ABSTRACTGRAPHDATA_H
