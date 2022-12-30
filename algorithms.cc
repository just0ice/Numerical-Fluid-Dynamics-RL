#include "grid.h"

void grid::BASE(){
    // Algorihm 1. Base version, p. 40
    double t = 0;
    unsigned n = 1;
    unsigned it = 0;

    while (t < t_end){
        COMP_DELT();
        SETBCOND();
        COMP_FG();
        COMP_RHS();
        it = 0;
        // SOR Cycle
        t += delt;
        n += 1;
    }

    OUTPUTVEC();


}