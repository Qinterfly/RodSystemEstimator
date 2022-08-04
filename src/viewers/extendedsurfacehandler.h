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
    void mousePressEvent(QMouseEvent* event, const QPoint& mousePos) override;

private:
    void requestContextMenu(QPoint const& position);
    void copyImageToClipboard();

private:
    Q3DSurface* mpSurface;
    QString mLastPath;
};

}

}

#endif // EXTENDEDSURFACEHANDLER_H
