/*!
 * \file
 * \author Pavel Lakiza
 * \date July 2022
 * \brief Declaration of the Project class
 */

#ifndef PROJECT_H
#define PROJECT_H

#include <QString>
#include "abstractdataobject.h"
#include "damper.h"
#include "rodsystem.h"
#include "support.h"
#include "databasecables.h"

namespace RSE
{

namespace Solution
{
class SolutionOptions;
}

namespace Core
{
class ScalarDataObject;
class VectorDataObject;

using DataObjects = std::vector<AbstractDataObject*>;

class Project
{
public:
    Project(QString const& name, DataBaseCables dataBaseCables, Damper damper, RodSystem rodSystem, Support support);
    QString const& name() const { return mName; }
    void setName(QString const& name) { mName = name; }
    Damper& damper() { return mDamper; }
    RodSystem& rodSystem() { return mRodSystem; }
    Support& support() { return mSupport; }
    DataBaseCables const& dataBaseCables() const { return mDataBaseCables; }
    // IO
    void readTemplateData(QString const& path);
    void writeCalcData(QString const& path, Solution::SolutionOptions const& options);

private:
    AbstractDataObject* addDataObject(AbstractDataObject::ObjectType type);
    void importDataObjects(QString const& path, QString const& fileName);
    void readProjectID(QString const& path);
    // Modify data objects
    void modifyScalarDataObjects();
    void modifyVectorDataObjects(Spans const& spans);
    // IO
    void writeDataObjects(DataObjects const& dataObjects, QString const& path, QString const& fileName);
    void writeRods(QString const& path, QString const& fileName);
    void writeProgram(QString const& path, QString const& fileName, int numRods, int numCalcModes);

private:
    //! Name of a project
    QString mName;
    //! Parameters of a damper
    Damper mDamper;
    //! Parameters of a rod system
    RodSystem mRodSystem;
    //! Parameters of supports
    Support mSupport;
    //! Database of cables
    DataBaseCables mDataBaseCables;
    //! Data objects
    DataObjects mScalarDataObjects;
    DataObjects mVectorDataObjects;
    //! Project identifier
    int mProjectID;
    //! Content of the file named RODS
    QStringList mRods;
    //! Content of the file name PROG
    QStringList mProgram;
    //! Project extension
    static const QString skProjectExtension;
};

}

}

#endif // PROJECT_H
