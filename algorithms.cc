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

void grid::ALG_BASE2(){
    READ_PARAMETER("Lid-Driven Cavity.in");
    INIT_UVP();

    // Algorihm 1. Base version, p. 40
    double t = 0;
    unsigned n = 1;
    CLEAR_OUTPUT_FILES();

    // general geometries
    DOMAIN_BOUNDARY();
    FLAG_PP();

    while (t < t_end){
        cout << "t = " << t << endl;
        COMP_DELT2();
        SETBCOND2();
        // modify boundary conditions to set upper bound moving. Might move this to "boundary.cc" or "problem.cc" triggered by switch "problem" later
        for (auto i = 1; i != imax + 1; ++i){
            U[id(i,jmax+1)] = 2.0 - U[id(i,jmax)];
        }
        COMP_FG2();
        COMP_RHS();
        // SOR Cycle
        POISSON2();
        ADAP_UV2();
        t += delt;
        n += 1;
    }

    OUTPUTVEC();
}


void grid::ALG_STEP(){
    READ_PARAMETER("Step.in");
    INIT_UVP();
    // upper half only
    /*
    cout << jmax/2 << endl;
    for (unsigned j = jmax/2; j != jmax + 1; ++j){
        for (auto i = 1; i != imax + 1; ++i){
            U[id(i,j)] = 0;
        }
    }
    */

    // Algorihm 1. Base version, p. 40
    double t = 0;
    unsigned n = 1;
    CLEAR_OUTPUT_FILES();

    // general geometries
    //RECTANGLE(2,0,0.75,0.75,7.5);
    DOMAIN_BOUNDARY();
    FLAG_PP();

    while (t < t_end){
        cout << "t = " << t << endl;
        COMP_DELT();
        SETBCOND2();
        // modify boundary conditions to set upper bound moving. Might move this to "boundary.cc" or "problem.cc" triggered by switch "problem" later
        for (auto j = 1; j != jmax + 1; ++j){
            U[id(0,j)] = 1.0;
        }
        COMP_FG2();
        COMP_RHS2();
        // SOR Cycle
        POISSON2();
        ADAP_UV2();
        t += delt;
        n += 1;
    }

    OUTPUTVEC();
}

void grid::ALG_WORKING(){
    READ_PARAMETER("Step.in");
    INIT_UVP();

    // Algorihm 1. Base version, p. 40
    double t = 0;
    unsigned n = 1;
    CLEAR_OUTPUT_FILES();

    // general geometries
    DOMAIN_BOUNDARY();
    FLAG_PP();

    while (t < t_end){
        cout << "t = " << t << endl;
        COMP_DELT2();
        SETBCOND2();
        // modify boundary conditions to set upper bound moving. Might move this to "boundary.cc" or "problem.cc" triggered by switch "problem" later
        for (auto i = 1; i != imax + 1; ++i){
            U[id(i,jmax+1)] = 2.0 - U[id(i,jmax)];
        }
        COMP_FG2();
        COMP_RHS();
        // SOR Cycle
        POISSON2();
        ADAP_UV2();
        t += delt;
        n += 1;
    }

    OUTPUTVEC();
}