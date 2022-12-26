// main program
#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <fstream>

using std::cout; using std::cin; using std::endl; using std::cerr; using std::string; using std::vector; using std::array;


class data
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
    data(/* args */);
    ~data();

    // (for now) public member functios 
    vector<double> U, V, P; // not sure wether its better to use array here for performance. might be a problem because of fixed length so need to be carefull when to initialize. lets keep vector for now
    
    void PRINT_U(); // print U as a matrix into the Terminal
    void ADD_TO_FILE();
    void CLEAR_OUTPUT_FILES();
};

data::data(/* args */)
{
    READ_PARAMETER("settings.in"); // file doesnt exist yet
    INIT_UVP();
}

data::~data()
{
}

int data::READ_PARAMETER(string inputfile){
    // hardcode for now. make input file work later.
    UI = 0;
    VI = 0;
    PI = 0;

    // grid dimensions
    imax = 10;
    jmax = 10;
    n_ghost = 2;

    return 0;
}

void data::INIT_UVP(){

    U = vector<double>((imax + 2*n_ghost)*(jmax + 2*n_ghost),UI); // taking a 1D vector. Pseudo 2D conversion is i = x + width*y, x = i % width; y = i / width. Ghost zones at corresponding i,j.
    V = vector<double>((imax + 2*n_ghost)*(jmax + 2*n_ghost),VI);
    P = vector<double>((imax + 2*n_ghost)*(jmax + 2*n_ghost),PI);
}

void data::PRINT_U(){
    for (auto i=n_ghost; i != imax + n_ghost; ++i){
        for (auto j=n_ghost; j != jmax + n_ghost; ++j)
            cout << U[i + imax*j] << " ";
        cout << "\n";
    } cout << endl;
}

// write UVP into a tsv file. 3 files for each U,V,P. one line per time stamp
void data::ADD_TO_FILE(){
    std::ofstream file;
    
    file.open ("U.tsv", std::ios_base::app);
    for (auto it = U.cbegin(); it != U.cend() ; ++it)
        file << *it << "\t";
    file << endl;
    file.close();

    file.open ("V.tsv", std::ios_base::app);
    for (auto it = V.cbegin(); it != V.cend() ; ++it)
        file << *it << "\t";
    file << endl;
    file.close();

    file.open ("P.tsv", std::ios_base::app);
    for (auto it = P.cbegin(); it != P.cend() ; ++it)
        file << *it << "\t";
    file << endl;
    file.close();

    cout << "Complete: data written to files" << endl;
}

void data::CLEAR_OUTPUT_FILES(){
    std::ofstream file;
    file.open("U.tsv", std::ofstream::out | std::ofstream::trunc);
    file.close();
    file.open("V.tsv", std::ofstream::out | std::ofstream::trunc);
    file.close();
    file.open("P.tsv", std::ofstream::out | std::ofstream::trunc);
    file.close();
}


int main()
{
    data data1;
    data1.ADD_TO_FILE();
    data1.CLEAR_OUTPUT_FILES();
    //data1.PRINT_U();

    return 0;
}