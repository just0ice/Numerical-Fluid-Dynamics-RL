// SETBCOND and SETSPECBCOND
#include "grid.h"

void grid::SETBCOND(){
    // set boundary conditions according to wW, wE, wN and wS
    // 1 free-slip, 2 no-slip, 3 outflow, 4 periodic. see p. 41, to be added: Inflow condition
    // indexing: U[i + (imax+2)*j]

    switch (wW){
    case 1:
        // No-slip
        for (auto j=1; j != jmax+1; ++j){
            // 3.21
            U[imax + (imax+2)*j] = 0;
            // 3.23
            V[(imax+1) + (imax+2)*j] = - V[imax + (imax+2)*j];
        }
        break;
    case 2:
        // Free-slip
        for (auto j=1; j != jmax+1; ++j){
            // 3.24
            U[imax + (imax+2)*j] = 0;
            // 3.25
            V[(imax+1) + (imax+2)*j] = V[imax + (imax+2)*j];
        }
        break;
    case 3:
        // out-flow
        for (auto j=1; j != jmax+1; ++j){
            // 3.26
            U[imax + (imax+2)*j] = U[(imax-1) + (imax+2)*j];
            V[(imax+1) + (imax+2)*j] = V[imax + (imax+2)*j];
        }
        break;
    case 4:
        // periodic, to be added 
        // if wE = wW: full set of eq. 3.27 . if not -> error
        cout << "Periodic boundary condition not yet implemented" << endl;
        break;
    case 5:
        // inflow, to be added
        cout << "Inflow condition not yet implemented" << endl;
        break;
    default:
        cout << "Faulty boundary condidtion for wW" << endl;
        break;
    }

    switch (wE){
    case 1:
        // No-slip
        for (auto j=1; j != jmax+1; ++j){
            // 3.21
            U[0 + (imax+2)*j] = 0;
            // 3.23
            V[0 + (imax+2)*j] = - V[1 + (imax+2)*j];
        }
        break;
    case 2:
        // Free-slip
        for (auto j=1; j != jmax+1; ++j){
            // 3.24
            U[0 + (imax+2)*j] = 0;
            // 3.25
            V[0 + (imax+2)*j] = V[1 + (imax+2)*j];
        }
        break;
    case 3:
        // out-flow
        for (auto j=1; j != jmax+1; ++j){
            // 3.26
            U[0 + (imax+2)*j] = U[1 + (imax+2)*j];
            V[0 + (imax+2)*j] = V[1 + (imax+2)*j];
        }
        break;
    case 4:
        // periodic, to be added 
        // if wE = wW: nothing, already set by wW. if not -> error
        cout << "Periodic boundary condition not yet implemented" << endl;
        break;
    case 5:
        // inflow, to be added
        cout << "Inflow condition not yet implemented" << endl;
        break;
    default:
        cout << "Faulty boundary condidtion for wE" << endl;
        break;
    }

    switch (wN){
    case 1:
        // No-slip
        for (auto i=1; i != imax+1; ++i){
            // 3.21
            V[i + (imax+2)*jmax] = 0;
            // 3.23
            U[i + (imax+2)*(jmax+1)] = - U[i + (imax+2)*jmax];
        }
        break;
    case 2:
        // Free-slip
        for (auto i=1; i != imax+1; ++i){
            // 3.24
            V[i + (imax+2)*jmax] = 0;
            // 3.25
            U[i + (imax+2)*(jmax+1)] = U[i + (imax+2)*jmax];
        }
        break;
    case 3:
        // out-flow
        for (auto i=1; i != imax+1; ++i){
            // 3.26
            U[i + (imax+2)*(jmax+1)] = U[i + (imax+2)*jmax];
            V[i + (imax+2)*jmax] = V[i + (imax+2)*(jmax-1)];
        }
        break;
    case 4:
        // periodic, to be added 
        // if wE = wW: nothing, already set by wW. if not -> error
        cout << "Periodic boundary condition not yet implemented" << endl;
        break;
    case 5:
        // inflow, to be added
        cout << "Inflow condition not yet implemented" << endl;
        break;
    default:
        cout << "Faulty boundary condidtion for wN" << endl;
        break;
    }

    switch (wN){
    case 1:
        // No-slip
        for (auto i=1; i != imax+1; ++i){
            // 3.21
            V[i + (imax+2)*0] = 0;
            // 3.23
            U[i + (imax+2)*0] = U[i + (imax+2)*1];
        }
        break;
    case 2:
        // Free-slip
        for (auto i=1; i != imax+1; ++i){
            // 3.24
            V[i + (imax+2)*0] = 0;
            // 3.25
            U[i + (imax+2)*0] = U[i + (imax+2)*1];
        }
        break;
    case 3:
        // out-flow
        for (auto i=1; i != imax+1; ++i){
            // 3.26
            U[i + (imax+2)*0] = U[i + (imax+2)*1];
            V[i + (imax+2)*0] = V[i + (imax+2)*1];
        }
        break;
    case 4:
        // periodic, to be added 
        // if wE = wW: nothing, already set by wW. if not -> error
        cout << "Periodic boundary condition not yet implemented" << endl;
        break;
    case 5:
        // inflow, to be added
        cout << "Inflow condition not yet implemented" << endl;
        break;
    default:
        cout << "Faulty boundary condidtion for wN" << endl;
        break;
    }

}