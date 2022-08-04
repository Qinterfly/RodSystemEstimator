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

}

//! Wait for the right button to be clicked
void ExtendedSurfaceHandler::mousePressEvent(QMouseEvent* event, const QPoint& mousePos)
{
    if (Qt::MiddleButton == event->button())
        requestContextMenu(mousePos);
    Q3DInputHandler::mousePressEvent(event, mousePos);
}

//! Show a context menu at the specified position
void ExtendedSurfaceHandler::requestContextMenu(QPoint const& position)
{
    QMenu* pMenu = new QMenu();
    pMenu->setAttribute(Qt::WA_DeleteOnClose, true);
    pMenu->addAction(tr("Скопировать изображение"), this, &ExtendedSurfaceHandler::copyImageToClipboard);
    pMenu->popup(mpSurface->mapToGlobal(position));
}

//! Copy an image of a surface to the clipboard
void ExtendedSurfaceHandler::copyImageToClipboard()
{
    QImage image = mpSurface->renderToImage(skMSAASamples);
    QApplication::clipboard()->setImage(image);
}
