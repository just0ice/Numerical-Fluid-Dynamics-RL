#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <fstream>

using namespace::std;

class grid
{
private:
    // make most of the stuff private later 
public:
    // construction and deconstructions
    grid(/* args */);
    ~grid();

    // 3.3.1 Problem Parameters and Data Structures
    // Geometry data
    double xlenghth, ylength;
    unsigned imax, jmax; // book says int but this should hopefully be ok. reevaluate if imax / jmax are used for arithmetics
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

    // Data arrays
    vector<double> U, V, P; // not sure wether its better to use array here for performance. might be a problem because of fixed length so need to be carefull when to initialize. lets keep vector for now
    vector<double> RHS;
    vector<double> F,G;

    // 3.34 The Program
    // declar member functions according to p.43
    int READ_PARAMETER(string inputfile);
    void INIT_UVP();
    void COMP_DELT();
    void SETBCOND();
    void SETSPECBCOND();
    void COMP_FG();
    void COMP_RHS();
    int POISSON();
    void ADAP_UV();

    // additional member functions  
    void PRINT_U(); // print U as a matrix into the Terminal
    void ADD_TO_FILE();
    void CLEAR_OUTPUT_FILES();
};