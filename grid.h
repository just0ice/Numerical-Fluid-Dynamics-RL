#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <fstream>
#include <math.h> // for absolute value in COMP_DELT
#include <algorithm> // min, max of a vector
#include <cmath> // square root

using namespace::std;

class grid
{
private:
    // make most of the stuff private later 
public:
    grid(); // constructer
    ~grid(); // deconstructer
    // 3.3.1 Problem Parameters and Data Structures
    // Geometry data
    double xlength, ylength;
    unsigned imax, jmax;    // book says int but this should hopefully be ok. reevaluate if imax / jmax are used for arithmetics
                            // Caution! Matric is A_ji. j counts rows, i counts columns (annoying but coherent with the book)
    double delx, dely;

    // Time-stepping data
    double t;
    double t_end;
    double delt;
    double tau;

    // Pressure-iteration data
    int itermax;
    int it;
    double res;
    double eps;
    double omg;
    double gamma;

    // Problem-dependent quantities
    double Re;
    double GX, GY;
    double UI, VI, PI;
    unsigned wW, wE, wN, wS;
    char problem;

    // Auxiliary variables
    double Umax; // calculated in COMP_DELT. Also needed for CHECK_HIRT
    double Vmax;
    bool dev = false; // variable to enable extended output. dev variables should have "temp" in their name. makeshift solution. 

    // Data arrays
    vector<double> U, V, P; // not sure wether its better to use array here for performance. might be a problem because of fixed length so need to be carefull when to initialize. lets keep vector for now
    vector<double> Ucc, Vcc, Pcc; // cell centered averaged and taken only physical cells (not on boundary). P was already cell centered
    vector<double> RHS;
    vector<double> F,G;

    // Spatial derivatives to compute F, G
    // If code to slow, try to allocate and deallocate temporary space for these quantities
    vector<double> du2_dx;
    vector<double> duv_dy;
    vector<double> d2u_dx2;
    vector<double> d2u_dy2;
    vector<double> dp_dx;

    vector<double> duv_dx;
    vector<double> dv2_dy;
    vector<double> d2v_dx2;
    vector<double> d2v_dy2;
    vector<double> dp_dy;

    // obstacle FLAG array
    vector<unsigned int> FLAG;

    // 3.34 The Program
    // declar member functions according to p.43
    // init.cc
    int READ_PARAMETER(string inputfile);
    void INIT_UVP(); // and spatial derivatives
    void COMP_DELT();
    // boundary.cc
    void SETBCOND();
    void CHECKBCOND();
    void SETSPECBCOND();
    //uvp.cc
    void COMP_FG();
    void COMP_RHS();
    int POISSON(); // dev = true means extended output
    void ADAP_UV();

    // additional member functions. can be found in misc.cc
    void PRINT_UVP(); // print U as a matrix into the Terminal
    void PRINT_TO_TERMINAL(vector<double> X, unsigned iend, unsigned jend);
    void ADD_TO_FILE(string fname, vector<double> X); // writes vector X to file fname
    void OUTPUTVEC(string folder = ""); // see 4.1.4 on page 54 
    void CC_AVERAGE_UV(); // computes the cc average of U and V and stores the result in Ucc and Vcc
    void CLEAR_OUTPUT_FILES(string folder = "");
    void INIT_TEST_DATA();
    unsigned id(unsigned i, unsigned j);

    // more additional member functions
    void COMP_SPATIAL_DERIVATIVES(); // in uvp.cc . according to 3.19
    void CHECK_HIRT(); // in uvp.cc . according to 3.20
    void COMP_RES(); // in uvp.cc according to (3.45) and (3.46)
    bool eps_W(unsigned i); // in uvp.cc according to (3.43), below 
    bool eps_E(unsigned i); 
    bool eps_S(unsigned j);
    bool eps_N(unsigned j);
    void COMP_RES_EPS(); // same as COMP_RES() but more readable and with epsilons
    int POISSON_EPS(); // same as POISSON() but with epsilons

    // Algorithms
    void ALG_BASE();
    void ALG_WORKING();
    void ALG_EVANGELION(double V_in = 1);
    void ALG_DISC(double Re_man);
    void ALG_STEP(double Re_man = 0);

    // Obstacles
    unsigned int N_fluid;
    void DOMAIN_BOUNDARY();
    void RECTANGLE(unsigned bcond, double x_lo, double y_lo, double hight, double length);
    void CIRCLE(unsigned bcond, double x_center, double y_center, double radius);
    void PRINT_FLAG();
    void SETBCOND2();
    void COMP_FG2();
    void COMP_RES2();
    void COMP_RHS2();
    int POISSON2(); 
    int POISSON_EPS2(); 
    int COMPARE_POISSON(); 
    void COMP_SPATIAL_DERIVATIVES2();
    void ALG_BASE2();
    void ADAP_UV2();
    void ALG_WORKING2();
    double abs_max2(vector<double> X);
    void COMP_DELT2();
    bool eps_W2(unsigned i); // in uvp.cc according to (3.43), below 
    void COMP_RES_EPS2();
    bool eps_E2(unsigned i); 
    bool eps_S2(unsigned j);
    bool eps_N2(unsigned j);
    void FLAG_PP(); // post process of the flag array. gives each cell a value depending on its type (in or outside the obstacle), its neighbouring cell type, and the boundary condition 
    double x(unsigned i); // need to convert to double to get correct results :(
    double y(unsigned j);
};