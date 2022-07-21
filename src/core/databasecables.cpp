/*!
 * \file
 * \author Pavel Lakiza
 * \date July 2022
 * \brief Definition of the DataBaseCables class
 */

#include <QFile>
#include <QTextStream>
#include "databasecables.h"

using namespace RSE::Core;

DataBaseCables::DataBaseCables(QString const& directory, QString const& fileName)
{
    readDataBase(directory + fileName);
}

//! Names of available cables
std::vector<std::string> DataBaseCables::names() const
{
    std::vector<std::string> keys;
    for (auto const& item : mData)
        keys.push_back(item.first);
    return keys;
}

//! Read a database from a file
bool DataBaseCables::readDataBase(QString const& pathFile)
{
    // IO constants
    QString const kDelimiter = "\t";

    // Mechanical constants
    double const kYoungsModulusSteel    = 2.1e11;
    double const kYoungsModulusAluminum = 0.7e11;
    double const kPoissonRatioSteel     = 0.3;
    double const kPoissonRatioAluminum  = 0.3;

    // Compute shear modulus for an isotropic material
    double kShearModulusSteel = kYoungsModulusSteel / (2.0 * (1.0 + kPoissonRatioSteel));
    double kShearModulusAluminum = kYoungsModulusAluminum / (2.0 * (1.0 + kPoissonRatioAluminum));

    // Open a file for reading
    QFile file(pathFile);
    if (!file.open(QIODeviceBase::ReadOnly))
        return false;
    QTextStream stream(&file);
    QString line;
    QStringList parts;
    while (stream.readLineInto(&line))
    {
        // Read data of a cable
        parts = line.split(kDelimiter);
        // Number of wires
        int numSteelWires    = parts[2].toInt();
        int numAluminumWires = parts[3].toInt();
        // Diameter of steel and aluminum wires
        double diameterSteelWire    = parts[4].toDouble() * 1e-3;
        double diameterAluminumWire = parts[5].toDouble() * 1e-3;
        // Bending stiffness of one wire
        double bendingStiffnessSteelWire    = kYoungsModulusSteel * M_PI * pow(diameterSteelWire, 4.0) / 64.0;
        double bendingStiffnessAluminumWire = kYoungsModulusAluminum * M_PI * pow(diameterAluminumWire, 4.0) / 64.0;
        // Approximate bending stiffness of a cable
        double bendingStiffness = numSteelWires * bendingStiffnessSteelWire + numAluminumWires * bendingStiffnessAluminumWire;
        // Torsional stiffness of one wire
        double torsionalStiffnessSteelWire    = kShearModulusSteel * M_PI * pow(diameterSteelWire, 4.0) / 32.0;
        double torsionalStiffnessAluminumWire = kShearModulusAluminum * M_PI * pow(diameterAluminumWire, 4.0) / 32.0;
        // Torsional stiffness of one wire
        double torsionalStiffness = numSteelWires * torsionalStiffnessSteelWire + numAluminumWires * torsionalStiffnessAluminumWire;
        // Youngs modulus
        double youngsModulus = parts[7].toDouble() * 1e7;
        // Area
        double area = parts[6].toDouble() * 1e-6;
        // Mass per length
        double massPerLength = parts[1].toDouble() * 1e-3;
        // Create a cable
        std::string name = parts[0].toStdString();
        Cable cable{name, bendingStiffness, torsionalStiffness, massPerLength, youngsModulus, area};
        mData.emplace(name, cable);
    }
    file.close();
    return true;
}
