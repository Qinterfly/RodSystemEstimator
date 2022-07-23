/*!
 * \file
 * \author Pavel Lakiza
 * \date July 2022
 * \brief Declaration of the DataBaseCables class
 */

#ifndef DATACABLES_H
#define DATACABLES_H

#include <QString>
#include <unordered_map>

namespace RSE::Core
{

//! Mechanical properties of a cable
struct Cable
{
    //! Name of a cable
    std::string name;
    //! Bending stiffness, N
    double bendingStiffness;
    //! Torsional stiffness, N
    double torsionalStiffness;
    //! Mass per length, kg/m
    double massPerLength;
    //! Youngs modulus, Pa
    double youngsModulus;
    //! Area of a cross-section, m^2
    double area;
};

//! Aggregate data of cables
class DataBaseCables
{
public:
    DataBaseCables(QString const& directory, QString const& fileName);
    ~DataBaseCables() = default;
    std::vector<std::string> names() const;
    Cable const& getItem(std::string const& name) const { return mData.at(name); }

private:
    bool readDataBase(QString const& pathFile);

private:
    std::unordered_map<std::string, Cable> mData;
};

}

#endif // DATACABLES_H
