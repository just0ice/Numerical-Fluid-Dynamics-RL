#include "grid.h"


void grid::ALG_BASE(){
    READ_PARAMETER("Cavity.in");
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
    READ_PARAMETER("settings/Cavity.in");
    INIT_UVP();

    // Algorihm 1. Base version, p. 40
    double t = 0;
    unsigned n = 1;

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

        if (t >= 1 && (t - delt) <= 1){
            CLEAR_OUTPUT_FILES("Cavity/1/");
            OUTPUTVEC("Cavity/1/");
        }
        if (t >= 2 && (t - delt) <= 2){
            CLEAR_OUTPUT_FILES("Cavity/2/");
            OUTPUTVEC("Cavity/2/");
        }
        if (t >= 5 && (t - delt) <= 5){
            CLEAR_OUTPUT_FILES("Cavity/5/");
            OUTPUTVEC("Cavity/5/");
        }
        if (t >= 10 && (t - delt) <= 10){
            CLEAR_OUTPUT_FILES("Cavity/10/");
            OUTPUTVEC("Cavity/10/");
        }
    }

     // testing where the residual is high
    CLEAR_OUTPUT_FILES();
    CLEAR_OUTPUT_FILES("Cavity/");
    OUTPUTVEC();
    OUTPUTVEC("Cavity/");
}


void grid::ALG_STEP(double Re_man){
    cout << "ALG_STEP" << endl;
    READ_PARAMETER("settings/Step.in");
    string folder = "Step/";
    if (Re_man != 0){
        Re = Re_man;
        cout << "Manual Reynolds Number Re = " << Re << endl;
        string folder = "Step/"+to_string(Re)+"/";
    }

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
    CLEAR_OUTPUT_FILES(folder);

    // general geometries
    RECTANGLE(2,0,0.75,0.75,7.5);
    DOMAIN_BOUNDARY();
    FLAG_PP();

    while (t < t_end){
        cout << "t = " << t << endl;
        COMP_DELT();
        if (delt < 0.02){
            delt = 0.02;
        }
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
    OUTPUTVEC(folder);
}


void grid::ALG_EVANGELION(double V_in){
    READ_PARAMETER("settings/Evangelion.in");
    cout << "V_in = " << V_in << endl;
    string folder = "Evangelion/"+to_string(V_in)+"/";
    VI = V_in; // keep the initial residual low
    //imax  = 30;
    //jmax = 40;
    INIT_UVP();

    // Algorihm 1. Base version, p. 40
    double t = 0;
    unsigned n = 1;
    CLEAR_OUTPUT_FILES(folder);
    CLEAR_OUTPUT_FILES();

    // general geometries
    DOMAIN_BOUNDARY();
    RECTANGLE(2,0,4,1,1);
    RECTANGLE(2,2,4,1,1);
    RECTANGLE(2,0,2,4,1);
    RECTANGLE(2,2,2,4,1);
    FLAG_PP();

    while (t < t_end){
        cout << "t = " << t << endl;
        COMP_DELT2();
        SETBCOND2();
        // modify boundary conditions to set upper bound moving. Might move this to "boundary.cc" or "problem.cc" triggered by switch "problem" later
        for (auto i = 1; i != imax + 1; ++i){
            //V[id(i,0)] = V_in;
            if (x(i) >= xlength/3 && x(i) <= xlength*2/3)
                V[id(i,0)] = V_in * ( 1 - pow(x(i)-xlength/2,2)/pow(xlength/6,2));
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
    OUTPUTVEC(folder);
}

void grid::ALG_DISC(double Re_man){
    READ_PARAMETER("settings/Disc.in");
    //imax  = 440;
    //jmax = 80;
    if (Re_man != 0){
        Re = Re_man;
        cout << "Manual Reynolds Number Re = " << Re << endl;
    }
    string folder = "Disc/"+to_string(Re)+"/";
    INIT_UVP();

    // Algorihm 1. Base version, p. 40
    double t = 0;
    unsigned n = 1;

    // general geometries
    DOMAIN_BOUNDARY();
    CIRCLE(2,2.0,2.0,0.5);
    FLAG_PP();

    while (t < t_end){
        cout << "t = " << t << endl;
        COMP_DELT2();
        SETBCOND2();
        // modify boundary conditions to set upper bound moving. Might move this to "boundary.cc" or "problem.cc" triggered by switch "problem" later
        for (auto j = 1; j != jmax + 1; ++j){
            //U[id(0,j)] = 1.5 * ( 1 - pow(j-jmax/2,2)/pow(jmax/2,2));
           // U[id(0,j)] = 1.0;
            U[id(0,j)] = 1.5 * ( 1 - pow(y(j)-ylength/2,2)/pow(ylength/2,2));
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
    CLEAR_OUTPUT_FILES(folder);
    CLEAR_OUTPUT_FILES();
    OUTPUTVEC();
    OUTPUTVEC(folder);
}

void grid::ALG_WORKING(){
    cout << "ALG_STEP" << endl;
    READ_PARAMETER("Cavity.in");
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

    OUTPUTVEC();
}

void grid::ALG_WORKING2(){
    READ_PARAMETER("Cavity.in");
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

    // testing where the residual is high
    vector<double> temp;
    double tempmax = 0;
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
