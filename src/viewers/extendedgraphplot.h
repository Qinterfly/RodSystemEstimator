/*!
 * \file
 * \author Pavel Lakiza
 * \date August 2022
 * \brief Declaration of the ExtendedGraphPlot class
 */

#ifndef EXTENDEDGRAPHPLOT_H
#define EXTENDEDGRAPHPLOT_H

#include "qcustomplot.h"

namespace RSE
{

namespace Viewers
{

//! Class to enrich user interactions with the QCustomPlot widget
class ExtendedGraphPlot : public QCustomPlot
{
    Q_OBJECT

public:
    ExtendedGraphPlot(QWidget* pParent = nullptr);

protected:
    void contextMenuEvent (QContextMenuEvent* pEvent) override;

private:
    void createActions();
    void copyPixmapToClipboard();
    void savePixmap();
    void refresh();

private:
    QAction* mpCopyAction;
    QAction* mpSaveAction;
    QAction* mpRefreshAction;
};

}

}

#endif // EXTENDEDGRAPHPLOT_H
