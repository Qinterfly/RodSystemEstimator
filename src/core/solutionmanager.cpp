/*!
 * \file
 * \author Pavel Lakiza
 * \date July 2022
 * \brief Definition of the SolutionManager class
 */

#include <QFileInfo>
#include <QDir>
#include "solutionoptions.h"
#include "solutionmanager.h"

using namespace RSE::Core;
using namespace RSE::Solution;

static QString const skNameRodSystemSolver       = "KLPALGSYSx64.exe";
static QString const skNameOptimizationSolver    = "OptimalDamping.exe";
static QString const skNameExporter              = "KLPExport.exe";
static QString const skNameVisualizer            = "VisualizationX64.exe";
static QString const skFileNameStatus            = "Status.txt";
static QString const skFileNameOptimizationInput = "dampinput.txt";

SolutionManager::SolutionManager(QString const& rootPath, QString const& relativeInputPath, QString const& relativeOutputPath)
{
    mRootPath = QFileInfo(rootPath).absoluteFilePath();
    QChar separator = QDir::separator();
    if (!mRootPath.endsWith(separator))
        mRootPath.append(separator);
    mInputPath  = mRootPath + relativeInputPath;
    mOutputPath = mRootPath + relativeOutputPath;
}

SolutionManager::~SolutionManager()
{
    delete mpRodSystemSolver;
    delete mpOptimizationSolver;
}

//! Stop the solution process
void SolutionManager::stopSolution()
{
    // Interrupt the solution of a rod system
    if (mpRodSystemSolver)
    {
        mpRodSystemSolver->close();
        delete mpRodSystemSolver;
        mpRodSystemSolver = nullptr;
    }
    // Interrupt the optimizer
    if (mpOptimizationSolver)
    {
        mpOptimizationSolver->close();
        delete mpOptimizationSolver;
        mpOptimizationSolver = nullptr;
    }
}

//! Solve a rod system
void SolutionManager::solveRodSystem(Project& project, SolutionOptions const& options)
{
    if (mpRodSystemSolver)
    {
        mpRodSystemSolver->close();
        delete mpRodSystemSolver;
    }
    // Configure the process
    QString program = mRootPath + skNameRodSystemSolver;
    mpRodSystemSolver = new QProcess();
    mpRodSystemSolver->setProgram(program);
    mpRodSystemSolver->setProcessChannelMode(QProcess::MergedChannels);
    mpRodSystemSolver->setWorkingDirectory(mRootPath);
    // Specify signals & slots
    connect(mpRodSystemSolver, &QProcess::readyRead, this, &SolutionManager::processRodSystemStream);
    // Write the input data
    project.writeCalcData(mInputPath, options);
    // Run the solver
#ifdef Q_OS_WINDOWS
    mpRodSystemSolver->start();
#elif defined(Q_OS_LINUX)
    mpRodSystemSolver->startDetached("wine", {program}, mRootPath);
#endif
}

//! Check if the solution process if finished
int SolutionManager::getRodSystemStatus()
{
    int status = -1;
    QFile file(mRootPath + skFileNameStatus);
    if (!file.open(QIODeviceBase::ReadOnly))
        return status;
    QTextStream stream(&file);
    stream >> status;
    return status;
}

//! Process the output of the rod system solver
void SolutionManager::processRodSystemStream()
{
    emit outputSent(mpRodSystemSolver->readAll());
    if (mpRodSystemSolver->state() == QProcess::Running && getRodSystemStatus() >= 0)
    {
        emit rodSystemSolved();
        mpRodSystemSolver->kill();
    }
}

//! Optimize viscosities of dampers as to damp selected set of modes
void SolutionManager::solveOptimization(Project& project, SolutionOptions const& options)
{
    if (mpOptimizationSolver)
    {
        mpOptimizationSolver->close();
        delete mpOptimizationSolver;
    }
    // Configure the optimization process
    QString program = mOutputPath + skNameOptimizationSolver;
    mpOptimizationSolver = new QProcess();
    mpOptimizationSolver->setProcessChannelMode(QProcess::MergedChannels);
    mpOptimizationSolver->setWorkingDirectory(mOutputPath);
#ifdef Q_OS_WINDOWS
    mpOptimizationSolver->setProgram(program);
#elif defined(Q_OS_LINUX)
    mpOptimizationSolver->setProgram("wine");
    mpOptimizationSolver->setArguments({program});
#endif
    // Set signals & slots
    connect(mpOptimizationSolver, &QProcess::readyRead, this, &SolutionManager::processOptimizationStream);
    // Prepare the input data for optimization
    int numDampers = project.rodSystem().numRods() - 1;
    writeOptimizationInput(mOutputPath + skFileNameOptimizationInput, numDampers, options);
    runParserProcess();
    // Run the optimizer
    mpOptimizationSolver->start();
}

//! Process the optimization output
void SolutionManager::processOptimizationStream()
{
    QString const newStepLabel = "Mode optimization step";
    QString const finishedLabel = "Optimization finished";
    QByteArray message = mpOptimizationSolver->readAll();
    // Check the solution state
    bool isNewStep = QString(message).contains(newStepLabel, Qt::CaseInsensitive);
    bool isFinished = QString(message).contains(finishedLabel, Qt::CaseInsensitive);
    // Send the current state
    emit outputSent(std::move(message));
    // Send the signals
    if (mpOptimizationSolver->state() == QProcess::Running)
    {
        if (isNewStep)
            emit optimizationStepPerformed();
        if (isFinished)
        {
            emit optimizationSolved();
            mpOptimizationSolver->kill();
        }
    }
}

//! Prepare data for the optimization process
void SolutionManager::runParserProcess()
{
    // Configure the process
    QString program = mOutputPath + skNameExporter;
    QProcess* pProcess = new QProcess();
    pProcess->setWorkingDirectory(mOutputPath);
#ifdef Q_OS_WINDOWS
    pProcess->setProgram(program);
#elif defined(Q_OS_LINUX)
    pProcess->setProgram("wine");
    pProcess->setArguments({program});
#endif
    // Export modeshapes and DOFs
    pProcess->start();
    while (pProcess->waitForFinished()) { }
    pProcess->close();
    delete pProcess;
}

//! Write the input data for optimization of viscosities
void SolutionManager::writeOptimizationInput(QString const& pathFile, int numDampers, SolutionOptions const& options)
{
    const char kDelimiter = ' ';
    QFile file(pathFile);
    if (!file.open(QIODeviceBase::WriteOnly))
        return;
    QTextStream stream(&file);
    stream << options.numDampModes() << kDelimiter
           << options.numCalcModes() << kDelimiter
           << options.stepModes() << kDelimiter
           << options.tolTrunc() << '\r' << '\n';
    for (int i = 0; i != numDampers; ++i)
        stream << i * 3 + 1 << ":" << 2 << kDelimiter;
    file.close();
}

//! Run the visualizer of a rod system
void SolutionManager::runVisualizer()
{
    QString program = mOutputPath + skNameVisualizer;
#ifdef Q_OS_WINDOWS
    QProcess::startDetached(program, {}, mOutputPath);
#elif defined(Q_OS_LINUX)
    QProcess::startDetached("wine", {program}, mOutputPath);
#endif
}
