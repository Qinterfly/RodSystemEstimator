/*!
 * \file
 * \author Pavel Lakiza
 * \date July 2022
 * \brief Declaration of the KLPGraphViewer class
 */

#ifndef KLPGRAPHVIEWER_H
#define KLPGRAPHVIEWER_H

#include <QDialog>
#include "qcustomplot.h"

QT_BEGIN_NAMESPACE
class QSettings;
QT_END_NAMESPACE

namespace ads
{
class CDockManager;
class CDockWidget;
}

namespace KLP
{
class Result;
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
    // Deal with results
    void openResultsDialog();
    void openResults(QStringList const& locationFiles);

private:
    // Content
    void initialize();
    void createContent();
    ads::CDockWidget* createResultWidget();
    ads::CDockWidget* createFigureWidget();
    ads::CDockWidget* createConstructorWidget();
    ads::CDockWidget* createPropertyWidget();
    // Settings
    void saveSettings();
    void restoreSettings();
    void closeEvent(QCloseEvent* pEvent) override;

private:
    // GUI
    QString mLastPath;
    QSettings& mSettings;
    ads::CDockManager* mpDockManager = nullptr;
    QCustomPlot* mpFigure;
    // Data
    std::vector<KLP::Result> mResults;
};

}

}

#endif // KLPGRAPHVIEWER_H
