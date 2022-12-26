// READ_PARAMETER, RMATRIX, FREE_RMATRIX, INIT_UVP
#include "grid.h"

int grid::READ_PARAMETER(string inputfile){
    // hardcode for now. make input file work later.
    UI = 0;
    VI = 0;
    PI = 0;

    // grid dimensions
    imax = 2;
    jmax = 2;
    xlength = 10;
    ylength = 5;

    return 0;
}

void grid::INIT_UVP(){

    U = vector<double>((imax + 2)*(jmax + 2),UI); // taking a 1D vector. Pseudo 2D conversion is i = x + width*y, x = i % width; y = i / width. Ghost zones at corresponding i,j.
    V = vector<double>((imax + 2)*(jmax + 2),VI);
    P = vector<double>((imax + 2)*(jmax + 2),PI);
}