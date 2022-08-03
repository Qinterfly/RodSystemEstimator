/*!
 * \file
 * \author Pavel Lakiza
 * \date August 2022
 * \brief Declaration of the ConvergenceViewer class
 */

#ifndef CONVERGENCEVIEWER_H
#define CONVERGENCEVIEWER_H

#include <QWidget>
#include "array.h"
#include "qcustomplot.h"

namespace RSE
{

namespace Viewers
{

//! Class to represent convergence of viscosities
class ConvergenceViewer : public QWidget
{
public:
    ConvergenceViewer(QString const& pathFile, QWidget* pParent = nullptr);
    ~ConvergenceViewer();
    void plot();

private:
    void initialize();
    bool read();

private:
    QString const mkPathFile;
    QCustomPlot* mpFigure;
    QStringList mStandardColorNames;
    QVector<QCPScatterStyle::ScatterShape> mStandardShapes;
    QVector<int> mCalcModes;
    Core::Array<double> mDampingValues;
};

}

}

#endif // CONVERGENCEVIEWER_H
