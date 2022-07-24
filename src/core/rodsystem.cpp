/*!
 * \file
 * \author Pavel Lakiza
 * \date July 2022
 * \brief Definition of the RodSystem class
 */

#include <functional>
#include <stdlib.h>
#include <stdio.h>
#include <gsl/gsl_multiroots.h>
#include "rodsystem.h"
#include "constants.h"
#include "databasecables.h"

using IntegralFun = std::function<double(double)>;

using namespace RSE::Core;
using namespace RSE::Constants;

RodSystem::RodSystem(std::vector<double> distances, Cable const& cable, double force)
{
    mParameters.distances = distances;
    mParameters.force = force;
    mParameters.numRods = mParameters.distances.size();
    setCable(cable);
}

//! Compute integral using the MidPoint rule
double integrate(IntegralFun const& f, double const& a, double const& b, int const& n)
{
    if (n <= 0) return 0;
    double integral = 0;
    double h = (b - a) / n;
    for (int i = 0; i != n; ++i)
        integral += f(a + h * (i + 0.5));
    return integral * h;
}

double x1(double u, double u0, double uL)
{
    return (u - u0) / (sinh(uL) - sinh(u0));
}

double x2(double u, double u0, double uL)
{
    return (cosh(u) - cosh(u0)) / (sinh(uL) - sinh(u0));
}

double Q1(double u0, double uL)
{
    return 1.0 / (sinh(uL) - sinh(u0));
}

double Q2(double u, double u0, double uL)
{
    return sinh(u) / (sinh(uL) - sinh(u0));
}

double Nf(double u, double u0, double uL)
{
    return cosh(u) / (sinh(uL) - sinh(u0));
}

double LL(double L, double u0, double uL, RodSystemParameters const* pParameters)
{
    const int kNumIntegrationSteps = 1000;
    IntegralFun funSystem = [&pParameters, &u0, &uL, &L](double u)
    {
        double numerator = pow(Nf(u, u0, uL), 2.0) * pParameters->massPerLength * pow(L, 2.0) * kGravitationalAcceleration;
        double denominator = pParameters->youngsModulus * pParameters->area;
        return numerator / denominator;
    };
    return L + integrate(funSystem, u0, uL, kNumIntegrationSteps);
}

double projForce(double u0, double uL, double L, RodSystemParameters const* pParameters)
{
    return Q1(u0, uL) * pParameters->massPerLength * LL(L, u0, uL, pParameters) * kGravitationalAcceleration;
}

//! System of equations
int equations(const gsl_vector* pState, void* pVoidParameters, gsl_vector* pFun)
{
    RodSystemParameters* pParameters = (struct RodSystemParameters*) pVoidParameters;
    int numRods = pParameters->numRods;
    std::vector<double> rodsU0(numRods);
    std::vector<double> rodsUL(numRods);
    std::vector<double> rodsL(numRods);
    // Slice computation parameters
    int iLast = 0;
    for (int iRod = 0; iRod != numRods; ++iRod)
    {
        rodsU0[iRod] = gsl_vector_get(pState, iLast    );
        rodsUL[iRod] = gsl_vector_get(pState, iLast + 1);
        rodsL[iRod]  = gsl_vector_get(pState, iLast + 2);
        iLast += 3;
    }

    // First two groups of equations
    double u0, uL, L;
    iLast = 0;
    for (int iRod = 0; iRod != numRods; ++iRod)
    {
        // Slice current parameters
        uL = rodsUL[iRod];
        u0 = rodsU0[iRod];
        L  = rodsL[iRod];
        // Arrange equations
        gsl_vector_set(pFun, iLast,     x2(uL, u0, uL));
        gsl_vector_set(pFun, iLast + 1, x1(uL, u0, uL) * LL(L, u0, uL, pParameters) - pParameters->distances[iRod]);
        iLast += 2;
    }

    // The third group of equations
    double equ;
    // First rod
    u0 = rodsU0[0];
    uL = rodsUL[0];
    L  = rodsL[0];
    equ = Nf(u0, u0, uL) * pParameters->massPerLength * L * kGravitationalAcceleration - pParameters->force;
    gsl_vector_set(pFun, iLast, equ);
    if (numRods > 1)
    {
        ++iLast;
        // Second rod
        equ = Nf(uL, u0, uL) * L  - Nf(rodsU0[1], rodsU0[1], rodsUL[1]) * rodsL[1];
        gsl_vector_set(pFun, iLast, equ);
        ++iLast;
        // Rest
        for (int iRod = 1; iRod < numRods - 1; ++iRod, ++iLast)
        {
            equ =  Nf(rodsUL[iRod], rodsU0[iRod], rodsUL[iRod]) * rodsL[iRod]
                   - Nf(rodsU0[iRod + 1], rodsU0[iRod + 1], rodsUL[iRod + 1]) * rodsL[iRod + 1];
            gsl_vector_set(pFun, iLast, equ);
        }
    }
    return GSL_SUCCESS;
}

//! Compute characteristics of spans
Spans RodSystem::computeSpans()
{
    const int kMaxIter            = 1000;
    const double kTolResidual     = 1e-7;
    const double kApproxU0        = -1e-3;
    const double kApproxUL        = 1e-3;
    const double kApproxDeltaSpan = 1;

    // Create an object to aggregate the results
    int numRods = mParameters.numRods;
    Spans spans(numRods);
    if (numRods == 0)
        return Spans(numRods);

    // Set the solution function
    size_t n = 3 * numRods;
    gsl_multiroot_function f = {&equations, n, &mParameters};

    // Specify the initial values of parameters to be optimized
    gsl_vector* pState = gsl_vector_alloc(n);
    int iLast = 0;
    for (int iRod = 0; iRod != mParameters.numRods; ++iRod)
    {
        gsl_vector_set(pState, iLast, kApproxU0);
        gsl_vector_set(pState, iLast + 1, kApproxUL);
        gsl_vector_set(pState, iLast + 2, mParameters.distances[iRod] + kApproxDeltaSpan);
        iLast += 3;
    }

    // Compute the starting error
    size_t iter = 0;
    const gsl_multiroot_fsolver_type* T = gsl_multiroot_fsolver_hybrids;
    gsl_multiroot_fsolver* pSolver = gsl_multiroot_fsolver_alloc (T, n);
    gsl_multiroot_fsolver_set (pSolver, &f, pState);

    // Solve the system
    int status;
    do
    {
        ++iter;
        status = gsl_multiroot_fsolver_iterate(pSolver);
        // Check if the solution is obtained
        if (status)
            break;
        status = gsl_multiroot_test_residual(pSolver->f, kTolResidual);
    }
    while (status == GSL_CONTINUE && iter < kMaxIter);

    // Solve the solution
    iLast = 0;
    for (int iRod = 0; iRod != numRods; ++iRod)
    {
        spans.u0[iRod] = gsl_vector_get(pSolver->x, iLast);
        spans.uL[iRod] = gsl_vector_get(pSolver->x, iLast + 1);
        spans.L[iRod]  = gsl_vector_get(pSolver->x, iLast + 2);
        iLast += 3;
    }
    spans.projectedForce = projForce(spans.u0[0], spans.uL[0], spans.L[0], &mParameters);
    // Free the working objects
    gsl_multiroot_fsolver_free(pSolver);
    gsl_vector_free (pState);
    return spans;
}

//! Specify distances between supports
void RodSystem::setDistances(std::vector<double> const& distances)
{
    mParameters.distances = distances;
    mParameters.numRods = size(mParameters.distances);
}

//! Modify the cable used in the rod system
void RodSystem::setCable(Cable const& cable)
{
    mNameCable                = cable.name;
    mParameters.massPerLength = cable.massPerLength;
    mParameters.youngsModulus = cable.youngsModulus;
    mParameters.area          = cable.area;
}

