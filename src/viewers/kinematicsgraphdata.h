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
public:
    enum KinematicsType
    {
        kStrain,
        kDisplacement,
        kRotation,
        kSpeed,
        kAngularSpeed,
        kAcceleration,
        kAngularAcceleration,
        MAX_KINEMATICS
    };
    KinematicsGraphData(KinematicsType type, Direction direction = Direction::dFull);
    ~KinematicsGraphData();
    GraphDataset data(KLP::FrameCollection const& collection) override;

private:
    KinematicsType mType;
};

}

#endif // KINEMATICSGRAPHDATA_H
