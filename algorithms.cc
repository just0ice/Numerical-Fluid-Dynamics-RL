#include "grid.h"

void grid::ALG_BASE(){
    READ_PARAMETER("Lid-Driven Cavity.in");
    INIT_UVP();

    // Algorihm 1. Base version, p. 40
    double t = 0;
    unsigned n = 1;
    CLEAR_OUTPUT_FILES();

    while (t < t_end){
        cout << "t = " << t << endl;
        COMP_DELT();
        SETBCOND();
        // modify boundary conditions to set upper bound moving. Might move this to "boundary.cc" or "problem.cc" triggered by switch "problem" later
        for (auto i = 1; i != imax + 1; ++i){
            U[id(i,jmax+1)] = 2.0 - U[id(i,jmax)];
        }
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

void grid::ALG_WORKING(){
    READ_PARAMETER("settings.in");
    INIT_UVP();
    INIT_TEST_DATA();
    COMP_DELT();
    SETBCOND();
    CHECKBCOND();
    
    DOMAIN_BOUNDARY();
    RECTANGLE(2,0.,0.5,0.5,0.5);
    CIRCLE(1,0.75,0.75,0.25);
    cout << "FLAG" << endl;
    PRINT_FLAG();
    FLAG_PP();
    cout << "FLAG PP" << endl;
    PRINT_FLAG();
    CLEAR_OUTPUT_FILES();
    OUTPUTVEC();
}