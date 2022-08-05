/*!
 * \file
 * \author Pavel Lakiza
 * \date August 2022
 * \brief Declaration of the ExtendedSurfaceHandler class
 */

#ifndef EXTENDEDSURFACEHANDLER_H
#define EXTENDEDSURFACEHANDLER_H

#include <Q3DInputHandler>

class Q3DSurface;
class QMenu;

namespace RSE
{

namespace Viewers
{
//! Class which enables additional actions while working with a 3D-surface
class ExtendedSurfaceHandler : public Q3DInputHandler
{
public:
    ExtendedSurfaceHandler(Q3DSurface* pSurface, QObject* pParent = nullptr);

protected:
    void mousePressEvent(QMouseEvent* pEvent, const QPoint& mousePos) override;

private:
    void createActions();
    void showContextMenu(QPoint const& position);
    void copyImageToClipboard();
    void saveImage();

private:
    Q3DSurface* mpSurface;
    QAction* mpCopyAction;
    QAction* mpSaveAction;
};

}

}

#endif // EXTENDEDSURFACEHANDLER_H
