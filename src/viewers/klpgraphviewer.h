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
#include "klp/aliasklp.h"
#include "aliasviewers.h"

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

//! Class to graphically represent content of KLP output files
class KLPGraphViewer : public QDialog
{
    Q_OBJECT

public:
    KLPGraphViewer(QString const& lastPath, QSettings& settings, QWidget* pParent = nullptr);
    ~KLPGraphViewer();
    // Results
    void openResultsDialog();
    void openResults(QStringList const& locationFiles);
    // Graphs
    void setGraphs(MapGraphs const& graphs);

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

private:
    QString mLastPath;
    QSettings& mSettings;
    // GUI
    ads::CDockManager* mpDockManager = nullptr;
    QCustomPlot* mpFigure;
    QListView* mpListResults;
    QTextEdit* mpTextInfo;
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
