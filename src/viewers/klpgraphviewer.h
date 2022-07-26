/*!
 * \file
 * \author Pavel Lakiza
 * \date August 2022
 * \brief Declaration of the KLPGraphViewer class
 */

#ifndef KLPGRAPHVIEWER_H
#define KLPGRAPHVIEWER_H

#include "klp/aliasklp.h"
#include "aliasviewers.h"
#include "propertytreewidget.h"

QT_BEGIN_NAMESPACE
class QSettings;
class QListView;
class QTextEdit;
QT_END_NAMESPACE

namespace ads
{
class CDockManager;
class CDockWidget;
}

namespace KLP
{
class ResultInfo;
}

namespace RSE
{

namespace Models
{
class ResultListModel;
class GraphListModel;
}

namespace Viewers
{

class FigureManager;
class PropertyTreeWidget;

//! Class to graphically represent content of KLP output files
class KLPGraphViewer : public QWidget
{
    Q_OBJECT

public:
    KLPGraphViewer(QString const& lastPath, QSettings& settings, QWidget* pParent = nullptr);
    ~KLPGraphViewer();
    // Results
    void openResultsDialog();
    void openResults(QStringList const& locationFiles);
    // Graphs
    void setStandardGraphs();
    void setGraphs(MapGraphs&& graphs);
    void plot();

private:
    // Content
    void initialize();
    void createContent();
    ads::CDockWidget* createResultWidget();
    ads::CDockWidget* createFigureWidget();
    ads::CDockWidget* createConstructorWidget();
    ads::CDockWidget* createPropertyWidget();
    // Results
    void processSelectedResults();
    void showResultInfo(KLP::ResultInfo const& info);
    // Graphs
    void processSelectedGraphs();
    // Settings
    void saveSettings();
    void restoreSettings();
    void closeEvent(QCloseEvent* pEvent) override;
    // Plotting
    void plotCurve(PointerGraph const pGraph, PointerResult const pResult, bool isCompareResults);
    void plotSurface(PointerGraph const pGraph, PointerResult const pResult, bool isCompareResults);

private:
    QString mLastPath;
    QSettings& mSettings;
    // GUI
    ads::CDockManager* mpDockManager = nullptr;
    FigureManager* mpFigureManager = nullptr;
    QListView* mpListResults;
    QTextEdit* mpTextInfo;
    QListView* mpListGraphs;
    RSE::Viewers::PropertyTreeWidget* mpPropertyTreeWidget;
    // Models
    RSE::Models::ResultListModel* mpResultListModel;
    RSE::Models::GraphListModel* mpGraphListModel;
    // Data
    KLP::Results mResults;
    MapGraphs mGraphs;
};

}

}

#endif // KLPGRAPHVIEWER_H
