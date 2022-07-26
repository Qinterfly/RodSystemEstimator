/*!
 * \file
 * \author Pavel Lakiza
 * \date July 2022
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
#include "klpresultlistmodel.h"

using ads::CDockManager;
using ads::CDockWidget;
using ads::CDockAreaWidget;
using namespace RSE::Viewers;
using namespace RSE::Models;

static const QString skGroupName = "KLPGraphViewer";
static const QString skResultFileExtension = ".klp";

KLPGraphViewer::KLPGraphViewer(QString const& lastPath, QSettings& settings, QWidget* pParent)
    : QDialog(pParent), mLastPath(lastPath), mSettings(settings)
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
    const QSize kToolBarIconSize = {22, 22};
    CDockWidget* pDockWidget = new CDockWidget(tr("Расчетные проекты"));
    pDockWidget->setFeature(CDockWidget::DockWidgetClosable, false);
    // List of results
    mpListResults = new QListView();
    mpResultListModel = new KLPResultListModel(mResults, mpListResults);
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
    pDockWidget->setToolBarIconSize(kToolBarIconSize, CDockWidget::StateDocked);
    pAction = pToolBar->addAction(QIcon(":/icons/document-open.svg"), tr("Открыть (Ctrl+O)"),
                                  this, &KLPGraphViewer::openResultsDialog);
    pAction->setShortcut(Qt::CTRL | Qt::Key_O);
    pAction = pToolBar->addAction(QIcon(":/icons/delete.svg"), tr("Удалить (Delete)"),
                                  mpResultListModel, &KLPResultListModel::removeSelected);
    pAction->setShortcut(Qt::Key_Delete);
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
    pDockWidget->setWidget(mpFigure);
    return pDockWidget;
}

//! Create a widget to construct graphs
CDockWidget* KLPGraphViewer::createConstructorWidget()
{
    CDockWidget* pDockWidget = new CDockWidget(tr("Конструктор графиков"));
    pDockWidget->setFeature(CDockWidget::DockWidgetClosable, false);
    QListView* pWidget = new QListView();
    pDockWidget->setWidget(pWidget);
    return pDockWidget;
}

//! Create a widget to modify properties of graphs
CDockWidget* KLPGraphViewer::createPropertyWidget()
{
    CDockWidget* pDockWidget = new CDockWidget(tr("Редактор свойств графиков"));
    pDockWidget->setFeature(CDockWidget::DockWidgetClosable, false);
    QTreeView* pWidget = new QTreeView();
    pDockWidget->setWidget(pWidget);
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
}

//! Process selected results
void KLPGraphViewer::processSelectedResults()
{
    // Show information about the selected result
    mpTextInfo->clear();
    QModelIndexList indices = mpListResults->selectionModel()->selectedIndexes();
    if (indices.size() == 1)
    {
        int iSelected = indices[0].row();
        showResultInfo(mResults[iSelected]->info());
    }
}

//! Show information about the selected result
void KLPGraphViewer::showResultInfo(KLP::ResultInfo const& info)
{
    mpTextInfo->append(tr("Дата создания проекта: %1").arg(info.creationDateTime.date().toString()));
    mpTextInfo->append(tr("Время создания проекта: %1").arg(info.creationDateTime.time().toString()));
    mpTextInfo->append(tr("Размер файла: %1 Кб").arg(info.fileSize));
    mpTextInfo->append(tr("Общее число записей: %1").arg(info.numTotalRecords));
    mpTextInfo->append(tr("Число записей по времени: %1").arg(info.numTimeRecords));
    mpTextInfo->append(tr("Идентифиактор проекта: %1").arg(info.identifier));
}

