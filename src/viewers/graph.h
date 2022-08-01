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
#include "aliasklp.h"
#include "graphdataslicer.h"

namespace RSE::Viewers
{

using KLP::PointerResult;

class Graph
{
public:
    Graph(QString const& name);
    ~Graph();
    // Getters of data properties
    static GraphIDType maxGraphID() { return smMaxGraphID; }
    QString const& name() const { return mName; }
    GraphIDType id() const { return mID; }
    AbstractGraphData** data() { return mpData; }
    bool isData(int iData) const { return mpData[iData]; }
    bool isDataSlicer() const { return mpDataSlicer; }
    GraphDataSlicer& dataSlicer() const { return *mpDataSlicer; }
    QVector<int> indicesReadyData() const;
    // Getters of visual properties
    QCPGraph::LineStyle lineStyle() const { return mLineStyle; }
    uint lineWidth() const { return mLineWidth; }
    QColor color() const { return mColor; }
    QCPScatterStyle::ScatterShape scatterShape() const { return mScatterShape; }
    double scatterSize() const { return mScatterSize; }
    QStringList const& axesLabels() const { return mAxesLabels; }
    // Setters of data properties
    void setName(QString const& name) { mName = name; }
    void setData(AbstractGraphData* pData, int iData);
    void setData(AbstractGraphData* pXData = nullptr, AbstractGraphData* pYData = nullptr, AbstractGraphData* pZData = nullptr);
    void eraseData(int iData) { setData(nullptr, iData); }
    bool createDataSlicer(GraphDataSlicer::SliceType type, PointerResult pResult);
    // Setters of visual properties
    void setLineStyle(QCPGraph::LineStyle const& lineStyle) { mLineStyle = lineStyle; }
    void setLineWidth(uint lineWidth) { mLineWidth = lineWidth; }
    void setColor(QColor const& color) { mColor = color; }
    void setScatterShape(QCPScatterStyle::ScatterShape const& scatterShape) { mScatterShape = scatterShape; }
    void setScatterSize(double scatterSize) { mScatterSize = scatterSize; }
    void setAxesLabels(QStringList const& axesLabels) { mAxesLabels = axesLabels; }

private:
    QString mName;
    GraphIDType mID;
    // Data
    AbstractGraphData* mpData[KLP::kNumDirections] = {nullptr, nullptr, nullptr};
    GraphDataSlicer* mpDataSlicer = nullptr;
    // Line options
    QCPGraph::LineStyle mLineStyle = QCPGraph::lsLine;
    uint mLineWidth = 1;
    QColor mColor = Qt::blue;
    // Scatter options
    QCPScatterStyle::ScatterShape mScatterShape = QCPScatterStyle::ssNone;
    double mScatterSize = 5;
    // Axes labels
    QStringList mAxesLabels = QStringList({"Ось X", "Ось Y", "Ось Z"});

private:
    static GraphIDType smMaxGraphID;
};

}

#endif // GRAPH_H
