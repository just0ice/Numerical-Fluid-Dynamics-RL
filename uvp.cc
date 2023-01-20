// COMP_FG, CMP_RHS, POISSON, and ADAP_UV
#include "grid.h"

void grid::CHECK_HIRT(){
    // Checking the Hirt condition 3.20 for gamma
    if (gamma < max(Umax * delt / delx, Vmax * delt / dely) )
        cout << "WARNING! Gamma violates Hirt condition (3.20). Choose a larger gamma in settings.in." << endl;
}

void grid::COMP_SPATIAL_DERIVATIVES(){
    // Computes the spatial derivatives according to 3.19
    
    CHECK_HIRT();

    // 3.19a
    for (auto j=1; j != jmax+1; ++j){
        for (auto i=1; i != imax; ++i){
            du2_dx[id(i,j)] = 
                    1/delx * ( pow( (U[id(i,j)] + U[id(i+1,j)])/2, 2) - pow( (U[id(i-1,j)] + U[id(i,j)])/2, 2) )
                    + gamma * 1/delx * ( abs(U[id(i,j)] + U[id(i+1,j)])/2 * (U[id(i,j)] - U[id(i+1,j)])/2  - 
                    abs(U[id(i-1,j)] + U[id(i,j)])/2 * (U[id(i-1,j)] - U[id(i,j)])/2 );
            duv_dy[id(i,j)] = 
                    1/dely * ( (V[id(i,j)] + V[id(i+1,j)])/2 * (U[id(i,j)] + U[id(i,j+1)])/2
                    - (V[id(i,j-1)] + V[id(i+1,j-1)])/2 * (U[id(i,j-1)] + U[id(i,j)])/2 )

                    + gamma * 1/dely * ( abs(V[id(i,j)] + V[id(i+1,j)])/2 * (U[id(i,j)] - U[id(i,j+1)])/2  
                    - abs(V[id(i,j-1)] + V[id(i+1,j-1)])/2 * (U[id(i,j-1)] - U[id(i,j)])/2 );
            d2u_dx2[id(i,j)] =
                ( U[id(i+1,j)] - 2*U[id(i,j)] + U[id(i-1,j)] ) / pow(delx, 2);
            d2u_dy2[id(i,j)] =
                ( U[id(i,j+1)] - 2*U[id(i,j)] + U[id(i,j-1)] ) / pow(dely, 2);
            dp_dx[id(i,j)] =
                ( P[id(i+1,j)] - P[id(i,j)] ) / delx;
        }
    }

    for (auto j=1; j != jmax; ++j){
        for (auto i=1; i != imax+1; ++i){
            duv_dx[id(i,j)] = 
                1/delx * ( (U[id(i,j)] + U[id(i,j+1)])/2 * (V[id(i,j)] + V[id(i+1,j)])/2
                - (U[id(i-1,j)] + U[id(i-1,j+1)])/2 * (V[id(i-1,j)] + V[id(i,j)])/2 )

                + gamma * 1/delx * ( abs(U[id(i,j)] + U[id(i,j+1)])/2 * (V[id(i,j)] - V[id(i+1,j)])/2  
                - abs(U[id(i-1,j)] + U[id(i-1,j+1)])/2 * (V[id(i-1,j)] - V[id(i,j)])/2 );
            dv2_dy[id(i,j)] = 
                1/dely * ( pow( (V[id(i,j)] + V[id(i,j+1)])/2, 2) - pow( (V[id(i,j-1)] + V[id(i,j)])/2, 2) )
                + gamma * 1/dely * ( abs(V[id(i,j)] + V[id(i,j+1)])/2 * (V[id(i,j)] - V[id(i,j+1)])/2  - 
                abs(V[id(i,j-1)] + V[id(i,j)])/2 * (V[id(i,j-1)] - V[id(i,j)])/2 );
            d2v_dx2[id(i,j)] =
                ( V[id(i+1,j)] - 2*V[id(i,j)] + V[id(i-1,j)] ) / pow(delx, 2);
            d2v_dy2[id(i,j)] =
                ( V[id(i,j+1)] - 2*V[id(i,j)] + V[id(i,j-1)] ) / pow(dely, 2);
            dp_dy[id(i,j)] =
                ( P[id(i,j+1)] - P[id(i,j)] ) / dely;
        }
    }
    //out << "Spatial derivatives computed." << endl;



}

void grid::COMP_FG(){
    COMP_SPATIAL_DERIVATIVES();
    // Formulas 3.36, 3.37. At boundary 3.42

    //  3.36
    for (auto j=1; j != jmax+1; ++j){
        for (auto i=1; i != imax; ++i){
            F[id(i,j)] = U[id(i,j)]
                + delt * ( 1/Re * (d2u_dx2[id(i,j)] + d2u_dy2[id(i,j)]) - du2_dx[id(i,j)] - duv_dy[id(i,j)] + GX);
        }
    }

    // 3.37
    for (auto j=1; j != jmax; ++j){
        for (auto i=1; i != imax+1; ++i){
            G[id(i,j)] = V[id(i,j)]
                + delt * ( 1/Re * (d2v_dx2[id(i,j)] + d2v_dy2[id(i,j)]) - duv_dx[id(i,j)] - dv2_dy[id(i,j)] + GY);
        }
    }

    // 3.41, 3.42
    for (auto j=1; j != jmax+1; ++j){
        // shouldnt be needed here because same assignment is already in POISSON
        //P[id(0,j)] = P[id(1,j)];
        //P[id(imax+1,j)] = P[id(imax,j)];
        F[id(0,j)] = U[id(0,j)];
        F[id(imax,j)] = U[id(imax,j)];
    }

    for (auto i=1; i != imax+1; ++i){
        //P[id(i,0)] = P[id(i,1)];
        //P[id(i,jmax+1)] = P[id(i,jmax)];
        G[id(i,0)] = V[id(i,0)];
        G[id(i,jmax)] = V[id(i,jmax)];
    }
}

void grid::COMP_FG2(){
    COMP_SPATIAL_DERIVATIVES();
    // Formulas 3.36, 3.37. At boundary 3.42

    // Sets F and G also inside the obstacles. Should be useless but no problem. boundary values get overwritten later
    //  3.36
    for (auto j=1; j != jmax+1; ++j){
        for (auto i=1; i != imax; ++i){
            F[id(i,j)] = U[id(i,j)]
                + delt * ( 1/Re * (d2u_dx2[id(i,j)] + d2u_dy2[id(i,j)]) - du2_dx[id(i,j)] - duv_dy[id(i,j)] + GX);
        }
    }

    // 3.37
    for (auto j=1; j != jmax; ++j){
        for (auto i=1; i != imax+1; ++i){
            G[id(i,j)] = V[id(i,j)]
                + delt * ( 1/Re * (d2v_dx2[id(i,j)] + d2v_dy2[id(i,j)]) - duv_dx[id(i,j)] - dv2_dy[id(i,j)] + GY);
        }
    }

    for (auto j=0; j != jmax+2; ++j){
        for (auto i=0; i != imax+2; ++i){
            // (FLAG[id(i,j)]-2) % 16 fluid cells to the 1 West, 3 East, 7 South, 5 Nord, 9 NW, 11 SW, 13 NE, 15 SE
            if (FLAG[id(i,j)] >= 2){
                switch ((FLAG[id(i,j)]-2) % 16)
                {
                case 1:
                    F[id(i-1,j)] = U[id(i-1,j)];
                    break;

                case 3:
                    F[id(i,j)] = U[id(i,j)];
                    break;
                
                case 5:
                    G[id(i,j)] = V[id(i,j)];
                    break;
                
                case 7:
                    G[id(i,j-1)] = V[id(i,j-1)];
                    break;

                case 9:
                    F[id(i-1,j)] = U[id(i-1,j)];
                    G[id(i,j)] = V[id(i,j)];
                    break;
                
                case 11:
                    F[id(i-1,j)] = U[id(i-1,j)];
                    G[id(i,j-1)] = V[id(i,j-1)];
                    break;
                
                case 13:
                    G[id(i,j)] = V[id(i,j)];
                    F[id(i,j)] = U[id(i,j)];
                    break;

                case 15:
                    G[id(i,j-1)] = V[id(i,j-1)];
                    F[id(i,j)] = U[id(i,j)];
                    break;

                default:
                    break;
                }
            }
        }
    }
}

void grid::COMP_RHS(){
    // of eq 3.38
    for (auto j=1; j != jmax+1; ++j){
        for (auto i=1; i != imax+1; ++i){
            RHS[id(i,j)] = 1/delt * ( (F[id(i,j)] - F[id(i-1,j)])/delx + (G[id(i,j)] - G[id(i,j-1)])/dely  );
        }
    }
}

// epsilon from (3.43).
bool grid::eps_W(unsigned i){
    if (i == 1) return 0;
    else return 1;
} 

bool grid::eps_E(unsigned i){
    if (i == imax) return 0;
    else return 1;
} 

bool grid::eps_S(unsigned j){
    if (j == 1) return 0;
    else return 1;
} 

bool grid::eps_N(unsigned j){
    if (j == jmax) return 0;
    else return 1;
} 

void grid::COMP_RES(){
    // (3.45) , (3.46)
    res = 0;

    for (auto j=1; j != jmax+1; ++j){
        for (auto i=1; i != imax+1; ++i){
            res += pow( (P[id(i+1,j)] - 2*P[id(i,j)] + P[id(i-1,j)] )/pow(delx, 2) 
            + (P[id(i,j+1)] - 2*P[id(i,j)] + P[id(i,j-1)] )/pow(dely, 2) - RHS[id(i,j)], 2);
        }
    }
    res = sqrt(res * 1/imax * 1/jmax);
}

void grid::COMP_RES2(){
    // (3.45) , (3.46)
    res = 0;

    for (auto j=1; j != jmax+1; ++j){
        for (auto i=1; i != imax+1; ++i){
            res += pow( (P[id(i+1,j)] - 2*P[id(i,j)] + P[id(i-1,j)] )/pow(delx, 2) 
            + (P[id(i,j+1)] - 2*P[id(i,j)] + P[id(i,j-1)] )/pow(dely, 2) - RHS[id(i,j)], 2);
        }
    }
    res = sqrt(res * 1/imax * 1/jmax);
}

void grid::COMP_RES_EPS(){
    // (3.45) , (3.46)
    res = 0;
    vector<double> res_vec;
    res_vec = vector<double>((imax + 2)*(jmax + 2),0); 

    // (3.45)
    for (auto j=1; j != jmax+1; ++j){
        for (auto i=1; i != imax+1; ++i){
            res_vec[id(i,j)] = ( eps_E(i) * ( P[id(i+1,j)] - P[id(i,j)] ) - eps_W(i) * ( P[id(i,j)] - P[id(i-1,j)] ) )/pow(delx, 2) 
            + ( eps_N(j) * ( P[id(i,j+1)] - P[id(i,j)] ) - eps_S(j) * ( P[id(i,j)] - P[id(i,j-1)] ) )/pow(dely, 2)  - RHS[id(i,j)];
        }
    }

    for (auto j=1; j != jmax+1; ++j){
        for (auto i=1; i != imax+1; ++i){
            res += pow(res_vec[id(i,j)], 2);
        }
    }

    res = sqrt( res/(imax * jmax) );
}

int grid::POISSON(){
    // res is the L2 norm of the residual, see (3.46) and (3.45)
    // epsilon E, N, W, S are set to 1 as this is identically fulfilled via (3.48)

    COMP_RES();
    cout << "Initial res = " << res << endl;

    it = 0;

    while (it < itermax && res > eps)
    {

        // (3.48)
        for (auto j=1; j != jmax+1; ++j){
            P[id(0,j)] = P[id(1,j)];
            P[id(imax+1,j)] = P[id(imax,j)];
        }
        for (auto i=1; i != imax+1; ++i){
            P[id(i,0)] = P[id(i,1)];
            P[id(i,jmax+1)] = P[id(i,jmax)];
        }

        //3.44, omega = omg from input file
        
        for (auto j=1; j != jmax+1; ++j){
            for (auto i=1; i != imax+1; ++i){
                P[id(i,j)] =  P[id(i,j)] * (1 - omg) + ( (P[id(i+1,j)] + P[id(i-1,j)])/pow(delx,2) + (P[id(i,j+1)] + P[id(i,j-1)])/pow(dely,2) - RHS[id(i,j)]) * omg/( 2/pow(delx,2) + 2/pow(dely,2) );
            }
        }

        COMP_RES();
        ++it;
    }
    cout << "Final res = " << res << " at iteration " << it <<  endl;

    return 0;
}

int grid::POISSON2(){
    // res is the L2 norm of the residual, see (3.46) and (3.45)
    // epsilon E, N, W, S are set to 1 as this is identically fulfilled via (3.48)

    COMP_RES2();
    cout << "Initial res = " << res << endl;

    it = 0;

    while (it < itermax && res > eps)
    {

        // (below 3.53)
        for (auto j=0; j != jmax+2; ++j){
            for (auto i=0; i != imax+2; ++i){
                // (FLAG[id(i,j)]-2) % 16 fluid cells to the 1 West, 3 East, 5 Nord, 7 South, 9 NW, 11 SW, 13 NE, 15 SE
                if (FLAG[id(i,j)] >= 2){
                    switch ((FLAG[id(i,j)] - 2) % 16)
                    {
                    case 1:
                        P[id(i,j)] = P[id(i-1,j)];
                        break;

                    case 3:
                        P[id(i,j)] = P[id(i+1,j)];
                        break;
                    
                    case 5:
                        P[id(i,j)] = P[id(i,j+1)];
                        break;
                    
                    case 7:
                        P[id(i,j)] = P[id(i,j-1)];
                        break;

                    case 9:
                        P[id(i,j)] = ( P[id(i,j+1)] + P[id(i-1,j)] )/2;
                        break;
                    
                    case 11:
                        P[id(i,j)] = ( P[id(i,j-1)] + P[id(i-1,j)] )/2;
                        break;
                    
                    case 13:
                        P[id(i,j)] = ( P[id(i,j+1)] + P[id(i+1,j)] )/2;
                        break;
                    case 15:
                        P[id(i,j)] = ( P[id(i,j-1)] + P[id(i+1,j)] )/2;
                        break;

                    default:
                        break;
                    }
                }
            }
        }
        

        //3.44, omega = omg from input file
        
        for (auto j=1; j != jmax+1; ++j){
            for (auto i=1; i != imax+1; ++i){
                P[id(i,j)] =  P[id(i,j)] * (1 - omg) + ( (P[id(i+1,j)] + P[id(i-1,j)])/pow(delx,2) + (P[id(i,j+1)] + P[id(i,j-1)])/pow(dely,2) - RHS[id(i,j)]) * omg/( 2/pow(delx,2) + 2/pow(dely,2) );
            }
        }

        COMP_RES2();
        ++it;
    }
    cout << "Final res = " << res << " at iteration " << it <<  endl;

    return 0;
}


int grid::POISSON_EPS(){
    // res is the L2 norm of the residual, see (3.46) and (3.45)
    // epsilon E, N, W, S are set to 1 as this is identically fulfilled via (3.48)
    vector<double> res_list; // for file output

    COMP_RES_EPS();
    res_list.push_back(res);
    cout << "Initial res = " << res << endl;

    it = 0;

    while (it < itermax && res > eps)
    {
        //3.44, omega = omg from input file
        
        for (auto j=1; j != jmax+1; ++j){
            for (auto i=1; i != imax+1; ++i){
                P[id(i,j)] =  P[id(i,j)] * (1 - omg) + ( eps_E(i)*(P[id(i+1,j)] + eps_W(i)*P[id(i-1,j)])/pow(delx,2) + (eps_N(j)*P[id(i,j+1)] + eps_S(j)*P[id(i,j-1)])/pow(dely,2) - RHS[id(i,j)]) * omg/( ( eps_E(i) + eps_W(i) )/pow(delx,2) + ( eps_S(j) + eps_N(j) )/pow(dely,2) );
            }
        }

        COMP_RES_EPS();
        res_list.push_back(res);
        ++it;
    }
    cout << "Final res = " << res << " at iteration " << it <<  endl;

    ADD_TO_FILE("res_EPS.tsv", res_list);
    cout << "Res written to file res_EPS.tsv" << endl;
    

    return 0;
}




void grid::ADAP_UV(){
    // (3.34)
    for (auto j=1; j != jmax+1; ++j){
        for (auto i=1; i != imax; ++i){
            U[id(i,j)] = F[id(i,j)] - delt/delx * ( P[id(i+1,j)] - P[id(i,j)] );
        }
    }

    // (3.35)
    for (auto j=1; j != jmax; ++j){
        for (auto i=1; i != imax+1; ++i){
            V[id(i,j)] = G[id(i,j)] - delt/dely * ( P[id(i,j+1)] - P[id(i,j)] );
        }
    }
}