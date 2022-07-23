/*!
 * \file
 * \author Pavel Lakiza
 * \date July 2022
 * \brief Declaration of the IO class
 */

#ifndef IO_H
#define IO_H

#include <QString>
#include <QPair>
#include "solutionoptions.h"

namespace RSE
{

namespace Core
{

class Project;
class DataBaseCables;

using IOPair = QPair<Project*, RSE::Solution::SolutionOptions*>;

//! Class to save the project and solution data
class IO
{
public:
    IO(QString const& lastPath);
    ~IO() = default;
    QString const& lastPath() const { return mLastPath; }
    QString const& extension() const { return mkProjectExtension; }
    void saveAs(QString const& pathFile, Project& project, Solution::SolutionOptions& options);
    IOPair open(QString const& pathFile, DataBaseCables const& dataBaseCables);

private:
    const QString mkProjectExtension = ".rse";
    QString mLastPath;
};

}

}

#endif // IO_H
