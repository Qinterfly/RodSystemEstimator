/*!
 * \file
 * \author Pavel Lakiza
 * \date August 2022
 * \brief Definition of the EnergyGraphData class
 */

#include "energygraphdata.h"
#include "klp/framecollection.h"

using namespace RSE::Viewers;

EnergyGraphData::EnergyGraphData(EnergyType type, Direction direction)
    : AbstractGraphData(Category::cEnergy, direction)
    , mType(type)
{

}

//! Retrieve the data of the specified type and direction from a given frame
GraphDataset EnergyGraphData::getDataset(KLP::FrameCollection const& collection, qint64 sliceIndex) const
{

    switch (mType)
    {
    case eKinetic:
        return sliceByIndex(collection.energy.kinetic, sliceIndex);
    case ePotential:
        return sliceByIndex(collection.energy.potential, sliceIndex);
    case eFull:
        return sliceByIndex(collection.energy.full, sliceIndex);
    default:
        break;
    }
    return GraphDataset();
}
