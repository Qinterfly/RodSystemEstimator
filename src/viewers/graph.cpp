/*!
 * \file
 * \author Pavel Lakiza
 * \date August 2022
 * \brief Definition of the Graph class
 */

#include "graph.h"
#include "spacetimegraphdata.h"

using namespace RSE::Viewers;

GraphIDType Graph::smMaxGraphID = 0;

using DataSign = std::pair<AbstractGraphData::Category, int>;

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
QVector<int> Graph::indicesUniqueData() const
{
    // Create the set of unique graph data
    std::map<DataSign, int> uniqueData;
    for (int i = 0; i != KLP::kNumDirections; ++i)
    {
        if (!mpData[i])
            continue;
        DataSign sign(mpData[i]->category(), mpData[i]->type());
        if (!uniqueData.contains(sign))
            uniqueData[sign] = i;
    }
    // Retrieve indices of unique entities
    QVector<int> indices;
    for (auto item : uniqueData)
        indices.push_back(item.second);
    std::sort(indices.begin(), indices.end());
    return indices;
}

//! Retrieve the index of the time data
int Graph::indexTimeData() const
{
    return indexData(AbstractGraphData::cSpaceTime, SpaceTimeGraphData::stTime);
}

//! Retrieve the index of the data of the requested category and type (optional)
int Graph::indexData(int iCategory, int iType) const
{
    int iData = -1;
    bool isData;
    for (int i = 0; i != KLP::kNumDirections; ++i)
    {
        AbstractGraphData* pData = mpData[i];
        if (!pData)
            continue;
        isData = pData->category() == iCategory;
        // Check if the type is specified
        if (iType >= 0)
            isData = isData && pData->type() == iType;
        if (isData)
        {
            iData = i;
            break;
        }
    }
    return iData;
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
