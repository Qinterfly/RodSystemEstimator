/*!
 * \file
 * \author Pavel Lakiza
 * \date July 2022
 * \brief Declaration of the SolutionManager class
 */

#ifndef SOLUTIONMANAGER_H
#define SOLUTIONMANAGER_H

#include <QString>
#include <QProcess>
#include <QObject>
#include <QTextStream>
#include "project.h"

namespace RSE::Solution
{

class SolutionOptions;

//! Class to control the solution process
class SolutionManager : public QObject
{
    Q_OBJECT

public:
    SolutionManager(QString const& rootPath, QString const& relativeInputPath, QString const& relativeOutputPath);
    ~SolutionManager();
    void solveRodSystem(Core::Project& project, SolutionOptions const& options);
    void solveOptimization(Core::Project& project, SolutionOptions const& options);
    void runVisualizer();

signals:
    void outputSent(QByteArray);
    void rodSystemSolved();
    void optimizationSolved();
    void optimizationStepPerformed();

public slots:
    void stopSolution();

private:
    void processRodSystemStream();
    void processOptimizationStream();
    void runParserProcess();
    void writeOptimizationInput(QString const& pathFile, int numDampers, SolutionOptions const& options);
    int getRodSystemStatus();

private:
    QString mRootPath;
    QString mInputPath;
    QString mOutputPath;
    QProcess* mpRodSystemSolver = nullptr;
    QProcess* mpOptimizationSolver = nullptr;
};

}



#endif // SOLUTIONMANAGER_H
