/*!
 * \file
 * \author Pavel Lakiza
 * \date August 2022
 * \brief Definition of the KLPGraphViewer class
 */

#include <QSettings>
#include <QTextEdit>
#include "qcustomplot.h"
#include "DockManager.h"
#include "DockWidget.h"
#include "DockAreaWidget.h"
#include "ads_globals.h"

#include "central/uiconstants.h"
#include "klp/result.h"
#include "apputilities.h"
#include "klpgraphviewer.h"
#include "resultlistmodel.h"
#include "graphlistmodel.h"
#include "graph.h"

using ads::CDockManager;
using ads::CDockWidget;
using ads::CDockAreaWidget;
using namespace RSE::Viewers;
using namespace RSE::Models;

static const QString skGroupName = "KLPGraphViewer";
static const QString skResultFileExtension = ".klp";
static const QSize skToolBarIconSize = {22, 22};

using CurveData = QPair<QVector<GraphDataset>, QVector<int>>;
CurveData getCurveData(PointerGraph const pGraph, PointerResult const pResult, QVector<int> const& indicesData);

KLPGraphViewer::KLPGraphViewer(QString const& lastPath, QSettings& settings, QWidget* pParent)
    : QWidget(pParent), mLastPath(lastPath), mSettings(settings)
{
    initialize();
    createContent();
    restoreSettings();
}

KLPGraphViewer::~KLPGraphViewer()
{
    delete mpDockManager;
}

//! Intialize default graphical objects
void KLPGraphViewer::initialize()
{
    setWindowTitle(tr("Просмотр результатов KLP"));
    setGeometry(0, 0, 1024, 768);
    CDockManager::setConfigFlag(CDockManager::FocusHighlighting, true);
    CDockManager::setConfigFlag(CDockManager::DockAreaHideDisabledButtons, true);
    CDockManager::setConfigFlag(CDockManager::EqualSplitOnInsertion, true);
    mpDockManager = new CDockManager(this);
    mpDockManager->setStyleSheet("");
    RSE::Utilities::App::moveToCenter(this, parentWidget());
}

//! Construct graphical interface
void KLPGraphViewer::createContent()
{
    // Main layout
    QVBoxLayout* pMainLayout = new QVBoxLayout(this);
    pMainLayout->setContentsMargins(0, 0, 0, 0);
    pMainLayout->addWidget(mpDockManager);
    // View KLP results
    mpDockManager->addDockWidget(ads::LeftDockWidgetArea, createResultWidget());
    // Construct graphs
    mpDockManager->addDockWidget(ads::BottomDockWidgetArea, createConstructorWidget());
    // Edit properties of graphs
    mpDockManager->addDockWidget(ads::BottomDockWidgetArea, createPropertyWidget());
    // Plot graphs
    mpDockManager->addDockWidget(ads::RightDockWidgetArea, createFigureWidget());
}

//! Create a widget to open and deal with KLP results
CDockWidget* KLPGraphViewer::createResultWidget()
{
    CDockWidget* pDockWidget = new CDockWidget(tr("Расчетные проекты"));
    pDockWidget->setFeature(CDockWidget::DockWidgetClosable, false);
    // List of results
    mpListResults = new QListView();
    mpResultListModel = new ResultListModel(mResults, mpListResults);
    mpListResults->setModel(mpResultListModel);
    mpListResults->setSelectionMode(QAbstractItemView::ExtendedSelection);
    connect(mpListResults->selectionModel(), &QItemSelectionModel::selectionChanged, this, &KLPGraphViewer::processSelectedResults);
    // Info
    mpTextInfo = new QTextEdit();
    mpTextInfo->setReadOnly(true);
    // Toolbar
    QToolBar* pToolBar = pDockWidget->createDefaultToolBar();
    QAction* pAction;
    pToolBar->setToolButtonStyle(Qt::ToolButtonStyle::ToolButtonIconOnly);
    pDockWidget->setToolBarIconSize(skToolBarIconSize, CDockWidget::StateDocked);
    pAction = pToolBar->addAction(QIcon(":/icons/document-open.svg"), tr("Открыть (Ctrl+O)"),
                                  this, &KLPGraphViewer::openResultsDialog);
    pAction->setShortcut(Qt::CTRL | Qt::Key_O);
    pAction = pToolBar->addAction(QIcon(":/icons/delete.svg"), tr("Удалить (Delete)"),
                                  mpResultListModel, &ResultListModel::removeSelected);
    pAction->setShortcut(Qt::Key_Delete);
    pAction = pToolBar->addAction(QIcon(":/icons/refresh.svg"), tr("Обновить (F5)"),
                                  mpResultListModel, &ResultListModel::updateData);
    pAction->setShortcut(Qt::Key_F5);
    // Arrangement
    QSplitter* pSplitter = new QSplitter();
    pSplitter->addWidget(mpListResults);
    pSplitter->addWidget(mpTextInfo);
    pSplitter->setStretchFactor(0, 1);
    pSplitter->setStretchFactor(1, 2);
    pDockWidget->setWidget(pSplitter);
    return pDockWidget;
}

//! Create a widget to plot graphs
CDockWidget* KLPGraphViewer::createFigureWidget()
{
    CDockWidget* pDockWidget = new CDockWidget(tr("Графический просмотр объектов"));
    pDockWidget->setFeature(CDockWidget::DockWidgetClosable, false);
    mpFigure = new QCustomPlot();
    mpFigure->setAntialiasedElement(QCP::AntialiasedElement::aeAll, true);
    mpFigure->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
    mpFigure->axisRect()->setupFullAxesBox(true);
    pDockWidget->setWidget(mpFigure);
    return pDockWidget;
}

//! Create a widget to construct graphs
CDockWidget* KLPGraphViewer::createConstructorWidget()
{
    CDockWidget* pDockWidget = new CDockWidget(tr("Конструктор графиков"));
    pDockWidget->setFeature(CDockWidget::DockWidgetClosable, false);
    // List of graphs
    mpListGraphs = new QListView();
    mpGraphListModel = new GraphListModel(mGraphs, mpListGraphs);
    mpListGraphs->setModel(mpGraphListModel);
    mpListGraphs->setSelectionMode(QAbstractItemView::ExtendedSelection);
    connect(mpListGraphs->selectionModel(), &QItemSelectionModel::selectionChanged, this, &KLPGraphViewer::processSelectedGraphs);
    // Toolbar
    QToolBar* pToolBar = pDockWidget->createDefaultToolBar();
    QAction* pAction;
    pToolBar->setToolButtonStyle(Qt::ToolButtonStyle::ToolButtonIconOnly);
    pDockWidget->setToolBarIconSize(skToolBarIconSize, CDockWidget::StateDocked);
    pAction = pToolBar->addAction(QIcon(":/icons/list-add.svg"), tr("Добавить (A)"),
                                  mpGraphListModel, &GraphListModel::create);
    pAction->setShortcut(Qt::Key_A);
    pAction = pToolBar->addAction(QIcon(":/icons/list-remove.svg"), tr("Удалить (D)"),
                                  mpGraphListModel, &GraphListModel::removeSelected);
    pAction->setShortcut(Qt::Key_D);
    // Arrangement
    pDockWidget->setWidget(mpListGraphs);
    return pDockWidget;
}

//! Create a widget to modify properties of graphs
CDockWidget* KLPGraphViewer::createPropertyWidget()
{
    CDockWidget* pDockWidget = new CDockWidget(tr("Редактор свойств графиков"));
    pDockWidget->setFeature(CDockWidget::DockWidgetClosable, false);
    mpPropertyTreeWidget = new PropertyTreeWidget();
    connect(mpPropertyTreeWidget, &PropertyTreeWidget::graphChanged, this, &KLPGraphViewer::plot);
    // Arrangement
    pDockWidget->setWidget(mpPropertyTreeWidget);
    return pDockWidget;
}

//! Save settings to a file
void KLPGraphViewer::saveSettings()
{
    mSettings.beginGroup(skGroupName);
    mSettings.setValue(UiConstants::Settings::skGeometry, saveGeometry());
    mSettings.setValue(UiConstants::Settings::skDockingState, mpDockManager->saveState());
    mSettings.endGroup();
}

//! Restore settings from a file
void KLPGraphViewer::restoreSettings()
{
    mSettings.beginGroup(skGroupName);
    restoreGeometry(mSettings.value(UiConstants::Settings::skGeometry).toByteArray());
    mpDockManager->restoreState(mSettings.value(UiConstants::Settings::skDockingState).toByteArray());
    mSettings.endGroup();
}

//! Save settings and delete handling widgets before closing the window
void KLPGraphViewer::closeEvent(QCloseEvent* pEvent)
{
    saveSettings();
    pEvent->accept();
}

//! Open results using a file system dialog
void KLPGraphViewer::openResultsDialog()
{
    QStringList locationFiles = QFileDialog::getOpenFileNames(this, tr("Открыть проект"), mLastPath,
                                                              tr("Формат проекта (*%1)").arg(skResultFileExtension));
    if (!locationFiles.isEmpty())
        openResults(locationFiles);
}

//! Open a set of results using their locations
void KLPGraphViewer::openResults(QStringList const& locationFiles)
{
    for (QString const& pathFile : locationFiles)
    {
        std::shared_ptr<KLP::Result> pResult = std::make_shared<KLP::Result>(pathFile);
        if (!pResult->isEmpty())
        {
            mResults.push_back(pResult);
            mLastPath = pResult->pathFile();
        }
    }
    mpResultListModel->updateContent();
    mpResultListModel->selectItem();
}

//! Replace the current set of graphs with the new one
void KLPGraphViewer::setGraphs(MapGraphs&& graphs)
{
    mGraphs = graphs;
    mpGraphListModel->updateContent();
    mpGraphListModel->selectItem();
}

//! Process selected results
void KLPGraphViewer::processSelectedResults()
{
    mpTextInfo->clear();
    QModelIndexList indices = mpListResults->selectionModel()->selectedIndexes();
    PointerResult pResult = nullptr;
    if (indices.size() == 1)
    {
        pResult = mResults[indices[0].row()];
        showResultInfo(pResult->info());
    }
    mpPropertyTreeWidget->setSelectedResult(pResult);
}

//! Process selected graphs
void KLPGraphViewer::processSelectedGraphs()
{
    QModelIndexList indices = mpListGraphs->selectionModel()->selectedIndexes();
    PointerGraph pGraph = nullptr;
    if (indices.size() == 1)
    {
        int idGraph = indices[0].data(Qt::UserRole).toInt();
        if (mGraphs.contains(idGraph))
            pGraph = mGraphs[idGraph];
    }
    mpPropertyTreeWidget->setSelectedGraph(pGraph);
}

//! Show information about the selected result
void KLPGraphViewer::showResultInfo(KLP::ResultInfo const& info)
{
    mpTextInfo->append(tr("Дата создания проекта: %1").arg(info.creationDateTime.date().toString()));
    mpTextInfo->append(tr("Время создания проекта: %1").arg(info.creationDateTime.time().toString()));
    mpTextInfo->append(tr("Размер файла: %1 Кб").arg(info.fileSize));
    mpTextInfo->append(tr("Общее число записей: %1").arg(info.numTotalRecords));
    mpTextInfo->append(tr("Число записей по времени: %1").arg(info.numTimeRecords));
    mpTextInfo->append(tr("Идентифиактор проекта: %1").arg(info.ID));
}

//! Plot the resulting set of graphs
void KLPGraphViewer::plot()
{
    mpFigure->clearPlottables();
    mpFigure->replot();
    QModelIndexList indicesResults = mpListResults->selectionModel()->selectedIndexes();
    QModelIndexList indicesGraphs  = mpListGraphs->selectionModel()->selectedIndexes();
    // Check if there is enough data to plot
    if (indicesResults.empty() || indicesGraphs.empty())
        return;
    int numResults = indicesResults.size();
    int numGraphs  = indicesGraphs.size();
    // Iterate through results
    for (int i = 0; i != numResults; ++i)
    {
        int iResult = indicesResults[i].row();
        PointerResult pResult = mResults[iResult];
        if (!pResult)
            continue;
        // Iterate through graphs
        for (int j = 0; j != numGraphs; ++j)
        {
            int jGraph = indicesGraphs[j].data(Qt::UserRole).toInt();
            PointerGraph pGraph = mGraphs[jGraph];
            if (!pGraph)
                continue;
            // Check if time data is presented
            if (!pGraph->isTimeData())
                continue;
            // Skip underfilled graphs
            int numData = pGraph->indicesUniqueData().size();
            if (numData < 2)
                continue;
            // Determine the type of the graph to plot
            if (!pGraph->isDataSlicer() && numData == KLP::kNumDirections)
                plotSurface(pGraph, pResult);
            else
                plotCurve(pGraph, pResult);
        }
    }
}

//! Represent plottable data as a surface
void KLPGraphViewer::plotSurface(PointerGraph const pGraph, PointerResult const pResult)
{
    // TODO
}

//! Represent plottable data as a curve
void KLPGraphViewer::plotCurve(PointerGraph const pGraph, PointerResult const pResult)
{
    QVector<int> const& indicesData = pGraph->indicesUniqueData();
    auto [curveValues, curveIndices] = getCurveData(pGraph, pResult, indicesData);
    // Check if the curve data is complete
    if (curveValues.size() < 2 || curveValues[0].size() != curveValues[1].size())
        return;
    // Plot the curve
    QCPGraph* pCurve = mpFigure->addGraph();
    pCurve->setData(curveValues[0], curveValues[1]);
    // Specify visual properties
    pCurve->setPen(QPen(pGraph->color(), pGraph->lineWidth()));
    pCurve->setLineStyle(pGraph->lineStyle());
    pCurve->setScatterStyle(QCPScatterStyle(pGraph->scatterShape(), pGraph->scatterSize()));
    // Specify labels
    mpFigure->xAxis->setLabel(pGraph->axesLabels()[curveIndices[0]]);
    mpFigure->yAxis->setLabel(pGraph->axesLabels()[curveIndices[1]]);
    // Rescale axes and update
    mpFigure->rescaleAxes();
    mpFigure->replot();
}

//! Helper function to retrieve data associated with a 2D-curve
CurveData getCurveData(PointerGraph const pGraph, PointerResult const pResult, QVector<int> const& indicesData)
{
    QVector<GraphDataset> curveValues;
    QVector<int> curveIndices;
    bool isDataSlicer = pGraph->isDataSlicer();
    int iTimeData = pGraph->indexTimeData();
    if (isDataSlicer)
    {
        GraphDataSlicer const& dataSlicer = pGraph->dataSlicer();
        qint64 sliceIndex = dataSlicer.index();
        if (dataSlicer.isTime())
        {
            KLP::FrameCollection const& collection = pResult->getFrameCollection(sliceIndex);
            for (int iData : indicesData)
            {
                if (iData == iTimeData)
                    continue;
                AbstractGraphData* pData = pGraph->data()[iData];
                curveValues.push_back(pData->getDataset(collection));
                curveIndices.push_back(iData);
            }
        }
        else
        {
            int numTime = pResult->numTimeRecords();
            int iSliceData = dataSlicer.type();
            for (int iData : indicesData)
            {
                if (iData == iSliceData)
                    continue;
                curveIndices.push_back(iData);
                curveValues.push_back(GraphDataset(numTime));
            }
            int numCurves = curveIndices.size();
            for (int iTime = 0; iTime != numTime; ++iTime)
            {
                KLP::FrameCollection const& collection = pResult->getFrameCollection(iTime);
                for (int k = 0; k != numCurves; ++k)
                {
                    int jData = curveIndices[k];
                    AbstractGraphData* pData = pGraph->data()[jData];
                    curveValues[k][iTime] = pData->getDataset(collection, sliceIndex)[0];
                }
            }
        }
    }
    else
    {
        if (indicesData.size() > 2)
            return CurveData();
        int numTime = pResult->numTimeRecords();
        curveIndices = indicesData;
        curveValues = { GraphDataset(numTime), GraphDataset(numTime) };
        int numCurves = curveIndices.size();
        for (int iTime = 0; iTime != numTime; ++iTime)
        {
            KLP::FrameCollection const& collection = pResult->getFrameCollection(iTime);
            for (int k = 0; k != numCurves; ++k)
            {
                int jData = curveIndices[k];
                AbstractGraphData* pData = pGraph->data()[jData];
                curveValues[k][iTime] = pData->getDataset(collection)[0];
            }
        }
    }
    return CurveData(curveValues, curveIndices);
}
