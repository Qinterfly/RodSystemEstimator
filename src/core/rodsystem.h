/*!
 * \file
 * \author Pavel Lakiza
 * \date July 2022
 * \brief Declaration of the RodSystem class
 */

#ifndef RODSYSTEM_H
#define RODSYSTEM_H

#include <QString>
#include <vector>
#include <gsl/gsl_vector.h>

namespace RSE::Core
{

struct Cable;

//! Computed parameters of spans
struct Spans
{
    Spans(int numRods) : u0(numRods), uL(numRods), L(numRods) { }

    //! Constant at the left end
    std::vector<double> u0;
    //! Constant at the right end
    std::vector<double> uL;
    //! Length of a rod, m
    std::vector<double> L;
    //! Projected stretching force, N
    double projectedForce;
};

//! Parameters of a rod system
struct RodSystemParameters
{
    //! Distance between supports, m
    std::vector<double> distances;
    //! Mass per length, kg
    double massPerLength;
    //! Youngs modulus, Pa
    double youngsModulus;
    //! Area of a cross-section, m^2
    double area;
    //! Stretching force, N
    double force;
    //! Number of rods
    int numRods = 0;
};

class RodSystem
{
public:
    RodSystem(std::vector<double> distances, Cable const& cable, double force);
    // Get parameters of a system
    std::vector<double> const& distances() const { return mParameters.distances; }
    std::string const& nameCable() const { return mNameCable; }
    double force() const { return mParameters.force; }
    int numRods() const { return mParameters.numRods; }
    double massPerLength() const { return mParameters.massPerLength; }
    // Set parameters of a system
    void setDistances(std::vector<double> const& distances);
    void setCable(Cable const& cable);
    void setForce(double force) { mParameters.force = force; };
    // Compute parameters of spans
    Spans computeSpans();

private:
    RodSystemParameters mParameters;
    std::string mNameCable;
};

}

#endif // RODSYSTEM_H
