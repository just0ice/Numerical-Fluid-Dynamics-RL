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
    // taking a 1D vector. Pseudo 2D conversion is i = x + width*y, x = i % width; y = i / width.
    // 1 block width boundary. Whole array dimension [0,imax+1]x[0,jmax+1]. Without boundary [1,imax]x[1,jmax]

    U = vector<double>((imax + 2)*(jmax + 2),UI); 
    V = vector<double>((imax + 2)*(jmax + 2),VI);
    P = vector<double>((imax + 2)*(jmax + 2),PI);
}