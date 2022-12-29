// COMP_FG, CMP_RHS, POISSON, and ADAP_UV
#include "grid.h"

void grid::COMP_SPATIAL_DERIVATIVES(){
    // Computes the spatial derivatives according to 3.19
    U[id(1,2)] = 1;
    // 3.19a
    for (auto j=1; j != jmax+1; ++j){
        for (auto i=1; i != imax; ++i){
            du2_dx[id(i,j)] = 
                    1/delx * ( pow( (U[id(i,j)] + U[id(i+1,j)])/2, 2) - pow( (U[id(i-1,j)] + U[id(i,j)])/2, 2) )
                    + gamma * 1/delx * ( abs(U[id(i,j)] + U[id(i+1,j)])/2 * (U[id(i,j)] - U[id(i+1,j)])/2  - 
                    abs(U[id(i-1,j)] + U[id(i,j)])/2 * (U[id(i-1,j)] - U[id(i,j)])/2 );
        }
    }



}

void grid::COMP_FG(){
    COMP_SPATIAL_DERIVATIVES();
    // Formulas 3.36, 3.37. At boundary 3.42

}