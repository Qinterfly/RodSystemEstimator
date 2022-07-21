/*!
 * \file
 * \author Pavel Lakiza
 * \date July 2022
 * \brief Declaration of the SolutionOptions class
 */

#ifndef SOLUTIONOPTIONS_H
#define SOLUTIONOPTIONS_H

namespace RSE
{

namespace Solution
{

class SolutionOptions
{
public:
    SolutionOptions() = default;
    SolutionOptions(int numCalcModes, int numDampModes, int stepModes, double tolTrunc);
    ~SolutionOptions() = default;
    // Get parameters
    int numCalcModes() const { return mNumCalcModes; }
    int numDampModes() const { return mNumDampModes; }
    int stepModes() const { return mStepModes; }
    double tolTrunc() const { return mTolTrunc; }
    // Set parameters
    void setNumCalcModes(int numCalcModes) { mNumCalcModes = numCalcModes; }
    void setNumDampModes(int numDampModes) { mNumDampModes = numDampModes; }
    void setStepModes(int stepModes) { mStepModes = stepModes; }
    void setTolTrunc(double tolTrunc) { mTolTrunc = tolTrunc; }

private:
    //! Number of computational modes
    int mNumCalcModes;
    //! Number of modes to be damped
    int mNumDampModes;
    //! Step through computational modes
    int mStepModes;
    //! Limit to truncate computational modes
    double mTolTrunc;
};

}

}


#endif // SOLUTIONOPTIONS_H
