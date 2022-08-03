/*!
 * \file
 * \author Pavel Lakiza
 * \date August 2022
 * \brief Declaration of the EnergyGraphData class
 */

#ifndef ENERGYGRAPHDATA_H
#define ENERGYGRAPHDATA_H

#include "abstractgraphdata.h"

namespace RSE::Viewers
{

//! Class to deal with energy entities of KLP-data
class EnergyGraphData : public AbstractGraphData
{
    Q_OBJECT

public:
    enum EnergyType
    {
        enKinetic,
        enPotential,
        enFull
    };
    Q_ENUM(EnergyType)
    EnergyGraphData(EnergyType type, Direction direction = Direction::dFull);
    ~EnergyGraphData() = default;
    GraphDataset getDataset(KLP::FrameCollection const& collection, qint64 sliceIndex) const override;
    int type() const override { return mType; }

private:
    EnergyType mType;
};

}

#endif // ENERGYGRAPHDATA_H
