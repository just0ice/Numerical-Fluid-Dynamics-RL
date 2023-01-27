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

     // testing where the residual is high

    OUTPUTVEC();
}


void grid::ALG_STEP(){
    cout << "ALG_STEP" << endl;
    READ_PARAMETER("Step.in");
    INIT_UVP();
    // upper half only
    cout << jmax/2 << endl;
    for (unsigned j = 0; j != jmax/2 + 1; ++j){
        for (auto i = 0; i != imax + 2; ++i){
            U[id(i,j)] = 0;
        }
    }

    // Algorihm 1. Base version, p. 40
    double t = 0;
    unsigned n = 1;
    CLEAR_OUTPUT_FILES();

    // general geometries
    RECTANGLE(2,0,0.75,0.75,7.5);
    DOMAIN_BOUNDARY();
    FLAG_PP();

    while (t < t_end){
        cout << "t = " << t << endl;
        COMP_DELT();
        SETBCOND2();
        //CHECKBCOND();
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
    cout << "ALG_STEP" << endl;
    READ_PARAMETER("Lid-Driven Cavity.in");
    wE = 2;
    wW = 2;
    wN = 2;
    wS = 2;
    imax = 5;
    jmax = 5;
    t_end = 0.00000001;
    INIT_UVP();
    INIT_TEST_DATA();
    //RECTANGLE(2,0,0.5,0.5,0.5);
    DOMAIN_BOUNDARY();
    FLAG_PP();
    PRINT_FLAG();
    cout << "N_fluid"<< N_fluid << endl; 

    // Algorihm 1. Base version, p. 40
    double t = 0;
    unsigned n = 1;
    CLEAR_OUTPUT_FILES();

    while (t < t_end){
        cout << "t = " << t << endl;
        COMP_DELT();
        SETBCOND();
        CHECKBCOND();
        // modify boundary conditions to set upper bound moving. Might move this to "boundary.cc" or "problem.cc" triggered by switch "problem" later
        /*
        for (auto j = 1; j != jmax + 1; ++j){
            U[id(0,j)] = 1.0;
        }
        */
       delx = 1;
       dely = 1;
       gamma = 0;
        COMP_FG();
        COMP_RHS();
        // SOR Cycle
        //POISSON_EPS2();
        cout << "RHS" << endl;
        PRINT_TO_TERMINAL(RHS,imax+1,jmax+1);
        PRINT_UVP();
        cout << "du2_dx" << endl;
        PRINT_TO_TERMINAL(du2_dx,imax+1,jmax+1);
        cout << "duv_dy" << endl;
        PRINT_TO_TERMINAL(duv_dy,imax+1,jmax+1);
        cout << "d2u_dy2" << endl;
        PRINT_TO_TERMINAL(d2u_dy2,imax+1,jmax+1);
        cout << "dp_dx" << endl;
        PRINT_TO_TERMINAL(dp_dx,imax+1,jmax+1);
        cout << "duv_dx" << endl;
        PRINT_TO_TERMINAL(duv_dx,imax+1,jmax+1);
        cout << "dv2_dy" << endl;
        PRINT_TO_TERMINAL(dv2_dy,imax+1,jmax+1);
        cout << "d2v_dx2" << endl;
        PRINT_TO_TERMINAL(d2v_dx2,imax+1,jmax+1);
        cout << "d2v_dy2" << endl;
        PRINT_TO_TERMINAL(d2v_dy2,imax+1,jmax+1);
        cout << "dp_dy" << endl;
        PRINT_TO_TERMINAL(dp_dy,imax+1,jmax+1);
        //COMPARE_POISSON();
        //COMPARE_POISSON();
        ADAP_UV();
        t += delt;
        n += 1;
    }

    vector<double> temp;
    double tempmax = 0;
    
    // testing where the residual is high
    temp = vector<double>((imax + 2)*(jmax + 2),0); 

    for (auto j=1; j != jmax+1; ++j){
        for (auto i=1; i != imax+1; ++i){
            if (FLAG[id(i,j)] % 2 == 0){
                temp[id(i,j)] = pow( (P[id(i+1,j)] - 2*P[id(i,j)] + P[id(i-1,j)] )/pow(delx, 2) 
                + (P[id(i,j+1)] - 2*P[id(i,j)] + P[id(i,j-1)] )/pow(dely, 2) - RHS[id(i,j)], 2);
            }
        }
    }
    //cout << "Temp" << endl;
    //PRINT_TO_TERMINAL(temp,imax+1,jmax+1);
    vector<double> temp_out;
    for (auto j=1; j != jmax + 1; ++j){
        for (auto i=1; i != imax + 1; ++i){
            temp_out.push_back(temp[id(i,j)]);
        }
    }
    ADD_TO_FILE("temp.tsv", temp_out);

    OUTPUTVEC();
}

void grid::ALG_WORKING2(){
    READ_PARAMETER("Lid-Driven Cavity.in");
    imax = 30;
    jmax = 30;
    t_end = 0.3;
    wE = 3;
    wW = 3;
    wN = 1;
    wS = 1;
    INIT_UVP();

    // Algorihm 1. Base version, p. 40
    double t = 0;
    unsigned n = 1;
    CLEAR_OUTPUT_FILES();

    // general geometries

    DOMAIN_BOUNDARY();
    //RECTANGLE(2,0,0.5,0.5,0.5);

    FLAG_PP();
    PRINT_FLAG();

    while (t < t_end){
        cout << "t = " << t << endl;
        COMP_DELT();
        // modify boundary conditions to set upper bound moving. Might move this to "boundary.cc" or "problem.cc" triggered by switch "problem" later
        /*
        for (auto i = 1; i != imax + 1; ++i){
            U[id(i,jmax+1)] = 2.0 - U[id(i,jmax)];
        }
        */
        SETBCOND();
        for (auto j = 1; j != jmax + 1; ++j){
            U[id(0,j)] = 1.0;
            //V[id(1,j)] = 0;
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
