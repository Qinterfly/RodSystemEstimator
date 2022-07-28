/*!
 * \file
 * \author Pavel Lakiza
 * \date July 2022
 * \brief Declaration of the Graph class
 */

#ifndef GRAPH_H
#define GRAPH_H

#include <QString>
#include <QColor>
#include "qcustomplot.h"
#include "abstractgraphdata.h"
#include "aliasviewers.h"

namespace RSE::Viewers
{

class Graph
{
public:
    Graph(QString const& name);
    ~Graph();
    static GraphIDType maxGraphID() { return smMaxGraphID; }
    GraphIDType id() const { return mID; }
    QString const& name() const { return mName; }
    void setName(QString const& name) { mName = name; }
    void setXData(AbstractGraphData* pData) { setDirectionalData(pData, 0); }
    void setYData(AbstractGraphData* pData) { setDirectionalData(pData, 1); }
    void setZData(AbstractGraphData* pData) { setDirectionalData(pData, 2); }
    void setData(AbstractGraphData* pXData = nullptr, AbstractGraphData* pYData = nullptr, AbstractGraphData* pZData = nullptr);

private:
    void setDirectionalData(AbstractGraphData* pData, int direction);

private:
    QString mName;
    GraphIDType mID;
    // Data
    AbstractGraphData* mpData[KLP::kNumDirections] = {nullptr, nullptr, nullptr};
    // Line options
    QCPGraph::LineStyle mLineStyle = QCPGraph::lsLine;
    uint mLineWidth = 1;
    QColor mColor = Qt::blue;
    // Scatter options
    QCPScatterStyle mScatterStyle = QCPScatterStyle::ssNone;
    double mScatterSize = 5;

private:
    static GraphIDType smMaxGraphID;
};

}

#endif // GRAPH_H
