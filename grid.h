#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <fstream>

using namespace::std;

class grid
{
private:
    // size of the grid
    unsigned imax; // book says int but this should hopefully be ok. reevaluate if imax / jmax are used for arithmetics
    unsigned jmax;
    unsigned n_ghost; // number of ghost zones to implement boundary conditions

    // initial values
    double UI;
    double VI;
    double PI;

    // declar member functions
    int READ_PARAMETER(string inputfile);
    void INIT_UVP();

public:
    // construction and stuff
    grid(/* args */);
    ~grid();

    // (for now) public member functios 
    vector<double> U, V, P; // not sure wether its better to use array here for performance. might be a problem because of fixed length so need to be carefull when to initialize. lets keep vector for now
    
    void PRINT_U(); // print U as a matrix into the Terminal
    void ADD_TO_FILE();
    void CLEAR_OUTPUT_FILES();
};