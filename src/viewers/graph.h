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
    void setXData(AbstractGraphData* pXData) { mpXData = pXData; }
    void setYData(AbstractGraphData* pYData) { mpYData = pYData; }
    void setZData(AbstractGraphData* pZData) { mpZData = pZData; }

private:
    QString mName;
    GraphIDType mID;
    // Data
    AbstractGraphData* mpXData = nullptr;
    AbstractGraphData* mpYData = nullptr;
    AbstractGraphData* mpZData = nullptr;
    // Line options
    QCPGraph::LineStyle mLineStyle = QCPGraph::lsLine;
    QColor mColor = Qt::blue;
    uint mLineWidth = 1;
    // Scatter options
    QCPScatterStyle mScatterStyle = QCPScatterStyle::ssNone;
    double mScatterSize = 5;

private:
    static GraphIDType smMaxGraphID;
};

}

#endif // GRAPH_H
