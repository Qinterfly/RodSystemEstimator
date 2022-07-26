/*!
 * \file
 * \author Pavel Lakiza
 * \date July 2022
 * \brief Definition of the MainWindow class
 */

#include <QVBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QTextEdit>
#include <QDoubleSpinBox>
#include <QSpinBox>
#include <QSpacerItem>
#include <QSettings>
#include <QTableView>
#include <QHeaderView>
#include <QToolBar>
#include <QComboBox>
#include <QFileDialog>
#include <QMessageBox>

#include "DockManager.h"
#include "DockWidget.h"
#include "DockAreaWidget.h"
#include "ads_globals.h"

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "uiconstants.h"
#include "rodsystemtablemodel.h"
#include "doublespinboxitemdelegate.h"
#include "core/project.h"
#include "core/solutionoptions.h"
#include "core/solutionmanager.h"
#include "core/io.h"
#include "viewers/convergenceviewer.h"
#include "viewers/klpgraphviewer.h"

using ads::CDockManager;
using ads::CDockWidget;
using ads::CDockAreaWidget;

using namespace RSE::App;
using namespace RSE::Solution;
using namespace RSE::Viewers;

// Configuration constants
const static QString skDefaultPath        = "./";
const static QString skDefaultProjectName = "Default";
const static QString skMainWindow         = "MainWindow";
const static QString skSettingsFileName   = "Settings.ini";

// IO constants
const static bool skIsDistribute           = false;
static QString skDirectoryData             = "data/";
static QString skDirectoryInput            = "Input/";
static QString skDirectoryOutput           = "Output/";
const static QString skFileNameCables      = "Провода.txt";
const static QString skFileNameConvergence = "optimal.txt";

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , mpUi(new Ui::MainWindow)
{
    if (!skIsDistribute) skDirectoryData = "../../../" + skDirectoryData;
    initialize();
    createContent();
    specifyMenuConnections();
    restoreSettings();
    openProject(skDefaultPath + skDefaultProjectName + mpIO->extension());
}

MainWindow::~MainWindow()
{
    // GUI
    delete mpDoubleSpinBoxItemDelegate;
    delete mpRodSystemTableModel;
    delete mpUi;
    delete mpDockManager;
    // Project
    delete mpProject;
    delete mpSolutionManager;
    delete mpSolutionOptions;
    delete mpIO;
}

//! Set the state and geometry of the central window
void MainWindow::initialize()
{
    mpUi->setupUi(this);
    setWindowModified(false);
    CDockManager::setConfigFlag(CDockManager::FocusHighlighting, true);
    mpDockManager = new CDockManager(this);
    mpDockManager->setStyleSheet("");
}

//! Create all the widgets and links between them
void MainWindow::createContent()
{
    mpSettings = QSharedPointer<QSettings>(new QSettings(skSettingsFileName, QSettings::IniFormat));
    mpIO = new Core::IO(skDefaultPath);
    // Construct the solution manager
    mpSolutionManager = new SolutionManager(skDirectoryData, skDirectoryInput, skDirectoryOutput);
    connect(mpSolutionManager, &SolutionManager::outputSent, this, &MainWindow::appendOutputData);
    // Create the default project and solution options
    createDefaultProject();
    createDefaultSolutionOptions();
    // Solution process
    mpDockManager->addDockWidget(ads::RightDockWidgetArea, createCalculationWidget());
    // Computational workflow
    mpDockManager->addDockWidget(ads::BottomDockWidgetArea, createConsole());
    // Damper
    CDockAreaWidget* pArea = nullptr;
    pArea = mpDockManager->addDockWidget(ads::LeftDockWidgetArea, createDamperWidget());
    // Mechanical properties and geometry of a rod system
    pArea = mpDockManager->addDockWidget(ads::BottomDockWidgetArea, createRodSystemWidget(), pArea);
    // Parameters of supports
    mpDockManager->addDockWidget(ads::BottomDockWidgetArea, createSupportWidget(), pArea);
    // Represent the project and solution options through the graphical interface
    setProjectData();
    setSolutionOptions();
}

//! Create a field to input a floating-point number
QDoubleSpinBox* createDoubleField(double value, double maxValue = 1e3, int numDecimals = 3)
{
    static const double kMinValue = 0.0;
    QDoubleSpinBox* pWidget = new QDoubleSpinBox();
    pWidget->setRange(kMinValue, maxValue);
    pWidget->setStepType(QAbstractSpinBox::AdaptiveDecimalStepType);
    pWidget->setDecimals(numDecimals);
    pWidget->setValue(value);
    return pWidget;
}

//! Create a field to input an integer
QSpinBox* createIntegerField(int value, int maxValue = 1000)
{
    static const double kMinValue = 0;
    QSpinBox* pWidget = new QSpinBox();
    pWidget->setRange(kMinValue, maxValue);
    pWidget->setValue(value);
    return pWidget;
}

//! Create a widget to specify data of a damper
CDockWidget* MainWindow::createDamperWidget()
{
    CDockWidget* pDockWidget = new CDockWidget(tr("Параметры демпфера"));
    QWidget* pWidget = new QWidget();
    QVBoxLayout* pWidgetLayout = new QVBoxLayout(pWidget);
    QGridLayout* pGridLayout = new QGridLayout();
    // Mass of a cable
    mpMassCable = createDoubleField(0.0);
    pGridLayout->addWidget(new QLabel(tr("Масса провода: ")), 0, 0);
    pGridLayout->addWidget(mpMassCable, 0, 1);
    pGridLayout->addWidget(new QLabel(tr(" (кг)")), 0, 2);
    // Mass of a cable with ice
    mpMassLoadedCable = createDoubleField(0.0);
    pGridLayout->addWidget(new QLabel(tr("Масса провода с куржаком: ")), 1, 0);
    pGridLayout->addWidget(mpMassLoadedCable, 1, 1);
    pGridLayout->addWidget(new QLabel(tr(" (кг)")), 1, 2);
    // Working length
    mpWorkingLength = createDoubleField(0.0);
    pGridLayout->addWidget(new QLabel(tr("Рабочая высота: ")), 2, 0);
    pGridLayout->addWidget(mpWorkingLength, 2, 1);
    pGridLayout->addWidget(new QLabel(tr(" (м)")), 2, 2);
    // Constraint length
    mpBouncerLength = createDoubleField(0.0);
    pGridLayout->addWidget(new QLabel(tr("Высота ограничителя: ")), 3, 0);
    pGridLayout->addWidget(mpBouncerLength, 3, 1);
    pGridLayout->addWidget(new QLabel(tr(" (м)")), 3, 2);
    // Length of a spring
    mpSpringLength = createDoubleField(0.0);
    pGridLayout->addWidget(new QLabel(tr("Длина пружины: ")), 4, 0);
    pGridLayout->addWidget(mpSpringLength, 4, 1);
    pGridLayout->addWidget(new QLabel(tr(" (м)")), 4, 2);
    // Spring stiffness
    mpSpringStiffness = createDoubleField(0.0, 1e5);
    pGridLayout->addWidget(new QLabel(tr("Жесткость пружины: ")), 5, 0);
    pGridLayout->addWidget(mpSpringStiffness, 5, 1);
    pGridLayout->addWidget(new QLabel(tr(" (Н/м)")), 5, 2);
    // Arrangement
    pGridLayout->setColumnStretch(1, 1);
    pWidgetLayout->addLayout(pGridLayout);
    pWidgetLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::MinimumExpanding));
    // Signals & Slots
    connect(mpMassCable, &QDoubleSpinBox::valueChanged, this, &MainWindow::setMassCable);
    connect(mpMassLoadedCable, &QDoubleSpinBox::valueChanged, this, &MainWindow::setMassLoadedCable);
    connect(mpWorkingLength, &QDoubleSpinBox::valueChanged, this, &MainWindow::setWorkingLength);
    connect(mpBouncerLength, &QDoubleSpinBox::valueChanged, this, &MainWindow::setBouncerLength);
    connect(mpSpringLength, &QDoubleSpinBox::valueChanged, this, &MainWindow::setSpringLength);
    connect(mpSpringStiffness, &QDoubleSpinBox::valueChanged, this, &MainWindow::setSpringStiffness);
    pDockWidget->setWidget(pWidget);
    mpUi->menuWindow->addAction(pDockWidget->toggleViewAction());
    return pDockWidget;
}

//! Create a widget to set and control data of a rod system
CDockWidget* MainWindow::createRodSystemWidget()
{
    int const kToolBarIconSize = 30;
    CDockWidget* pDockWidget = new CDockWidget(tr("Параметры расчетной схемы"));
    QWidget* pWidget = new QWidget();
    QVBoxLayout* pWidgetLayout = new QVBoxLayout(pWidget);
    QGridLayout* pGridLayout = new QGridLayout();
    // Table
    QTableView* pTable = new QTableView();
    mpRodSystemTableModel = new Models::RodSystemTableModel(pTable);
    mpRodSystemTableModel->setRodSystem(&mpProject->rodSystem());
    mpDoubleSpinBoxItemDelegate = new Models::DoubleSpinBoxItemDelegate();
    pTable->setModel(mpRodSystemTableModel);
    pTable->setItemDelegate(mpDoubleSpinBoxItemDelegate);
    pTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    pTable->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    // Name of cable
    mpNameCable = new QComboBox();
    setCurrentCable();
    pGridLayout->addWidget(new QLabel(tr("Марка провода: ")), 0, 0);
    pGridLayout->addWidget(mpNameCable, 0, 1, 1, 2);
    // Stretrching force
    mpForce = createDoubleField(0.0, 1e6);
    pGridLayout->addWidget(new QLabel(tr("Тяжение: ")), 3, 0);
    pGridLayout->addWidget(mpForce, 3, 1);
    pGridLayout->addWidget(new QLabel(tr(" (Н)")), 3, 2);
    // Toolbar
    QToolBar* pToolBar = new QToolBar();
    QAction* pAction;
    pToolBar->setStyleSheet("background: #ececec");
    pToolBar->setToolButtonStyle(Qt::ToolButtonStyle::ToolButtonIconOnly);
    pAction = pToolBar->addAction(QIcon(":/icons/table-row-add.svg"), tr("Вставить (Insert, A)"),
                                  mpRodSystemTableModel, &Models::RodSystemTableModel::insertAfterSelected);
    pAction->setShortcuts({Qt::Key_Insert, Qt::Key_A});
    pAction = pToolBar->addAction(QIcon(":/icons/table-row-delete.svg"), tr("Удалить (Delete, D)"),
                                  mpRodSystemTableModel, &Models::RodSystemTableModel::removeSelected);
    pAction->setShortcuts({Qt::Key_Delete, Qt::Key_D});
    pToolBar->setMaximumHeight(kToolBarIconSize);
    // Signals & Slots
    connect(mpNameCable, &QComboBox::currentTextChanged, this, &MainWindow::setCable);
    connect(mpForce, &QDoubleSpinBox::valueChanged, this, &MainWindow::setForce);
    connect(mpRodSystemTableModel, &Models::RodSystemTableModel::modified, this, &MainWindow::computeSpans);
    // Arrangement of fields
    pGridLayout->setColumnStretch(1, 1);
    pWidgetLayout->addLayout(pGridLayout);
    pWidgetLayout->addWidget(pToolBar);
    pWidgetLayout->addWidget(pTable);
    pDockWidget->setWidget(pWidget);
    mpUi->menuWindow->addAction(pDockWidget->toggleViewAction());
    return pDockWidget;
}

//! Create a widget to specify data of supports
CDockWidget* MainWindow::createSupportWidget()
{
    CDockWidget* pDockWidget = new CDockWidget(tr("Жесткости опор"));
    QWidget* pWidget = new QWidget();
    QVBoxLayout* pWidgetLayout = new QVBoxLayout(pWidget);
    QGridLayout* pGridLayout = new QGridLayout();
    // Longitudinal stiffness
    mpLongitudinalStiffness = createDoubleField(0.0, 1e8);
    pGridLayout->addWidget(new QLabel(tr("Продольная: ")), 0, 0);
    pGridLayout->addWidget(mpLongitudinalStiffness, 0, 1);
    pGridLayout->addWidget(new QLabel(tr(" (Н/м)")), 0, 2);
    // Vertical stiffness
    mpVerticalStiffness = createDoubleField(0.0, 1e8);
    pGridLayout->addWidget(new QLabel(tr("Вертикальная: ")), 1, 0);
    pGridLayout->addWidget(mpVerticalStiffness, 1, 1);
    pGridLayout->addWidget(new QLabel(tr(" (Н/м)")), 1, 2);
    // Create signals & slots
    connect(mpLongitudinalStiffness, &QDoubleSpinBox::valueChanged, this, &MainWindow::setLongitudinalStiffness);
    connect(mpVerticalStiffness, &QDoubleSpinBox::valueChanged, this, &MainWindow::setVerticalStiffness);
    // Arrangement of fields
    pGridLayout->setColumnStretch(1, 1);
    pWidgetLayout->addLayout(pGridLayout);
    pWidgetLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::MinimumExpanding));
    pDockWidget->setWidget(pWidget);
    mpUi->menuWindow->addAction(pDockWidget->toggleViewAction());
    return pDockWidget;
}

//! Create a widget to control the solution process
CDockWidget* MainWindow::createCalculationWidget()
{
    QSize const kToolBarIconSize(18, 18);
    CDockWidget* pDockWidget = new CDockWidget(tr("Управление расчетом"));
    QWidget* pWidget = new QWidget();
    QVBoxLayout* pWidgetLayout = new QVBoxLayout(pWidget);
    QGridLayout* pGridLayout = new QGridLayout();
    // Toolbar
    QToolBar* pToolBar = pDockWidget->createDefaultToolBar();
    pToolBar->setToolButtonStyle(Qt::ToolButtonStyle::ToolButtonIconOnly);
    pDockWidget->setToolBarIconSize(kToolBarIconSize, CDockWidget::StateDocked);
    pToolBar->addAction(QIcon(":/icons/debug-stop.svg"), tr("Остановка процесса решения"),
                        mpSolutionManager, &Solution::SolutionManager::stopSolution);
    pToolBar->addSeparator();
    pToolBar->addAction(QIcon(":/icons/debug-start.svg"), tr("Запустить расчет частот"),
                        this, &MainWindow::runRodSystemSolution);
    pToolBar->addAction(QIcon(":/icons/idea.svg"), tr("Запустить расчет оптимального демпфирования"),
                        this, &MainWindow::runOptimizationSolution);
    pToolBar->addSeparator();
    pToolBar->addAction(QIcon(":/icons/window-new.svg"), tr("Запустить визуализацию"),
                        mpSolutionManager, &SolutionManager::runVisualizer);
    pToolBar->addAction(QIcon(":/icons/line-chart.svg"), tr("Просмотр сходимости"), this, &MainWindow::showConvergence);
    pToolBar->addAction(QIcon(":/icons/view.svg"), tr("Просмотр результатов"), this, &MainWindow::showResults);
    // Number of computational modes
    mpNumCalcModes = createIntegerField(0);
    pGridLayout->addWidget(new QLabel(tr("Число тонов колебаний для расчета: ")), 0, 0);
    pGridLayout->addWidget(mpNumCalcModes, 0, 1);
    pGridLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::MinimumExpanding, QSizePolicy::Minimum), 0, 2);
    // Number of modes to be damped
    mpNumDampModes = createIntegerField(0);
    pGridLayout->addWidget(new QLabel(tr("Число тонов колебаний для демпфирования: ")), 1, 0);
    pGridLayout->addWidget(mpNumDampModes, 1, 1);
    pGridLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::MinimumExpanding, QSizePolicy::Minimum), 1, 2);
    // Step of computational modes
    mpStepModes = createIntegerField(0);
    pGridLayout->addWidget(new QLabel(tr("Шаг по числу тонов колебаний для расчета: ")), 2, 0);
    pGridLayout->addWidget(mpStepModes, 2, 1);
    pGridLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::MinimumExpanding, QSizePolicy::Minimum), 2, 2);
    // Truncation limit of computational modes
    mpTolTrunc = createDoubleField(0.0, 1.0, 6);
    pGridLayout->addWidget(new QLabel(tr("Предел отсечения расчетных форм колебаний: ")), 3, 0);
    pGridLayout->addWidget(mpTolTrunc, 3, 1);
    pGridLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::MinimumExpanding, QSizePolicy::Minimum), 3, 2);
    // Specify signals & slots
    connect(mpNumCalcModes, &QSpinBox::valueChanged, this, [ & ](int value) { mpSolutionOptions->setNumCalcModes(value); });
    connect(mpNumDampModes, &QSpinBox::valueChanged, this, [ & ](int value) { mpSolutionOptions->setNumDampModes(value); });
    connect(mpStepModes, &QSpinBox::valueChanged, this, [ & ](int value) { mpSolutionOptions->setStepModes(value); });
    connect(mpTolTrunc, &QDoubleSpinBox::valueChanged, this, [ & ](double value) { mpSolutionOptions->setTolTrunc(value); });
    // Arrangement of fields
    pWidgetLayout->addLayout(pGridLayout);
    pWidgetLayout->addItem(new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::MinimumExpanding));
    pDockWidget->setWidget(pWidget);
    mpUi->menuWindow->addAction(pDockWidget->toggleViewAction());
    return pDockWidget;
}

//! Construct a widget to view solution information
CDockWidget* MainWindow::createConsole()
{
    CDockWidget* pDockWidget = new CDockWidget(tr("Вывод результатов расчета"));
    mpConsole = new QTextEdit();
    mpConsole->setReadOnly(true);
    pDockWidget->setWidget(mpConsole);
    mpUi->menuWindow->addAction(pDockWidget->toggleViewAction());
    return pDockWidget;
}

//! Specify menu interactions
void MainWindow::specifyMenuConnections()
{
    // Project
    connect(mpUi->actionExit, &QAction::triggered, this, &QMainWindow::close);
    connect(mpUi->actionNewProject, &QAction::triggered, this, &MainWindow::createProject);
    connect(mpUi->actionOpenProject, &QAction::triggered, this, &MainWindow::openProjectDialog);
    connect(mpUi->actionSaveAsProject, &QAction::triggered, this, &MainWindow::saveAsProject);
    // Help
    connect(mpUi->actionAboutQt, &QAction::triggered, qApp, &QApplication::aboutQt);
    connect(mpUi->actionAboutProgram, &QAction::triggered, this, &MainWindow::aboutProgram);
}

//! Save current graphical settings of floating widgets
void MainWindow::saveSettings()
{
    mpSettings->beginGroup(skMainWindow);
    mpSettings->setValue(UiConstants::Settings::skGeometry, saveGeometry());
    mpSettings->setValue(UiConstants::Settings::skState, saveState());
    mpSettings->setValue(UiConstants::Settings::skDockingState, mpDockManager->saveState());
    mpSettings->endGroup();
    if (mpSettings->status() == QSettings::NoError)
        qInfo() << tr("Настройки сохранены в файл") << skSettingsFileName;
}

//! Read graphical settings from a file
void MainWindow::restoreSettings()
{
    mpSettings->beginGroup(skMainWindow);
    bool isOk = restoreGeometry(mpSettings->value(UiConstants::Settings::skGeometry).toByteArray())
                && restoreState(mpSettings->value(UiConstants::Settings::skState).toByteArray())
                && mpDockManager->restoreState(mpSettings->value(UiConstants::Settings::skDockingState).toByteArray());
    mpSettings->endGroup();
    if (isOk)
        qInfo() << tr("Настройки считаны из файла") << skSettingsFileName;
    else
        qWarning() << tr("Возникла ошибка при чтении настроек из файла") << skSettingsFileName;
}

//! Save settings and parameters of project while closing the central window
void MainWindow::closeEvent(QCloseEvent* pEvent)
{
    saveProject();
    saveSettings();
    pEvent->accept();
}

//! Set mass of a cable
void MainWindow::setMassCable(double value)
{
    mpProject->damper().setMassCable(value);
    computeSpring();
}

//! Set mass of a cable with ice on it
void MainWindow::setMassLoadedCable(double value)
{
    mpProject->damper().setMassLoadedCable(value);
    computeSpring();
}

//! Specify working length
void MainWindow::setWorkingLength(double value)
{
    mpProject->damper().setWorkingLength(value);
    computeSpring();
}

//! Set length of a bouncer
void MainWindow::setBouncerLength(double value)
{
    mpProject->damper().setBouncerLength(value);
    computeSpring();
}

//! Assingn length of a spring
void MainWindow::setSpringLength(double value)
{
    mpProject->damper().setSpringLength(value);
}

//! Set spring stiffness of a damper
void MainWindow::setSpringStiffness(double value)
{
    mpProject->damper().setSpringStiffness(value);
}

//! Compute parameters of a spring
void MainWindow::computeSpring()
{
    Core::Damper& damper = mpProject->damper();
    damper.setSpringLength(0.0);
    damper.setSpringStiffness(0.0);
    if (damper.massLoadedCable() > damper.massCable() && damper.bouncerLength() < damper.workingLength())
        damper.computeSpring();
    mpSpringLength->setValue(damper.springLength());
    mpSpringStiffness->setValue(damper.springStiffness());
}

//! Assign cables to a rod system
void MainWindow::setCable(QString const& name)
{
    Core::DataBaseCables const& dataBaseCables = mpProject->dataBaseCables();
    mpProject->rodSystem().setCable(dataBaseCables.getItem(name.toStdString()));
    computeSpans();
}

//! Specify stretching force
void MainWindow::setForce(double value)
{
    mpProject->rodSystem().setForce(value);
    computeSpans();
}

//! Compute length of all cables
void MainWindow::computeSpans()
{
    mpRodSystemTableModel->updateContent();
}

//! Specify longitudinal stiffness of all supports
void MainWindow::setLongitudinalStiffness(double value)
{
    mpProject->support().setLongitudinalStiffness(value);
}

//! Specify vertical stiffness of all supports
void MainWindow::setVerticalStiffness(double value)
{
    mpProject->support().setVerticalStiffness(value);
}

//! Create a default project
void MainWindow::createDefaultProject()
{
    // Cables
    Core::DataBaseCables dataBaseCables(skDirectoryData, skFileNameCables);
    // Damper
    Core::Damper damper(62.0, 124.0, 0.5, 0.4);
    // Rod system
    Core::RodSystem rodSystem({72, 72, 72, 72, 72, 72, 72, 72, 72, 72}, dataBaseCables.getItem("АС 120/19"), 7200);
    // Supports
    Core::Support support(4.1e4, 2.6e5);
    // Project
    mpProject = new Core::Project(skDefaultProjectName, dataBaseCables, damper, rodSystem, support);
    mpProject->readTemplateData(skDirectoryData + skDirectoryInput);
    setProjectTitle();
}

//! Create default solution options
void MainWindow::createDefaultSolutionOptions()
{
    mpSolutionOptions = new SolutionOptions(30, 5, 0, 1e-5);
}

//! Solve the rod system
void MainWindow::runRodSystemSolution()
{
    mpConsole->clear();
    mpSolutionManager->solveRodSystem(*mpProject, *mpSolutionOptions);
}

//! Optimize viscosities of dampers
void MainWindow::runOptimizationSolution()
{
    mpSolutionManager->solveOptimization(*mpProject, *mpSolutionOptions);
}

//! Process the message from the solution process
void MainWindow::appendOutputData(QByteArray const& data)
{
    mpConsole->append(data);
}

//! Open a new project
void MainWindow::createProject()
{
    delete mpProject;
    delete mpSolutionOptions;
    createDefaultProject();
    createDefaultSolutionOptions();
    setProjectData();
    setSolutionOptions();
}

//! Save the project using a dialog window
void MainWindow::saveAsProject()
{
    QString pathFile = QFileDialog::getSaveFileName(this, tr("Сохранить проект"), mpIO->lastPath(),
                                                    tr("Формат проекта (*%1)").arg(mpIO->extension()));
    if (!pathFile.isEmpty())
        mpIO->saveAs(pathFile, *mpProject, *mpSolutionOptions);
    setProjectTitle();
}

//! Save the current project
void MainWindow::saveProject()
{
    QString filePath = mpIO->lastPath() + mpProject->name() + mpIO->extension();
    mpIO->saveAs(filePath, *mpProject, *mpSolutionOptions);
}

//! Open a project by means of a dialog window
void MainWindow::openProjectDialog()
{
    QString pathFile = QFileDialog::getOpenFileName(this, tr("Открыть проект"), mpIO->lastPath(),
                                                    tr("Формат проекта  (*%1)").arg(mpIO->extension()));
    if (!pathFile.isEmpty())
        openProject(pathFile);
}

//! Open a project using a path specified
void MainWindow::openProject(QString const& pathFile)
{
    Core::DataBaseCables dataBaseCables(skDirectoryData, skFileNameCables);
    Core::IOPair ioPair = mpIO->open(pathFile, dataBaseCables);
    if (!ioPair.first || !ioPair.second)
        return;
    // Обработка проекта
    delete mpProject;
    mpProject = ioPair.first;
    mpProject->readTemplateData(skDirectoryData + skDirectoryInput);
    // Обработка расчетных настроек
    delete mpSolutionOptions;
    mpSolutionOptions = ioPair.second;
    // Обновление данных приложения
    setProjectData();
    setSolutionOptions();
    setProjectTitle();
}

//! Set the name of a project
void MainWindow::setProjectTitle()
{
    QString title = APP_NAME;
    setWindowTitle(QString(title + ": %1[*]").arg(mpProject->name()));
}

//! Set project data
void MainWindow::setProjectData()
{
    setBlockedSignals(true);
    // Parameters of damper
    Core::Damper const& damper = mpProject->damper();
    mpMassCable->setValue(damper.massCable());
    mpMassLoadedCable->setValue(damper.massLoadedCable());
    mpWorkingLength->setValue(damper.workingLength());
    mpBouncerLength->setValue(damper.bouncerLength());
    mpSpringLength->setValue(damper.springLength());
    mpSpringStiffness->setValue(damper.springStiffness());
    // Parameters of a rod system
    Core::RodSystem const& rodSystem = mpProject->rodSystem();
    setCurrentCable();
    mpForce->setValue(rodSystem.force());
    // Parameters of supports
    Core::Support const& support = mpProject->support();
    mpLongitudinalStiffness->setValue(support.longitudinalStiffness());
    mpVerticalStiffness->setValue(support.verticalStiffness());
    setBlockedSignals(false);
    // Refresh the computational data
    if (damper.springLength() == 0.0 || damper.springStiffness() == 0.0)
        computeSpring();
    mpRodSystemTableModel->setRodSystem(&mpProject->rodSystem());
}

//! Set the data to be used as the solution parameters
void MainWindow::setSolutionOptions()
{
    mpNumCalcModes->setValue(mpSolutionOptions->numCalcModes());
    mpNumDampModes->setValue(mpSolutionOptions->numDampModes());
    mpStepModes->setValue(mpSolutionOptions->stepModes());
    mpTolTrunc->setValue(mpSolutionOptions->tolTrunc());
}

//! Select a current cable
void MainWindow::setCurrentCable()
{
    mpNameCable->blockSignals(true);
    mpNameCable->clear();
    auto const& names = mpProject->dataBaseCables().names();
    for (auto const& item : names)
        mpNameCable->addItem(item.data());
    mpNameCable->setCurrentText(mpProject->rodSystem().nameCable().data());
    mpNameCable->blockSignals(false);
}

//! (Un)Block all the signals from widget
void MainWindow::setBlockedSignals(bool flag)
{
    // Parameters of a damper
    mpMassCable->blockSignals(flag);
    mpMassLoadedCable->blockSignals(flag);
    mpWorkingLength->blockSignals(flag);
    mpBouncerLength->blockSignals(flag);
    // Parameters of a rod system
    mpForce->blockSignals(flag);
    // Parameters of supports
    mpLongitudinalStiffness->blockSignals(flag);
    mpVerticalStiffness->blockSignals(flag);
}

//! Represent the convergence of the optimization process
void MainWindow::showConvergence()
{
    QString const pathFile = skDirectoryData + skDirectoryOutput + skFileNameConvergence;
    CDockWidget* pConvergenceWidget = new CDockWidget(tr("Просмотр сходимости"));
    pConvergenceWidget->setFeature(CDockWidget::DockWidgetDeleteOnClose, true);
    // Graphical widget
    ConvergenceViewer* pWidget = new Viewers::ConvergenceViewer(pathFile);
    // Specify signals & slots
    connect(mpSolutionManager, &SolutionManager::optimizationStepPerformed, pWidget, &ConvergenceViewer::plot);
    connect(mpSolutionManager, &SolutionManager::optimizationSolved, pWidget, &ConvergenceViewer::plot);
    // Arrangement
    pConvergenceWidget->setWidget(pWidget);
    mpDockManager->addDockWidgetFloating(pConvergenceWidget);
}

//! Represent the results obtained via KLPALGSYS
void MainWindow::showResults()
{
    KLPGraphViewer* pWidget = new Viewers::KLPGraphViewer(mpIO->lastPath(), *mpSettings, this);
    pWidget->show();
}

//! Show the information about the program
void MainWindow::aboutProgram()
{
    const QString aboutMsg = tr(
                                 "RodSystemEstimator - это программа для генерации исходных данных для решателя стержневых систем, "
                                 "отображения результатов расчета и проведения оптимизации параметров демпферов.\n\n"
                                 "Авторы:\n"
                                 "Дмитрий Красноруцкий (KLPALGSYS, KLPExport),\n"
                                 "Павел Лакиза (RodSystemEstimator, OptimalDamping)."
                             );
    QMessageBox::about(this, tr("О программе %1 (v%2)").arg(APP_NAME, APP_VERSION), aboutMsg);
}
