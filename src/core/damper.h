/*!
 * \file
 * \author Pavel Lakiza
 * \date July 2022
 * \brief Declaration the Damper class
 */

#ifndef DAMPER_H
#define DAMPER_H

#include <QPair>

namespace RSE::Core
{

//! Class to compute and collect properties of a damper
class Damper
{
public:
    Damper(double massCable, double massLoadedCable, double workingLength, double bouncerLength,
           double springLength = 0, double springStiffness = 0);
    ~Damper() = default;
    // Get parameteres of damper
    double massCable() const { return mMassCable; }
    double massLoadedCable() const { return mMassLoadedCable; }
    double workingLength() const { return mWorkingLength; }
    double bouncerLength() const { return mBouncerLength; }
    double springLength() const { return mSpringLength; }
    double springStiffness() const { return mSpringStiffness; }
    // Set parameters of a damper
    void setMassCable(double massCable) { mMassCable = massCable; }
    void setMassLoadedCable(double massLoadedCable) { mMassLoadedCable = massLoadedCable; }
    void setWorkingLength(double workingLength) { mWorkingLength = workingLength; }
    void setBouncerLength(double bouncerLength) { mBouncerLength = bouncerLength; }
    void setSpringLength(double springLength) { mSpringLength = springLength; }
    void setSpringStiffness(double springStiffness) { mSpringStiffness = springStiffness; }
    // Compute characteristics of a damper
    void computeSpring();

private:
    //! Mass of a cable, kg
    double mMassCable;
    //! Mass of a cable with ice on it, kg
    double mMassLoadedCable;
    //! Working length, m
    double mWorkingLength;
    //! Length of a bouncer, m
    double mBouncerLength;
    //! Length of a spring, m
    double mSpringLength = 0.0;
    //! Spring stiffness, N/m
    double mSpringStiffness = 0.0;
};

}

#endif // DAMPER_H
