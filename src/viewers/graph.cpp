/*!
 * \file
 * \author Pavel Lakiza
 * \date July 2022
 * \brief Definition of the Graph class
 */

#include "graph.h"

using namespace RSE::Viewers;

GraphIDType Graph::smMaxGraphID = 0;

Graph::Graph(QString const& name)
    : mName(name)
{
    mID = ++smMaxGraphID;
}

Graph::~Graph()
{
    for (int i = 0; i != KLP::kNumDirections; ++i)
    {
        if (mpData[i])
            delete mpData[i];
    }
}

//! Specify data for all axes
void Graph::setData(AbstractGraphData* pXData, AbstractGraphData* pYData, AbstractGraphData* pZData)
{
    setDirectionalData(pXData, 0);
    setDirectionalData(pYData, 1);
    setDirectionalData(pZData, 2);
}

//! Set a new directional data and free the previous one
void Graph::setDirectionalData(AbstractGraphData* pData, int direction)
{
    if (mpData[direction])
        delete mpData[direction];
    mpData[direction] = pData;
}
