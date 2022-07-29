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
    // Getters
    static GraphIDType maxGraphID() { return smMaxGraphID; }
    QString const& name() const { return mName; }
    GraphIDType id() const { return mID; }
    AbstractGraphData** data() { return mpData; }
    QCPGraph::LineStyle lineStyle() const { return mLineStyle; }
    uint lineWidth() const { return mLineWidth; }
    QColor color() const { return mColor; }
    QCPScatterStyle scatterStyle() const { return mScatterStyle; }
    double scatterSize() const { return mScatterSize; }
    QStringList const& axesLabels() const { return mAxesLabels; }
    // Setters
    void setName(QString const& name) { mName = name; }
    void setData(AbstractGraphData* pData, int iData);
    void setData(AbstractGraphData* pXData = nullptr, AbstractGraphData* pYData = nullptr, AbstractGraphData* pZData = nullptr);
    void eraseData(int iData) { setData(nullptr, iData); }
    void setLineStyle(QCPGraph::LineStyle const& lineStyle) { mLineStyle = lineStyle; }
    void setLineWidth(uint lineWidth) { mLineWidth = lineWidth; }
    void setColor(QColor const& color) { mColor = color; }
    void setScatterStyle(QCPScatterStyle const& scatterStyle) { mScatterStyle = scatterStyle; }
    void setScatterSize(double scatterSize) { mScatterSize = scatterSize; }
    void setAxesLabels(QStringList const& axesLabels) { mAxesLabels = axesLabels; }

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
    // Axes labels
    QStringList mAxesLabels = QStringList({"Ось X", "Ось Y", "Ось Z"});

private:
    static GraphIDType smMaxGraphID;
};

}

#endif // GRAPH_H
