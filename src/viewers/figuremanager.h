/*!
 * \file
 * \author Pavel Lakiza
 * \date August 2022
 * \brief Declaration of the FigureManager class
 */

#ifndef FIGUREMANAGER_H
#define FIGUREMANAGER_H

class QWidget;
class Q3DSurface;

namespace ads
{
class CDockWidget;
}

namespace RSE
{

namespace Viewers
{

class ExtendedGraphPlot;

//! Manager to create and select figures of different types: graphs and surfaces
class FigureManager
{
public:
    FigureManager(ads::CDockWidget* pFigureContainer);
    ~FigureManager();
    // Getters
    ExtendedGraphPlot* graphFigure() { return mpGraphFigure; }
    Q3DSurface* surfaceFigure() { return mpSurfaceFigure; }
    // Setters
    void clear();
    void selectGraphFigure();
    void selectSurfaceFigure();

private:
    void initializeGraphFigure();
    void initializeSurfaceFigure();
    void clearGraphFigure();
    void clearSurfaceFigure();

private:
    ads::CDockWidget* mpFigureContainer;
    ExtendedGraphPlot* mpGraphFigure;
    Q3DSurface* mpSurfaceFigure;
    QWidget* mpSurfaceFigureContainer;
};

}

}

#endif // FIGUREMANAGER_H
