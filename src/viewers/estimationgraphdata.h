/*!
 * \file
 * \author Pavel Lakiza
 * \date August 2022
 * \brief Declaration of the EstimationGraphData class
 */

#ifndef ESTIMATIONGRAPHDATA_H
#define ESTIMATIONGRAPHDATA_H

#include "abstractgraphdata.h"

namespace RSE
{

namespace Viewers
{

//! Class to estimate errors of entities in KLP-data
class EstimationGraphData : public AbstractGraphData
{
    Q_OBJECT

public:
    enum EstimationType
    {
        esDisplacement,
        esRotation,
        esForce,
        esMoment
    };
    Q_ENUM(EstimationType)
    EstimationGraphData(EstimationType type, Direction direction = Direction::dFull);
    ~EstimationGraphData() = default;
    GraphDataset getDataset(KLP::FrameCollection const& collection, qint64 sliceIndex) const override;
    int type() const override { return mType; }

private:
    EstimationType mType;
};

}

}



#endif // ESTIMATIONGRAPHDATA_H
