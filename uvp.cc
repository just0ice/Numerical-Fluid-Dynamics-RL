// COMP_FG, CMP_RHS, POISSON, and ADAP_UV
#include "grid.h"

void grid::COMP_SPATIAL_DERIVATIVES(){
    // Computes the spatial derivatives according to 3.19
    
    // Test calculations
    U[id(2,2)] = 1;
    V[id(2,2)] = 2;
    P[id(2,2)] = 3;

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
    cout << "Spatial derivatives computed." << endl;



}

void grid::COMP_FG(){
    COMP_SPATIAL_DERIVATIVES();
    // Formulas 3.36, 3.37. At boundary 3.42

}