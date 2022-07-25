/*!
 * \file
 * \author Pavel Lakiza
 * \date July 2022
 * \brief Definition of the Project class
 */

#include <QFile>
#include <QLocale>
#include "project.h"
#include "scalardataobject.h"
#include "vectordataobject.h"
#include "fileutilities.h"
#include "solutionoptions.h"

using namespace RSE::Core;
using namespace RSE::Solution;

static const QString skFileNameScalar  = "w1.prn";
static const QString skFileNameVector  = "w3.prn";
static const QString skFileNameProject = "PROJ_ID.prn";
static const QString skFileNameRods    = "RODS.prn";
static const QString skFileNameProgram = "PROG.prn";
static const int     skFieldWidth      = 30;

void clearDataObjects(DataObjects& dataObjects);
QStringList readAllLines(QString const& path, QString const& fileName);
void replaceStringEntry(QString& string, int numSkipEntries, QString subString);
void writeAllLines(QStringList const& lines, QString const& path, QString const& fileName);

Project::Project(QString const& name, DataBaseCables dataBaseCables, Damper damper, RodSystem rodSystem, Support support)
    : mName(name), mDamper(damper), mRodSystem(rodSystem), mSupport(support), mDataBaseCables(dataBaseCables)
{

}

Project::~Project()
{
    clearDataObjects(mScalarDataObjects);
    clearDataObjects(mVectorDataObjects);
}

//! Read template data
void Project::readTemplateData(QString const& path)
{
    // Import data objects
    mScalarDataObjects.clear();
    mVectorDataObjects.clear();
    importDataObjects(path, skFileNameScalar);
    importDataObjects(path, skFileNameVector);
    // Set the project identifier
    readProjectID(path);
    // Read the file named RODS
    mRods = readAllLines(path, skFileNameRods);
    // Read the file named PROG
    mProgram = readAllLines(path, skFileNameProgram);
}

//! Create a data object with the specified type
AbstractDataObject* Project::addDataObject(AbstractDataObject::ObjectType type)
{
    QString name;
    AbstractDataObject* pObject = nullptr;
    switch (type)
    {
    case AbstractDataObject::ObjectType::kScalar:
        name = "Scalar " + QString::number(ScalarDataObject::numberInstances() + 1);
        pObject = new ScalarDataObject(name);
        mScalarDataObjects.push_back(pObject);
        break;
    case AbstractDataObject::ObjectType::kVector:
        name = "Vector " + QString::number(VectorDataObject::numberInstances() + 1);
        pObject = new VectorDataObject(name);
        mVectorDataObjects.push_back(pObject);
        break;
    default:
        break;
    }
    return pObject;
}

//! Import several data objects from a file
void Project::importDataObjects(QString const& path, QString const& fileName)
{
    auto [type, pFile] = Utilities::File::getDataObjectFile(path, fileName);
    if (pFile == nullptr)
        return;
    QTextStream stream(pFile.data());
    quint32 numDataObjects;
    stream.readLine();
    stream >> numDataObjects;
    stream.readLine();
    for (quint32 iDataObject = 0; iDataObject != numDataObjects; ++iDataObject)
    {
        AbstractDataObject* pDataObject = addDataObject(type);
        pDataObject->import(stream);
    }
    pFile->close();
}

//! Read the identifier of a project
void Project::readProjectID(QString const& path)
{
    QFile file(path + skFileNameProject);
    if (!file.open(QIODeviceBase::ReadOnly))
        return;
    QTextStream stream(&file);
    qint64 id;
    stream >> id;
    mProjectID = id;
    file.close();
}

//! Write the computational data
void Project::writeCalcData(QString const& path, SolutionOptions const& options)
{
    Spans spans = mRodSystem.computeSpans();
    // Modify the data objects
    modifyScalarDataObjects();
    modifyVectorDataObjects(spans);
    // Write the data objects
    writeDataObjects(mScalarDataObjects, path, skFileNameScalar);
    writeDataObjects(mVectorDataObjects, path, skFileNameVector);
    // Rewrite the data of the rods and program
    writeRods(path, skFileNameRods);
    writeProgram(path, skFileNameProgram, mRodSystem.numRods(), options.numCalcModes());
}

//! Modify scalar data objects
void Project::modifyScalarDataObjects()
{
    // Изменение объектов
    Cable const& cable = mDataBaseCables.getItem(mRodSystem.nameCable());
    // EJ
    mScalarDataObjects[0]->setArrayValue(0.0, cable.bendingStiffness);
    // GJ
    mScalarDataObjects[1]->setArrayValue(0.0, cable.torsionalStiffness);
    // EF
    mScalarDataObjects[3]->setArrayValue(0.0, cable.youngsModulus * cable.area);
    // rho * F
    mScalarDataObjects[4]->setArrayValue(0.0, cable.massPerLength);
    // kSpring * l
    mScalarDataObjects[5]->setArrayValue(0.0, mDamper.springLength() * mDamper.springStiffness());
}

//! Modify vector data objects
void Project::modifyVectorDataObjects(Spans const& spans)
{
    int kNumElements = 3;
    // Coordinates of cables
    int numRods = mRodSystem.numRods();
    AbstractDataObject* pRodCoordinates = mVectorDataObjects[0];
    double sumLength = 0.0;
    int k = 1;
    for (int i = 0; i != numRods; ++i)
    {
        sumLength += spans.L[i];
        pRodCoordinates->setArrayValue(k, sumLength);
        pRodCoordinates->setArrayValue(k + 1, sumLength);
        k += 2;
    }
    // Coordinates of devices
    AbstractDataObject* pDeviceCoordinates = mVectorDataObjects[1];
    k = 0;
    for (int i = 0; i != numRods - 1; ++i)
    {
        sumLength = pRodCoordinates->arrayValue(k + 1);
        pDeviceCoordinates->setArrayValue(k, sumLength);
        pDeviceCoordinates->setArrayValue(k + 1, sumLength);
        pDeviceCoordinates->setArrayValue(k + 1, mDamper.springLength(), 0, 1);
        k += 2;
    }
    // Stiffness of the right support
    double C1 = mSupport.longitudinalStiffness();
    double C2 = mSupport.verticalStiffness();
    int iRight = (numRods - 1) * 2 + 1;
    double newKey = iRight;
    AbstractDataObject* pRightSupport = mVectorDataObjects[5];
    double oldKey = pRightSupport->keys()[0];
    pRightSupport->changeItemKey(oldKey, newKey);
    pRightSupport->setArrayValue(newKey, C2);
    // Longitudinal stiffness
    AbstractDataObject* pLongitudinalStiffness = mVectorDataObjects[6];
    std::vector<double> keys = pLongitudinalStiffness->keys();
    for (auto const& key : keys)
        pLongitudinalStiffness->setArrayValue(key, C1);
    // Rest stiffness
    AbstractDataObject* pVerticalStiffness = mVectorDataObjects[7];
    keys = pVerticalStiffness->keys();
    for (auto const& key : keys)
    {
        for (int j = 1; j != kNumElements; ++j)
            pVerticalStiffness->setArrayValue(key, C2, 0, j);
    }
    // Stretrching force
    AbstractDataObject* pForce = mVectorDataObjects[8];
    keys = pForce->keys();
    pForce->setArrayValue(keys[0], -spans.projectedForce);
    pForce->setArrayValue(keys[1],  spans.projectedForce);
    pForce->changeItemKey(keys[1], newKey);
    // Stiffness of the supports located at the both ends
    AbstractDataObject* pRestStiffness = mVectorDataObjects[9];
    keys = pRestStiffness->keys();
    for (auto const& key : keys)
    {
        for (int j = 0; j != kNumElements; ++j)
            pRestStiffness->setArrayValue(key, C2, 0, j);
    }
    pRestStiffness->setArrayValue(keys[1], 0.0);
    pRestStiffness->changeItemKey(keys[1], newKey);
}

//! Write data objects to a file
void Project::writeDataObjects(DataObjects const& dataObjects, QString const& path, QString const& fileName)
{
    // Open a file for writing
    QFile file(path + fileName);
    if (!file.open(QIODeviceBase::WriteOnly))
        return;
    QTextStream stream(&file);
    stream.setFieldAlignment(QTextStream::AlignRight);
    stream.setFieldWidth(skFieldWidth);
    // 1. Identifier of the project
    stream << mProjectID;
    stream << Qt::endl;
    // 2. Number of scalar data objects
    stream << dataObjects.size();
    stream << Qt::endl;
    // 3. Data of objects
    for (auto const& item : dataObjects)
        item->write(stream);
    file.close();
}

//! Write data of rods
void Project::writeRods(QString const& path, QString const& fileName)
{
    int NP = (mRodSystem.numRods() - 1) * 3 + 1;
    // Substitute the number of rods
    QString subString = QString::number(NP);
    QString& line = mRods[1];
    replaceStringEntry(line, 0, subString);
    // Save the change
    writeAllLines(mRods, path, fileName);
}

//! Write data of a program
void Project::writeProgram(QString const& path, QString const& fileName, int numRods, int numModes)
{
    QString subString;
    // Substitue the boundary condition for the last rod
    int NR = ((numRods - 1) * 3 + 1);
    subString = QString::number(NR);
    replaceStringEntry(mProgram[8], 1, subString);
    // Substitute the number of computational modes
    subString = QString::number(numModes);
    replaceStringEntry(mProgram[15], 1, subString);
    // Modify the type of the boundary condition
    int iBoundary = 3;
    if (numRods == 1)
        iBoundary = 7; // Forbid the rotation along the axis
    subString = QString::number(iBoundary);
    replaceStringEntry(mProgram[8], 3, subString);
    // Save the changes
    writeAllLines(mProgram, path, fileName);
}

//! Replace a substring after specified number of skips
void replaceStringEntry(QString& string, int numSkipEntries, QString subString)
{
    int lenString = string.size();
    // Find the substring started at the specfied position
    int iEntry = -1;
    int iStartSubString = 0;
    for (int i = 0; i != lenString; ++i)
    {
        // Find the beginning of the substring
        if (iStartSubString == 0 && !string[i].isSpace())
            iStartSubString = i;
        // Skip specified number of entries
        if (iStartSubString > 0 && string[i].isSpace())
        {
            if (numSkipEntries == 0)
            {
                iEntry = iStartSubString;
                break;
            }
            iStartSubString = 0;
            --numSkipEntries;
        }
    }
    if (iEntry < 0)
        return;
    // Erase the entry
    int iChar = iEntry;
    while (string[iChar] != ' ')
    {
        string[iChar] = ' ';
        ++iChar;
    }
    // Replace skips with the substring
    int lenSubString = subString.length();
    iChar = iEntry;
    for (int k = 0; k != lenSubString; ++k)
    {
        string[iChar] = subString[k];
        ++iChar;
    }
}

//! Read all the lines from a file
QStringList readAllLines(QString const& path, QString const& fileName)
{
    QStringList lines;
    QFile file(path + fileName);
    if (!file.open(QIODeviceBase::ReadOnly))
        return lines;
    QTextStream stream(&file);
    QString tString;
    while (stream.readLineInto(&tString))
        lines.push_back(tString);
    file.close();
    return lines;
}

//! Write all the lines to a file
void writeAllLines(QStringList const& lines, QString const& path, QString const& fileName)
{
    QFile file(path + fileName);
    if (!file.open(QIODeviceBase::WriteOnly))
        return;
    QTextStream stream(&file);
    for (auto const& line : lines)
        stream << line << Qt::endl;
    file.close();
}

//! Helper function to clear a container consisted of pointers to data objects
void clearDataObjects(DataObjects& dataObjects)
{
    for (auto iter = dataObjects.begin(); iter != dataObjects.end(); ++iter)
        delete *iter;
    dataObjects.clear();
}
