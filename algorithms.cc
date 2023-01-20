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
        COMP_DELT();
        SETBCOND2();
        // modify boundary conditions to set upper bound moving. Might move this to "boundary.cc" or "problem.cc" triggered by switch "problem" later
        for (auto i = 1; i != imax + 1; ++i){
            U[id(i,jmax+1)] = 2.0 - U[id(i,jmax)];
        }
        COMP_FG2();
        COMP_RHS();
        // SOR Cycle
        POISSON2();
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
    
    DOMAIN_BOUNDARY();
    //RECTANGLE(2,0.25,0.75,0.5,0.5);
    //CIRCLE(1,0.75,0.75,0.25);
    FLAG_PP();
    cout << "FLAG PP" << endl;
    PRINT_FLAG();

    SETBCOND2();
    CHECKBCOND();
    COMP_FG();

    vector<double> Fold = F;
    vector<double> Gold = G;
    vector<double> Fdiff;
    Fdiff = vector<double>((imax + 2)*(jmax + 2),0); 
    vector<double> Gdiff;
    Gdiff = vector<double>((imax + 2)*(jmax + 2),0); 

    INIT_TEST_DATA();
    SETBCOND2();
    CHECKBCOND();
    cout << "TEST 1" << endl;
    COMP_FG2();
    cout << "TEST 2" << endl;

    for (auto j = 0; j != jmax+2; ++j){
        for (auto i = 0; i != imax+2; ++i){
            Fdiff[id(i,j)] = F[id(i,j)] - Fold[id(i,j)];
            Gdiff[id(i,j)] = G[id(i,j)] - Gold[id(i,j)];
        }
    }

    cout << "F diff" << endl;
    PRINT_TO_TERMINAL(Fdiff,imax+1,jmax+1);
    cout << "G diff" << endl;
    PRINT_TO_TERMINAL(Gdiff,imax+1,jmax+1);
    

    COMP_RHS();
    POISSON2();
    cout << "P" << endl;
    PRINT_TO_TERMINAL(P,imax+1,jmax+1);
    

    CLEAR_OUTPUT_FILES();
    OUTPUTVEC();
}