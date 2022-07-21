/*!
 * \file
 * \author Pavel Lakiza
 * \date July 2022
 * \brief Declaration of the Support class
 */

#ifndef SUPPORT_H
#define SUPPORT_H

namespace RSE::Core
{

//! Class to aggregate data of supports
class Support
{
public:
    Support(double longitudinalStiffness, double verticalStiffness);
    ~Support() = default;
    // Get characteristics
    double longitudinalStiffness() const { return mLongitudinalStiffness; }
    double verticalStiffness() const { return mVerticalStiffness; }
    // Set characteristics
    void setLongitudinalStiffness(double longitudinalStiffness) { mLongitudinalStiffness = longitudinalStiffness; }
    void setVerticalStiffness(double verticalStiffness) { mVerticalStiffness = verticalStiffness; }

private:
    //! Longitudinal stiffness (С1), N/m
    double mLongitudinalStiffness;
    //! Vertical stiffness (С2), N/m
    double mVerticalStiffness;
};

}

#endif // SUPPORT_H
