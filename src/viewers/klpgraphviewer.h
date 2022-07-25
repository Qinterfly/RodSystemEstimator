/*!
 * \file
 * \author Pavel Lakiza
 * \date July 2022
 * \brief Declaration of the KLPGraphViewer class
 */

#ifndef KLPGRAPHVIEWER_H
#define KLPGRAPHVIEWER_H

#include <QDialog>

QT_BEGIN_NAMESPACE
class QSettings;
QT_END_NAMESPACE

namespace ads
{
class CDockManager;
}

namespace RSE
{

namespace Viewers
{

//! Class to graphically represent content of KLP output files
class KLPGraphViewer : public QDialog
{
    Q_OBJECT

public:
    KLPGraphViewer(QString const& lastPath, QSettings& settings, QWidget* pParent = nullptr);
    ~KLPGraphViewer();

private:
    // Content
    void initialize();
    void createContent();
    // Settings
    void saveSettings();
    void restoreSettings();
    void closeEvent(QCloseEvent* pEvent) override;

private:
    QString mLastPath;
    QSettings& mSettings;
    ads::CDockManager* mpDockManager = nullptr;
};

}

}

#endif // KLPGRAPHVIEWER_H
