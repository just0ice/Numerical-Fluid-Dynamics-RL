#include "grid.h"

void grid::ALG_BASE(string inputfile = "settings.in"){
    READ_PARAMETER(inputfile);
    INIT_UVP();

    // Algorihm 1. Base version, p. 40
    double t = 0;
    unsigned n = 1;
    CLEAR_OUTPUT_FILES();

    while (t < t_end){
        COMP_DELT();
        SETBCOND();
        COMP_FG();
        COMP_RHS();
        // SOR Cycle
        POISSON();
        ADAP_UV();
        t += delt;
        n += 1;
    }

    OUTPUTVEC();


}

void grid::ALG_ALL(string inputfile = "settings.in"){
    READ_PARAMETER(inputfile);
    INIT_UVP();

    INIT_TEST_DATA();
    COMP_DELT();
    SETBCOND();
    CHECKBCOND();
    COMP_SPATIAL_DERIVATIVES();
    COMP_FG();
    COMP_RHS();
    POISSON();
    PRINT_UVP();
    CLEAR_OUTPUT_FILES();
    OUTPUTVEC();
}

void grid::ALG_TEST_POISSON(string inputfile = "settings.in"){
    READ_PARAMETER(inputfile);
    INIT_UVP();

    // Algorihm 1. Base version, p. 40
    double t = 0;
    unsigned n = 1;
    CLEAR_OUTPUT_FILES();

    SETBCOND();
    // set RHS to 0 (for which the solution is known)
    for (auto j=1; j != jmax+1; ++j){
        for (auto i=1; i != imax+1; ++i){
            RHS[id(i,j)] = 0;
        }
    }
    // SOR Cycle
    POISSON();

    OUTPUTVEC();

}