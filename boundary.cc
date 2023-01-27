// SETBCOND and SETSPECBCOND
#include "grid.h"


void grid::SETBCOND2(){
    // SETBCOND for general geometries via flag array
    for (auto j = 0; j != jmax+2; ++j){
        for (auto i = 0; i != imax+2; ++i){
            switch (FLAG[id(i,j)])
            {
            // No-slip 2
            // fluid cells to the 19 East, 21 West, 23 North, 25 South, 31 NE, 27 NW, 29 SW, 33 SE
            case 21:
                U[id(i,j)] = 0;
                break;
            case 23:
                V[id(i,j)] = 0;
                break;
            case 19:
                U[id(i-1,j)] = 0;
                break;
            case 25:
                V[id(i,j-1)] = 0;
                break;
            case 31:
                U[id(i,j)] = 0;
                V[id(i,j)] = 0;
                break;
            case 27:
                V[id(i,j)] = 0;
                U[id(i-1,j)] = 0;
                break;
            case 29:
                U[id(i-1,j)] = 0;
                V[id(i,j-1)] = 0;
                break;
            case 33:
                U[id(i,j)] = 0;
                V[id(i,j-1)] = 0;
                break;

            // Free-slip 1
            // fluid cells to the 3 East, 5 West, 7 North, 9 South, 15 NE, 11 NW, 13 SW, 17 SE
            case 5:
                U[id(i,j)] = 0;
                break;
            case 3:
                U[id(i-1,j)] = 0;
                break;
            case 7:
                V[id(i,j)] = 0;
                break;
            case 9:
                V[id(i,j-1)] = 0;
                break;
            case 15:
                V[id(i,j)] = 0;
                U[id(i,j)] = 0;
                break;
            case 11:
                V[id(i,j)] = 0;
                U[id(i-1,j)] = 0;
                break;
            case 13:
                U[id(i-1,j)] = 0;
                V[id(i,j-1)] = 0;
                break;
            case 17:
                V[id(i,j-1)] = 0;
                U[id(i,j)] = 0;
                break;

            // Outflow 3
            // fluid cells to the 37 West, 35 East, 39 North, 41 South, 47 NE, 43 NW, 45 SW, 49 SE
            // WARNING! there might be an obstacle! seperation to bound should be at least 2 cells! To be safe, use this only as bcond for the domain walls
            case 35:
                U[id(i-1,j)] = U[id(i-2,j)];
                break;
            case 41:
                V[id(i,j-1)] = V[id(i,j-2)];
                break;
            case 43:
                U[id(i-1,j)] = U[id(i-2,j)];
                break;
            case 45:
                U[id(i-1,j)] = U[id(i-2,j)];
                V[id(i,j-1)] = V[id(i,j-2)];
                break;
            case 49:
                V[id(i,j-1)] = V[id(i,j-2)];
                break;
            default:
                break;
            }
        }
    }


    // second loop where assignment is dependant on neighboring values
    for (auto j = 0; j != jmax+2; ++j){
        for (auto i = 0; i != imax+2; ++i){
            switch (FLAG[id(i,j)])
            {
            // No-slip 2
            case 21:
                V[id(i,j)] = - V[id(i+1,j)];
                break;
            case 23:
                U[id(i,j)] = - U[id(i,j+1)];
                break;
            case 19:
                V[id(i,j)] = - V[id(i-1,j)];
                break;
            case 25:
                U[id(i,j)] = - U[id(i,j-1)];
                break;
            // higher cases redundant -> corner cells are 45° angles!

            // Free-slip 1
            // fluid cells to the 5 East, 3 West, 7 North, 9 South, 15 NE, 11 NW, 13 SW, 17 SE
            case 5:
                V[id(i,j)] = V[id(i+1,j)];
                break;
            case 3:
                V[id(i,j)] = V[id(i-1,j)];
                break;
            case 7:
                U[id(i,j)] = U[id(i,j+1)];
                break;
            case 9:
                U[id(i,j)] = U[id(i,j-1)];
                break;

            // Outflow 3
            // fluid cells to the 37 East, 35 West, 39 North, 41 South, 47 NE, 43 NW, 45 SW, 49 SE
            // WARNING! there might be an obstacle! seperation to bound should be at least 2 cells! To be safe, use this only as bcond for the domain walls
            case 35:
                V[id(i,j)] = V[id(i-1,j)];
                break;
            case 41:
                U[id(i,j)] = U[id(i,j-1)];
                break;
            case 37:
                U[id(i,j)] = U[id(i+1,j)];
                V[id(i,j)] = V[id(i+1,j)];
                break;
            case 39:
                U[id(i,j)] = U[id(i,j+1)];
                V[id(i,j)] = V[id(i,j+1)];
                break;
            case 47:
            case 43:
            case 45:
            case 49:
                cout << "ERROR! Outflow does not work for convex corners!" << endl;
                break;
            default:
                break;
            }
        }
    }
}

void grid::SETBCOND(){
    // set boundary conditions according to wW, wE, wN and wS
    // 1 free-slip, 2 no-slip, 3 outflow, 4 periodic. see p. 41, to be added: Inflow condition
    // WARNING! Numbers switched compared to pp. 30. It's an inconsistency in the book :(
    // indexing: U[i + (imax+2)*j]

    // Big Problem: When setting boundary conditions sometimes values are used that should already have been asigned 
    // by other boundary conditions (for example example V for no-slip and j=jmax at wE and wW)
    // The problem only seems to arise from the formulae for wW and wE
    // To prevent this, the order of assignment is important (therefore its split into two switches here)
    // void grid::CHECKBCOND() checks if all boundary equations are fulifilled simultaniously 


    // first assignment /////////////////////////////////////////////////////////////////////////////////////////////////
    switch (wE){
    case 2:
        // No-slip
        for (auto j=1; j != jmax+1; ++j){
            // 3.21
            U[imax + (imax+2)*j] = 0;
        }
        break;
    case 1:
        // Free-slip
        for (auto j=1; j != jmax+1; ++j){
            // 3.24
            U[imax + (imax+2)*j] = 0;
        }
        break;
    case 3:
        // out-flow
        for (auto j=1; j != jmax+1; ++j){
            // 3.26
            U[imax + (imax+2)*j] = U[(imax-1) + (imax+2)*j];
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

    switch (wW){
    case 2:
        // No-slip
        for (auto j=1; j != jmax+1; ++j){
            // 3.21
            U[0 + (imax+2)*j] = 0;
        }
        break;
    case 1:
        // Free-slip
        for (auto j=1; j != jmax+1; ++j){
            // 3.24
            U[0 + (imax+2)*j] = 0;
        }
        break;
    case 3:
        // out-flow
        for (auto j=1; j != jmax+1; ++j){
            // 3.26
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
    case 2:
        // No-slip
        for (auto i=1; i != imax+1; ++i){
            // 3.21
            V[i + (imax+2)*jmax] = 0;
            // 3.23
            U[i + (imax+2)*(jmax+1)] = - U[i + (imax+2)*jmax];
        }
        break;
    case 1:
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
    case 2:
        // No-slip
        for (auto i=1; i != imax+1; ++i){
            // 3.21
            V[i + (imax+2)*0] = 0;
            // 3.23
            U[i + (imax+2)*0] = - U[i + (imax+2)*1];
        }
        break;
    case 1:
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

    // second assignment /////////////////////////////////////////////////////////////////////////////////////////////////
    switch (wW){
    case 2:
        // No-slip
        for (auto j=1; j != jmax+1; ++j){
            // 3.23
            V[(imax+1) + (imax+2)*j] = - V[imax + (imax+2)*j];
        }
        break;
    case 1:
        // Free-slip
        for (auto j=1; j != jmax+1; ++j){
            // 3.25
            V[(imax+1) + (imax+2)*j] = V[imax + (imax+2)*j];
        }
        break;
    case 3:
        // out-flow
        for (auto j=1; j != jmax+1; ++j){
            // 3.26
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
    case 2:
        // No-slip
        for (auto j=1; j != jmax+1; ++j){
            // 3.23
            V[0 + (imax+2)*j] = - V[1 + (imax+2)*j];
        }
        break;
    case 1:
        // Free-slip
        for (auto j=1; j != jmax+1; ++j){
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
}

void check_cond(bool no_good, unsigned cond_val, string wall, string comp, unsigned index){
    if (no_good){
        string cond;

        switch (cond_val)
        {
        case 1:
            cond = "Free-slip";
            break;
        case 2:
            cond = "No-slip";
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
    // check boundary conditions simultaniously according to wW, wE, wN and wS
    // 1 free-slip, 2 no-slip, 3 outflow, 4 periodic. see p. 41, to be added: Inflow condition
    // indexing: U[i + (imax+2)*j]

    switch (wE){
    case 2:
        // No-slip
        for (auto j=1; j != jmax+1; ++j){
            // 3.21
            if (U[id(imax,j)] != 0) 
                cout << "Warning! No-slip boundary condition for U 3.21 not fulfilled at wW j = " << j << endl; 
            // 3.23
            if (V[id(imax+1,j)] != - V[id(imax,j)]) 
                cout << "Warning! No-slip boundary condition for V 3.23 not fulfilled at wW j = " << j << endl;
        }
        break;
    case 1:
        // Free-slip
        for (auto j=1; j != jmax+1; ++j){
            // 3.24
            if (U[imax + (imax+2)*j] != 0)
                cout << "Warning! Free-slip boundary condition for U 3.24 not fulfilled at wW j = " << j << endl;
            // 3.25
            if (V[(imax+1) + (imax+2)*j] != V[imax + (imax+2)*j])
                cout << "Warning! Free-slip boundary condition for V 3.25 not fulfilled at wW j = " << j << endl;
        }
        break;
    case 3:
        // Outflow
        for (auto j=1; j != jmax+1; ++j){
            // 3.26
            if (U[id(imax,j)] != U[id(imax-1,j)])
                cout << "Warning! Outflow boundary condition for U 3.26 not fulfilled at wW j = " << j << endl;
            if (V[id(imax+1,j)] != V[id(imax,j)])
                cout << "Warning! Outflow boundary condition for V 3.26 not fulfilled at wW j = " << j << endl;
        }
        break;
    default:
        break;
    }

    switch (wW){
    case 2:
        // No-slip
        for (auto j=1; j != jmax+1; ++j){
            // 3.21
            if (U[id(0,j)] != 0)
                cout << "Warning! No-slip boundary condition for U 3.21 not fulfilled at wE j = " << j << endl; 
            // 3.23
            check_cond(V[id(0,j)] != - V[id(1,j)], wN, "wE", "V", j);
        }
        break;
    case 1:
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
            if (U[id(0,j)] != U[id(1,j)])
                cout << "Warning! Warning! Outflow boundary condition for U 3.26 not fulfilled at wE j = " << j << endl;
            if (V[id(0,j)] != V[id(1,j)])
                cout << "Warning! Outflow boundary condition for V 3.26 not fulfilled at wE j = " << j << endl;
        }
        break;
    default:
        break;
    }

    switch (wN){
    case 2:
        // No-slip
        for (auto i=1; i != imax+1; ++i){
            // 3.21
            check_cond(V[id(i,jmax)] != 0, wN, "wN", "V", i);
            // 3.23
            check_cond(U[id(i,jmax+1)] != - U[id(i,jmax)], wN, "wN", "U", i);
        }
        break;
    case 1:
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
            check_cond(U[id(i,jmax+1)] != U[id(i,jmax)], wN, "wN", "U", i);
            check_cond(V[id(i,jmax)] != V[id(i,jmax-1)], wN, "wN", "V", i);
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
    case 2:
        // No-slip
        for (auto i=1; i != imax+1; ++i){
            // 3.21
            check_cond(V[i + (imax+2)*0] != 0, wS, "wS", "V", i);
            // 3.23
            check_cond(U[i + (imax+2)*0] != - U[i + (imax+2)*1], wS, "wS", "U", i);
        }
        break;
    case 1:
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
            check_cond(V[i + (imax+2)*0] != V[i + (imax+2)*1], wS, "wS", "U", i);
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