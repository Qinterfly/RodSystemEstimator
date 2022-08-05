/*!
 * \file
 * \author Pavel Lakiza
 * \date August 2022
 * \brief Definition of the ExtendedSurfaceHandler class
 */

#include <Q3DSurface>
#include <QMenu>
#include <QApplication>
#include <QClipboard>
#include "extendedsurfacehandler.h"

using namespace RSE::Viewers;

const int skMSAASamples = 8;

ExtendedSurfaceHandler::ExtendedSurfaceHandler(Q3DSurface* pSurface, QObject* pParent)
    : Q3DInputHandler(pParent), mpSurface(pSurface)
{
    createActions();
}

//! Wait for the right button to be clicked
void ExtendedSurfaceHandler::mousePressEvent(QMouseEvent* pEvent, const QPoint& mousePos)
{
    if (Qt::MiddleButton == pEvent->button())
        showContextMenu(mousePos);
    Q3DInputHandler::mousePressEvent(pEvent, mousePos);
}

//! Create actions to deal with images
void ExtendedSurfaceHandler::createActions()
{
    // Copy action
    mpCopyAction = new QAction(tr("Скопировать изображение"), this);
    mpCopyAction->setShortcut(Qt::Modifier::CTRL | Qt::Key_C);
    connect(mpCopyAction, &QAction::triggered, this, &ExtendedSurfaceHandler::copyImageToClipboard);
    // Save action
    mpSaveAction = new QAction(tr("Сохранить изображение"), this);
    mpSaveAction->setShortcut(Qt::Modifier::CTRL | Qt::Key_S);
    connect(mpSaveAction, &QAction::triggered, this, &ExtendedSurfaceHandler::saveImage);
    // Add the actions to the parent widget
    QWidget* pWidget = qobject_cast<QWidget*>(parent());
    pWidget->addActions({ mpCopyAction, mpSaveAction });
}

//! Show a context menu at the specified position
void ExtendedSurfaceHandler::showContextMenu(QPoint const& position)
{
    QWidget* pWidget = qobject_cast<QWidget*>(parent());
    QMenu* pMenu = new QMenu(pWidget);
    pMenu->setAttribute(Qt::WA_DeleteOnClose, true);
    pMenu->addActions({ mpCopyAction, mpSaveAction });
    pMenu->popup(mpSurface->mapToGlobal(position));
}

//! Copy an image of a surface to the clipboard
void ExtendedSurfaceHandler::copyImageToClipboard()
{
    QImage image = mpSurface->renderToImage(skMSAASamples);
    QApplication::clipboard()->setImage(image);
}

//! Save an image of a surface to a file
void ExtendedSurfaceHandler::saveImage()
{
    QString fileName = QString("%1-%2-%3.png").arg(mpSurface->axisX()->title(), mpSurface->axisZ()->title(), mpSurface->axisY()->title());
    QImage image = mpSurface->renderToImage(skMSAASamples);
    image.save(fileName);
}
