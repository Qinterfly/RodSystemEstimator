/*!
 * \file
 * \author Pavel Lakiza
 * \date July 2022
 * \brief Specification of the record types in the KLP file
 */

#ifndef RECORDTYPE_H
#define RECORDTYPE_H

namespace KLP
{

//! Types of records in a KLP file
enum RecordType
{
    R     = 2,  // Rods
    Xi    = 3,  // Parameter
    S     = 4,  // Natural length
    SS    = 5,  // Accumulated natural length
    X1    = 6,  // Coordinate X1
    X2    = 7,  // Coordinate X2
    X3    = 8,  // Coordinate X3
    U     = 9,  // State vector: [U1, U2, U3, w1, w2, w3, Q1, Q2, Q3, M1, M2, M3]
    Ut    = 10, // First-order derivative of the state vector with respect to time
    Utt   = 11, // Second-order derivative of the state vector with respect to time
    Eps   = 12, // Strain
    Ul    = 13, // Projected state vector: [U1L, U2L, U3L, w1, w2, w3, Q1L, Q2L, Q3L, M1L, M2L, M3L]
    Beta  = 15, // Rotation matrix
    Qm    = 16, // Loading factors
    qm    = 17, // Distributed loading factors
    AE    = 18, // Aerodynamic
    MF    = 19, // Modal frequencies
    MV    = 20, // Modal vectors
    ND    = 21, // Nondimensional coefficients
    FM    = 22, // Finite element model
    ERR   = 23, // Computational errors of the state vector
    MASS  = 24, // Total mass and the center of gravity
    RMASS = 25, // Masses of rods
    IP    = 26, // Cross sections
    CSM   = 27, // ?
    CS    = 28, // ?
    CSP   = 29, // ?
    CSE   = 30, // ?
    CSG   = 31, // ?
    FI    = 32, // Finite element image: set of coordinates (X, Y, Z) to plot lines
    FM2   = 33, // ?
    EM    = 34, // Effective masses
    EN    = 35, // Energy
    SIZE        // Maximal size
};

}

#endif // RECORDTYPE_H
