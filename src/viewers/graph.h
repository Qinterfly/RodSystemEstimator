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

private:
    QString mName;
    GraphIDType mID;
    // Data
    AbstractGraphData* pXData = nullptr;
    AbstractGraphData* pYData = nullptr;
    AbstractGraphData* pZData = nullptr;
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
