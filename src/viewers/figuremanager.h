/*!
 * \file
 * \author Pavel Lakiza
 * \date August 2022
 * \brief Declaration of the FigureManager class
 */

#ifndef FIGUREMANAGER_H
#define FIGUREMANAGER_H

class QCustomPlot;
class Q3DSurface;
class QWidget;

namespace ads
{
class CDockWidget;
}

namespace RSE
{

namespace Viewers
{

//! Manager to create and select figures of different types: graphs and surfaces
class FigureManager
{
public:
    FigureManager(ads::CDockWidget* pFigureContainer);
    ~FigureManager();
    // Getters
    QCustomPlot* graphFigure() { return mpGraphFigure; }
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
    QCustomPlot* mpGraphFigure;
    Q3DSurface* mpSurfaceFigure;
    QWidget* mpSurfaceFigureContainer;
};

}

}

#endif // FIGUREMANAGER_H
