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

    switch (wS){
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

void check_cond(bool no_good, unsigned cond_val, string wall, string comp, unsigned index){
    if (no_good){
        string cond;

        switch (cond_val)
        {
        case 1:
            cond = "No-slip";
            break;
        case 2:
            cond = "Free-slip";
            break;
        case 3:
            cond = "Outflow";
            break;
        case 4:
            cond = "Periodic";
            break;
        
        default:
            cond = "Unrecognized condition";
            break;
        }

        cout << "Warning! " << cond << " boundary condition for "<< comp 
        << " not fulfilled at " << wall << " index = " << index << endl; 
    }
}

void grid::CHECKBCOND(){
    // check boundary conditions according to wW, wE, wN and wS
    // 1 free-slip, 2 no-slip, 3 outflow, 4 periodic. see p. 41, to be added: Inflow condition
    // indexing: U[i + (imax+2)*j]

    switch (wW){
    case 1:
        // No-slip
        for (auto j=1; j != jmax+1; ++j){
            // 3.21
            if (U[imax + (imax+2)*j] != 0) 
                cout << "Warning! No-slip boundary condition for U 3.21 not fulfilled at wW j = " << j << endl; 
            // 3.23
            if (V[(imax+1) + (imax+2)*j] != - V[imax + (imax+2)*j]) 
                cout << "Warning! No-slip boundary condition for V 3.23 not fulfilled at wW j = " << j << endl;
        }
        break;
    case 2:
        // Free-slip
        for (auto j=1; j != jmax+1; ++j){
            // 3.24
            if (U[imax + (imax+2)*j] != 0)
                cout << "Warning! Free-slip boundary condition for U 3.24 not fulfilled at wW j = " << j << endl;
            // 3.25
            if (V[(imax+1) + (imax+2)*j] = V[imax + (imax+2)*j])
                cout << "Warning! Free-slip boundary condition for V 3.25 not fulfilled at wW j = " << j << endl;
        }
        break;
    case 3:
        // Outflow
        for (auto j=1; j != jmax+1; ++j){
            // 3.26
            if (U[imax + (imax+2)*j] != U[(imax-1) + (imax+2)*j])
                cout << "Warning! Outflow boundary condition for U 3.26 not fulfilled at wW j = " << j << endl;
            if (V[(imax+1) + (imax+2)*j] != V[imax + (imax+2)*j])
                cout << "Warning! Outflow boundary condition for V 3.26 not fulfilled at wW j = " << j << endl;
        }
        break;
    default:
        break;
    }

    switch (wE){
    case 1:
        // No-slip
        for (auto j=1; j != jmax+1; ++j){
            // 3.21
            if (U[0 + (imax+2)*j] != 0)
                cout << "Warning! No-slip boundary condition for U 3.21 not fulfilled at wE j = " << j << endl; 
            // 3.23
            check_cond(V[0 + (imax+2)*j] != - V[1 + (imax+2)*j], wN, "wE", "V", j);
        }
        break;
    case 2:
        // Free-slip
        for (auto j=1; j != jmax+1; ++j){
            // 3.24
            if (U[0 + (imax+2)*j] != 0)
                cout << "Warning! Free-slip boundary condition for U 3.24 not fulfilled at wE j = " << j << endl; 
            // 3.25
            if (V[0 + (imax+2)*j] != V[1 + (imax+2)*j])
                cout << "Warning! Free-slip boundary condition for V 3.25 not fulfilled at wE j = " << j << endl;
        }
        break;
    case 3:
        // Outflow
        for (auto j=1; j != jmax+1; ++j){
            // 3.26
            if (U[0 + (imax+2)*j] != U[1 + (imax+2)*j])
                cout << "Warning! Warning! Outflow boundary condition for U 3.26 not fulfilled at wE j = " << j << endl;
            if (V[0 + (imax+2)*j] != V[1 + (imax+2)*j])
                cout << "Warning! Outflow boundary condition for V 3.26 not fulfilled at wE j = " << j << endl;
        }
        break;
    default:
        break;
    }

    switch (wN){
    case 1:
        // No-slip
        for (auto i=1; i != imax+1; ++i){
            // 3.21
            check_cond(V[i + (imax+2)*jmax] != 0, wN, "wN", "V", i);
            // 3.23
            check_cond(U[i + (imax+2)*(jmax+1)] != - U[i + (imax+2)*jmax], wN, "wN", "U", i);
        }
        break;
    case 2:
        // Free-slip
        for (auto i=1; i != imax+1; ++i){
            // 3.24
            check_cond(V[i + (imax+2)*jmax] != 0, wN, "wN", "V", i);
            // 3.25
            check_cond(U[i + (imax+2)*(jmax+1)] != U[i + (imax+2)*jmax], wN, "wN", "U", i);
        }
        break;
    case 3:
        // out-flow
        for (auto i=1; i != imax+1; ++i){
            // 3.26
            check_cond(U[i + (imax+2)*(jmax+1)] != U[i + (imax+2)*jmax], wN, "wN", "U", i);
            check_cond(V[i + (imax+2)*jmax] != V[i + (imax+2)*(jmax-1)], wN, "wN", "V", i);
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

    switch (wS){
    case 1:
        // No-slip
        for (auto i=1; i != imax+1; ++i){
            // 3.21
            check_cond(V[i + (imax+2)*0] != 0, wS, "wS", "V", i);
            // 3.23
            check_cond(U[i + (imax+2)*0] != U[i + (imax+2)*1], wS, "wS", "U", i);
        }
        break;
    case 2:
        // Free-slip
        for (auto i=1; i != imax+1; ++i){
            // 3.24
            check_cond(V[i + (imax+2)*0] != 0, wS, "wS", "V", i);
            // 3.25
            check_cond(U[i + (imax+2)*0] != U[i + (imax+2)*1], wS, "wS", "U", i);
        }
        break;
    case 3:
        // out-flow
        for (auto i=1; i != imax+1; ++i){
            // 3.26
            check_cond(U[i + (imax+2)*0] != U[i + (imax+2)*1], wS, "wS", "U", i);
            check_cond(V[i + (imax+2)*0] = V[i + (imax+2)*1], wS, "wS", "U", i);
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