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
    setData(pXData, 0);
    setData(pYData, 1);
    setData(pZData, 2);
}

//! Replace directional data with a new one
void Graph::setData(AbstractGraphData* pData, int direction)
{
    if (mpData[direction])
        delete mpData[direction];
    mpData[direction] = pData;
}
