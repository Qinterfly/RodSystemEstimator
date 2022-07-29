/*!
 * \file
 * \author Pavel Lakiza
 * \date July 2022
 * \brief Declaration of the MainWindow class
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui
{
class MainWindow;
}
class QSettings;
class QDoubleSpinBox;
class QSpinBox;
class QTableView;
class QTextEdit;
class QProcess;
class QComboBox;
QT_END_NAMESPACE

namespace ads
{
class CDockManager;
class CDockWidget;
}

namespace RSE
{

namespace Core
{
class Project;
class IO;
}

namespace Solution
{
class SolutionManager;
class SolutionOptions;
}

namespace Models
{
class RodSystemTableModel;
class DoubleSpinBoxItemDelegate;
}

namespace Viewers
{
class KLPGraphViewer;
}

namespace App
{

//! Central window of the program
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();
    // Set parameters of a damper
    void setMassCable(double value);
    void setMassLoadedCable(double value);
    void setWorkingLength(double value);
    void setBouncerLength(double value);
    void setSpringLength(double value);
    void setSpringStiffness(double value);
    // Set parameters of a rod system
    void setCable(QString const& name);
    void setForce(double value);
    // Set parameters of supports
    void setLongitudinalStiffness(double value);
    void setVerticalStiffness(double value);
    // Deal with projects
    void createProject();
    void openProjectDialog();
    void openProject(QString const& pathFile);
    void saveAsProject();
    void saveProject();

private:
    // Content
    void initialize();
    void createContent();
    void createDefaultProject();
    void createDefaultSolutionOptions();
    void closeEvent(QCloseEvent* pEvent) override;
    ads::CDockWidget* createDamperWidget();
    ads::CDockWidget* createRodSystemWidget();
    ads::CDockWidget* createSupportWidget();
    ads::CDockWidget* createCalculationWidget();
    ads::CDockWidget* createConsole();
    // Signals & Slots
    void specifyMenuConnections();

private slots:
    // Settings
    void saveSettings();
    void restoreSettings();
    // Recompute
    void computeSpring();
    void computeSpans();
    // Controlling the solution process
    void runRodSystemSolution();
    void runOptimizationSolution();
    void appendOutputData(QByteArray const& data);
    void showConvergence();
    void showResults();
    // Set project data
    void setProjectTitle();
    void setProjectData();
    void setSolutionOptions();
    void setCurrentCable();
    void setBlockedSignals(bool);
    void aboutProgram();

private:
    // GUI
    Ui::MainWindow* mpUi;
    ads::CDockManager* mpDockManager;
    Models::RodSystemTableModel* mpRodSystemTableModel;
    Models::DoubleSpinBoxItemDelegate* mpDoubleSpinBoxItemDelegate;
    std::shared_ptr<Viewers::KLPGraphViewer> mpGraphViewer;
    // Parameters of a damper
    QDoubleSpinBox* mpMassCable;
    QDoubleSpinBox* mpMassLoadedCable;
    QDoubleSpinBox* mpWorkingLength;
    QDoubleSpinBox* mpBouncerLength;
    QDoubleSpinBox* mpSpringLength;
    QDoubleSpinBox* mpSpringStiffness;
    // Parameters of a rod system
    QComboBox* mpNameCable;
    QDoubleSpinBox* mpForce;
    // Parameters of supports
    QDoubleSpinBox* mpLongitudinalStiffness;
    QDoubleSpinBox* mpVerticalStiffness;
    // Options of computational process
    QSpinBox* mpNumCalcModes;
    QSpinBox* mpNumDampModes;
    QSpinBox* mpStepModes;
    QDoubleSpinBox* mpTolTrunc;
    QTextEdit* mpConsole;
    // Project
    RSE::Core::Project* mpProject;
    RSE::Solution::SolutionManager* mpSolutionManager;
    RSE::Solution::SolutionOptions* mpSolutionOptions;
    RSE::Core::IO* mpIO;
    // Settings
    QSharedPointer<QSettings> mpSettings;
};

}

}

#endif // MAINWINDOW_H
