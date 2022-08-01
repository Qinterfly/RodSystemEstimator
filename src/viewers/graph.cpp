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
    removeDataSlicer();
}

//! Specify data for all axes
void Graph::setData(AbstractGraphData* pXData, AbstractGraphData* pYData, AbstractGraphData* pZData)
{
    setData(pXData, 0);
    setData(pYData, 1);
    setData(pZData, 2);
}

//! Replace directional data with a new one
void Graph::setData(AbstractGraphData* pData, int iData)
{
    if (mpData[iData])
        delete mpData[iData];
    mpData[iData] = pData;
}

//! Retrieve indices of assigned graph data
QVector<int> Graph::indicesReadyData() const
{
    QVector<int> indices;
    for (int i = 0; i != KLP::kNumDirections; ++i)
    {
        if (mpData[i])
            indices.push_back(i);
    }
    return indices;
}

//! Create an instance of object to slice one of datasets
bool Graph::createDataSlicer(GraphDataSlicer::SliceType type, PointerResult pResult)
{
    if (!isData(type))
        return false;
    removeDataSlicer();
    mpDataSlicer = new GraphDataSlicer(type, mpData[type], pResult);
    return true;
}

//! Remove data slicer if created
void Graph::removeDataSlicer()
{
    if (mpDataSlicer)
    {
        delete mpDataSlicer;
        mpDataSlicer = nullptr;
    }
}
