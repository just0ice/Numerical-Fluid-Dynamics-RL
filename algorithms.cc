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
    READ_PARAMETER("Step.in");
    xlength = 30;
    ylength = 30;
    imax = 30;
    jmax = 30;
    t_end = 0.003;
    INIT_UVP();
    // upper half only
    cout << jmax/2 << endl;
    
    /*
    for (unsigned j = 0; j != jmax/2 + 1; ++j){
        for (auto i = 0; i != imax + 2; ++i){
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
    RECTANGLE(2,0,15,15,15);
    DOMAIN_BOUNDARY();
    FLAG_PP();

    while (t < t_end){
        cout << "t = " << t << endl;
        COMP_DELT2();
        SETBCOND2();
        //CHECKBCOND();
        // modify boundary conditions to set upper bound moving. Might move this to "boundary.cc" or "problem.cc" triggered by switch "problem" later
        
        for (auto j = 1; j != jmax+1; ++j){
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
    cout << "Temp" << endl;
    PRINT_TO_TERMINAL(temp,imax+1,jmax+1);
    PRINT_FLAG();
    vector<double> temp_out;
    for (auto j=1; j != jmax + 1; ++j){
        for (auto i=1; i != imax + 1; ++i){
            temp_out.push_back(temp[id(i,j)]);
        }
    }
    ADD_TO_FILE("temp.tsv", temp_out);

    OUTPUTVEC();
}

/*
 vector<double> temp;
        double tempmax = 0;
        
        // testing where the residual is high
        temp = vector<double>((imax + 2)*(jmax + 2),0); 

        for (auto j=1; j != jmax+1; ++j){
            for (auto i=1; i != imax+1; ++i){
                    temp[id(i,j)] = pow( (P[id(i+1,j)] - 2*P[id(i,j)] + P[id(i-1,j)] )/pow(delx, 2) 
                    + (P[id(i,j+1)] - 2*P[id(i,j)] + P[id(i,j-1)] )/pow(dely, 2) - RHS[id(i,j)], 2);
            }
        }
        cout << "Temp" << endl;
        PRINT_TO_TERMINAL(temp,imax+1,jmax+1);
*/