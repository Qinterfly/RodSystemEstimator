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

private:
    // Content
    void initializeWindow();
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
    // Parameters of a damper
    void setMassCable(double);
    void setMassLoadedCable(double);
    void setWorkingLength(double);
    void setBouncerLength(double);
    void setSpringLength(double);
    void setSpringStiffness(double);
    void computeSpring();
    // Parameters of a rod system
    void setCable(QString const& name);
    void setForce(double);
    void computeSpans();
    // Parameters of supports
    void setLongitudinalStiffness(double);
    void setVerticalStiffness(double);
    // Controlling the solution process
    void runRodSystemSolution();
    void runOptimizationSolution();
    void appendOutputData(QByteArray);
    void showConvergence();
    // Creating a project
    void createProject();
    // Saving a project
    void saveAsProject();
    void saveProject();
    // Open a project
    void openProjectDialog();
    void openProject(QString const&);
    void setProjectTitle();
    // Set project data
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
