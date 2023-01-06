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
    int POISSON();
    void ADAP_UV();

    // additional member functions. can be found in misc.cc
    void PRINT_UVP(); // print U as a matrix into the Terminal
    void PRINT_TO_TERMINAL(vector<double> X, unsigned iend, unsigned jend);
    void ADD_TO_FILE(string fname, vector<double> X); // writes vector X to file fname
    void OUTPUTVEC(); // see 4.1.4 on page 54 
    void CC_AVERAGE_UV(); // computes the cc average of U and V and stores the result in Ucc and Vcc
    void CLEAR_OUTPUT_FILES();
    void INIT_TEST_DATA();
    unsigned id(unsigned i, unsigned j);

    // more additional member functions
    void COMP_SPATIAL_DERIVATIVES(); // in uvp.cc . according to 3.19
    void CHECK_HIRT(); // in uvp.cc . according to 3.20
    void COMP_RES(); // in uvp.cc according to (3.45) and (3.46)

    // Algorithms
    void ALG_BASE(string inputfile);
    void ALG_ALL(string inputfile);
    void ALG_TEST_POISSON(string inputfile);
};