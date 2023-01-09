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

void grid::ALG_ALL(){
    READ_PARAMETER("settings.in");
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

void grid::ALG_TEST_POISSON(){
    READ_PARAMETER("settings.in");
    INIT_UVP();

    // Algorihm 1. Base version, p. 40
    double t = 0;
    unsigned n = 1;
    CLEAR_OUTPUT_FILES();
    for (auto i=0; i != imax+2; ++i){
        for (auto j=0; j != jmax+2; ++j)
            P[id(i,j)] = pow((i-imax-1)*i, 2) + pow((j-jmax-1)*j, 2);
    }
    //INIT_TEST_DATA();

    //SETBCOND();
    // set RHS to 0 (for which the solution is known)
    for (auto j=1; j != jmax+1; ++j){
        for (auto i=1; i != imax+1; ++i){
            RHS[id(i,j)] = 0;
        }
    }
    // SOR Cycle
    dev = true;
    POISSON();

    OUTPUTVEC();

}

void grid::ALG_TEST_POISSON_2(){
    READ_PARAMETER("Lid-Driven Cavity.in");
    INIT_UVP();

    // Algorihm 1. Base version, p. 40
    CLEAR_OUTPUT_FILES();

    COMP_DELT();
    SETBCOND();
    // modify boundary conditions to set upper bound moving. Might move this to "boundary.cc" or "problem.cc" triggered by switch "problem" later
    for (auto i = 1; i != imax + 1; ++i){
        U[id(i,jmax+1)] = 2.0 - U[id(i,jmax)];
    }
    COMP_FG();
    COMP_RHS();
    // SOR Cycle
    dev = true;

    POISSON();

    ADAP_UV();

    OUTPUTVEC();
}