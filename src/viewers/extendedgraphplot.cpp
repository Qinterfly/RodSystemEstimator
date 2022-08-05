/*!
 * \file
 * \author Pavel Lakiza
 * \date August 2022
 * \brief Definition of the ExtendedGraphPlot class
 */

#include <QApplication>
#include <QMenu>
#include "extendedgraphplot.h"

using namespace RSE::Viewers;

ExtendedGraphPlot::ExtendedGraphPlot(QWidget* pParent)
    : QCustomPlot(pParent)
{
    createActions();
}

//! Create actions for a context menu
void ExtendedGraphPlot::createActions()
{
    // Copy action
    mpCopyAction = new QAction(tr("Скопировать изображение"), this);
    mpCopyAction->setShortcut(Qt::Modifier::CTRL | Qt::Key_C);
    connect(mpCopyAction, &QAction::triggered, this, &ExtendedGraphPlot::copyPixmapToClipboard);
    // Save action
    mpSaveAction = new QAction(tr("Сохранить изображение"), this);
    mpSaveAction->setShortcut(Qt::Modifier::CTRL | Qt::Key_S);
    connect(mpSaveAction, &QAction::triggered, this, &ExtendedGraphPlot::savePixmap);
    // Refresh action
    mpRefreshAction = new QAction(tr("Обновить"), this);
    mpRefreshAction->setShortcut(Qt::Modifier::CTRL | Qt::Key_F);
    connect(mpRefreshAction, &QAction::triggered, this, &ExtendedGraphPlot::refresh);
    // Add the actions to the widget
    addActions({mpCopyAction, mpSaveAction, mpRefreshAction});
}

//! Copy an image of plotted entities to the clipboard
void ExtendedGraphPlot::copyPixmapToClipboard()
{
    QApplication::clipboard()->setPixmap(toPixmap());
}

//! Save an image of plotted entities to a file
void ExtendedGraphPlot::savePixmap()
{
    QString fileName = QString("%1-%2.png").arg(xAxis->label(), yAxis->label());
    toPixmap().save(fileName);
}

//! Refresh the widget
void ExtendedGraphPlot::refresh()
{
    rescaleAxes();
    replot();
}

//! Show a context menu at the specified position
void ExtendedGraphPlot::contextMenuEvent(QContextMenuEvent* pEvent)
{
    if (pEvent->reason() != QContextMenuEvent::Mouse)
        return;
    QMenu* pMenu = new QMenu(this);
    pMenu->setAttribute(Qt::WA_DeleteOnClose, true);
    pMenu->addActions({mpCopyAction, mpSaveAction, mpRefreshAction});
    pMenu->popup(mapToGlobal(pEvent->pos()));
}
