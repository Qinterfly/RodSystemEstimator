/*!
 * \file
 * \author Pavel Lakiza
 * \date July 2022
 * \brief Definition of the IO class
 */

#include <QFile>
#include <QFileInfo>
#include <QDir>
#include "io.h"
#include "project.h"

using namespace RSE::Core;
using namespace RSE::Solution;

IO::IO(QString const& lastPath)
    : mLastPath(lastPath)
{

}

//! Save the project and solution data to a file
void IO::saveAs(QString const& pathFile, Project& project, SolutionOptions& options)
{
    QFile file(pathFile);
    if (!file.open(QIODeviceBase::WriteOnly))
        return;
    QDataStream stream(&file);
    // 1. Write the project
    // Parameters of damper
    Damper const& damper = project.damper();
    stream << damper.massCable()
           << damper.massLoadedCable()
           << damper.workingLength()
           << damper.bouncerLength()
           << damper.springLength()
           << damper.springStiffness();
    // Parameters of a rod system
    RodSystem const& rodSystem = project.rodSystem();
    stream << (qint64)rodSystem.numRods();
    for (auto const& item : rodSystem.distances())
        stream << item;
    stream << rodSystem.force();
    stream << QString(project.rodSystem().nameCable().data());
    // Parameters of supports
    Support const& support = project.support();
    stream << support.longitudinalStiffness()
           << support.verticalStiffness();
    // 2. Write the computation parameters
    stream << options.numCalcModes() << options.numDampModes() << options.stepModes() << options.tolTrunc();
    file.close();
    // Save the last path used
    QString name = QFileInfo(pathFile).baseName();
    project.setName(name);
    mLastPath = QFileInfo(pathFile).dir().path() + '/';
}

//! Read the computational data from a file
IOPair IO::open(QString const& pathFile, DataBaseCables const& dataBaseCables)
{
    QFile file(pathFile);
    // Open a file for reading
    if (!file.open(QIODeviceBase::ReadOnly))
        return IOPair(nullptr, nullptr);
    QDataStream stream(&file);
    // 1. Read a project
    // Parameters of damper
    double massCable, massLoadedCable, workingLength, bouncerLength, springLength, springStiffness;
    stream >> massCable >> massLoadedCable >> workingLength >> bouncerLength >> springLength >> springStiffness;
    Damper damper(massCable, massLoadedCable, workingLength, bouncerLength, springLength, springStiffness);
    // Parameters of a rod system
    double tValue;
    qint64 numRods;
    stream >> numRods;
    std::vector<double> distances(numRods);
    for (quint64 i = 0; i != numRods; ++i)
    {
        stream >> tValue;
        distances[i] = tValue;
    }
    double force;
    stream >> force;
    QString nameCable;
    stream >> nameCable;
    Cable cable = dataBaseCables.getItem(nameCable.toStdString());
    RodSystem rodSystem(distances, cable, force);
    // Parameters of supports
    double longitudinalStiffness, verticalStiffness;
    stream >> longitudinalStiffness >> verticalStiffness;
    Support support(longitudinalStiffness, verticalStiffness);
    // 2. Read solution options
    int numCalcModes, numDampModes, stepModes;
    double tolTrunc;
    stream >> numCalcModes >> numDampModes >> stepModes >> tolTrunc;
    SolutionOptions* pSolutionOptions = new SolutionOptions(numCalcModes, numDampModes, stepModes, tolTrunc);
    // Create a project
    QString name = QFileInfo(pathFile).baseName();
    Project* pProject = new Project(name, dataBaseCables, damper, rodSystem, support);
    // Remember the last path used
    mLastPath = QFileInfo(pathFile).dir().path() + '/';
    return IOPair(pProject, pSolutionOptions);
}
