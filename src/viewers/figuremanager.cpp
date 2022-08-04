/*!
 * \file
 * \author Pavel Lakiza
 * \date August 2022
 * \brief Definition of the FigureManager class
 */

#include <Q3DSurface>
#include "figuremanager.h"
#include "qcustomplot.h"
#include "DockWidget.h"

using namespace RSE::Viewers;
using ads::CDockWidget;

FigureManager::FigureManager(CDockWidget* pFigureContainer)
    : mpFigureContainer(pFigureContainer)
{
    initializeGraphFigure();
    initializeSurfaceFigure();
}

FigureManager::~FigureManager()
{
    mpFigureContainer->takeWidget();
    delete mpGraphFigure;
    delete mpSurfaceFigureContainer;
}

//! Clear all figures
void FigureManager::clear()
{
    clearGraphFigure();
    clearSurfaceFigure();
}

//! Select the graph figure as the central one
void FigureManager::selectGraphFigure()
{
    if (mpFigureContainer->widget() != mpGraphFigure)
    {
        mpFigureContainer->takeWidget();
        mpFigureContainer->setWidget(mpGraphFigure);
    }
}

//! Select the surface figure as the central one
void FigureManager::selectSurfaceFigure()
{
    if (mpFigureContainer->widget() != mpSurfaceFigureContainer)
    {
        mpFigureContainer->takeWidget();
        mpFigureContainer->setWidget(mpSurfaceFigureContainer);
    }
}

//! Initalize the widget to plot graphs
void FigureManager::initializeGraphFigure()
{
    mpGraphFigure = new QCustomPlot();
    mpGraphFigure->plotLayout()->insertRow(0);
    mpGraphFigure->plotLayout()->addElement(0, 0, new QCPTextElement(mpGraphFigure, "", QFont("sans", -1, QFont::Bold)));
    mpGraphFigure->setAntialiasedElement(QCP::AntialiasedElement::aeAll, true);
    mpGraphFigure->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
    mpGraphFigure->axisRect()->setupFullAxesBox(true);
}

//! Initialize the widget to plot surfaces
void FigureManager::initializeSurfaceFigure()
{
    QString const kLabelFormat = "%.3f";
    mpSurfaceFigure = new Q3DSurface();
    mpSurfaceFigure->setSelectionMode(QAbstract3DGraph::SelectionItem);
    mpSurfaceFigureContainer = QWidget::createWindowContainer(mpSurfaceFigure);
    mpSurfaceFigureContainer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    mpSurfaceFigureContainer->setFocusPolicy(Qt::StrongFocus);
    // Shadows
    mpSurfaceFigure->setShadowQuality(Q3DSurface::ShadowQualityNone);
    // Axes
    mpSurfaceFigure->setAxisX(new QValue3DAxis);
    mpSurfaceFigure->setAxisY(new QValue3DAxis);
    mpSurfaceFigure->setAxisZ(new QValue3DAxis);
    mpSurfaceFigure->axisX()->setLabelAutoRotation(30);
    mpSurfaceFigure->axisY()->setLabelAutoRotation(90);
    mpSurfaceFigure->axisZ()->setLabelAutoRotation(30);
    for (auto axis : mpSurfaceFigure->axes())
    {
        axis->setLabelFormat(kLabelFormat);
        axis->setTitleVisible(true);
    }
}

//! Clear the widget to plot graphs
void FigureManager::clearGraphFigure()
{
    QCPTextElement* pTitleElement = (QCPTextElement*)mpGraphFigure->plotLayout()->element(0, 0);
    pTitleElement->setText(QString());
    mpGraphFigure->clearPlottables();
    mpGraphFigure->clearItems();
    mpGraphFigure->replot();
}

//! Clear the widget to plot surfaces
void FigureManager::clearSurfaceFigure()
{
    for (auto pSeries : mpSurfaceFigure->seriesList())
        mpSurfaceFigure->removeSeries(pSeries);
}
