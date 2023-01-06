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
        P[id(0,j)] = P[id(1,j)];
        P[id(imax+1,j)] = P[id(imax,j)];
        F[id(0,j)] = U[id(0,j)];
        F[id(imax,j)] = U[id(imax,j)];
    }

    for (auto i=1; i != jmax+1; ++i){
        P[id(i,0)] = P[id(i,1)];
        P[id(i,jmax+1)] = P[id(i,jmax)];
        G[id(i,0)] = V[id(i,0)];
        G[id(i,jmax)] = U[id(i,jmax)];
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

int grid::POISSON(){
    // res is the L2 norm of the residual, see (3.46) and (3.45)
    // epsilon E, N, W, S are set to 1 as this is identically fulfilled via (3.48)

    // Test data
    for (auto j=0; j != jmax+2; ++j){
        for (auto i=0; i != imax+2; ++i){
            P[id(i,j)] = 0;
        }
    }

    P[id(2,2)] = 1;
    //omg = 1; // gauss seidel
    //
    
    COMP_RES();
    //cout << "Initial res = " << res << endl;

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
    //cout << "Final res = " << res << " at iteration " << it <<  endl;

    

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
            V[id(i,j)] = G[id(i,j)] - delt/delx * ( P[id(i,j+1)] - P[id(i,j)] );
        }
    }
}