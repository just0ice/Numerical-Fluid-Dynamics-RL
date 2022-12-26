// READ_PARAMETER, RMATRIX, FREE_RMATRIX, INIT_UVP
#include "grid.h"

int grid::READ_PARAMETER(string inputfile){
    // hardcode for now. make input file work later.
    UI = 0;
    VI = 0;
    PI = 0;

    // grid dimensions
    imax = 10;
    jmax = 10;
    n_ghost = 2;

    return 0;
}

void grid::INIT_UVP(){

    U = vector<double>((imax + 2*n_ghost)*(jmax + 2*n_ghost),UI); // taking a 1D vector. Pseudo 2D conversion is i = x + width*y, x = i % width; y = i / width. Ghost zones at corresponding i,j.
    V = vector<double>((imax + 2*n_ghost)*(jmax + 2*n_ghost),VI);
    P = vector<double>((imax + 2*n_ghost)*(jmax + 2*n_ghost),PI);
}