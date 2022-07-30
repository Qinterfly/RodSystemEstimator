/*!
 * \file
 * \author Pavel Lakiza
 * \date July 2022
 * \brief Declaration of the KinematisGraphData class
 */

#ifndef KINEMATICSGRAPHDATA_H
#define KINEMATICSGRAPHDATA_H

#include "abstractgraphdata.h"

namespace RSE::Viewers
{

//! Class to deal with kinematics of KLP-data
class KinematicsGraphData : public AbstractGraphData
{
    Q_OBJECT

public:
    enum KinematicsType
    {
        kStrain,
        kDisplacement,
        kRotation,
        kSpeed,
        kAngularSpeed,
        kAcceleration,
        kAngularAcceleration
    };
    Q_ENUM(KinematicsType)
    KinematicsGraphData(KinematicsType type, Direction direction = Direction::dFull);
    ~KinematicsGraphData();
    GraphDataset getDataset(KLP::FrameCollection const& collection, qint64 sliceIndex) override;
    int type() const override { return mType; }

private:
    KinematicsType mType;
};

}

#endif // KINEMATICSGRAPHDATA_H
