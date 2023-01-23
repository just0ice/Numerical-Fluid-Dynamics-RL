#include "grid.h"

void grid::PRINT_TO_TERMINAL(vector<double> X, unsigned iend, unsigned jend){
    for (auto j=jend+1; j-- > 0; ){
        for (auto i=0; i != iend + 1; ++i){
            cout << X[i + (iend+1)*j] << " ";
        }
        cout << "\n";
    } 
}

void grid::PRINT_UVP(){
    cout << "U" << endl;
    PRINT_TO_TERMINAL(U,imax+1,jmax+1);
    cout << "V" << endl;
    PRINT_TO_TERMINAL(V,imax+1,jmax+1);
    cout << "P" << endl;
    PRINT_TO_TERMINAL(P,imax+1,jmax+1);

    if (Ucc.empty() || Vcc.empty() || Pcc.empty())
        CC_AVERAGE_UV();
    cout << "Ucc" << endl;
    PRINT_TO_TERMINAL(Ucc,imax-1,jmax-1);
    cout << "Vcc" << endl;
    PRINT_TO_TERMINAL(Vcc,imax-1,jmax-1);
    cout << "Pcc" << endl;
    PRINT_TO_TERMINAL(Pcc,imax-1,jmax-1);

    cout << "done" << endl;
}

// write UVP into a tsv file. 3 files for each U,V,P. one line per time stamp
void grid::ADD_TO_FILE(string fname, vector<double> X){
    std::ofstream file;
    file.open (fname, ios_base::app);
    file << imax << "\t" << jmax << "\t" << xlength << "\t" << ylength << endl;

    for (auto i = X.cbegin(); i != X.cend() ; ++i){
        if (i == X.cbegin())
            file << *i;
        else file << "\t" << *i;
    }

    file << endl;
    file.close();
    cout << fname << " updated." << endl;
}

void grid::CC_AVERAGE_UV(){
    double average; // temporary variable for better readability
    
    for (auto j=1; j != jmax + 1; ++j){
        for (auto i=1; i != imax + 1; ++i){
            // Average to calculate Ucc and Vcc on physical cells (without boundary)
            average = (U[i + (imax+2) * j] + U[(i-1) + (imax+2) * j]) / 2;
            Ucc.push_back(average);
            average = (V[i + (imax+2) * j] + V[i + (imax+2) * (j-1)]) / 2;
            Vcc.push_back(average);

            // copy physical cells for P (without boundarys)
            Pcc.push_back(P[i + (imax+2) * j]);
        }
    }


}

void grid::OUTPUTVEC(){
    if (Ucc.empty() || Vcc.empty() || Pcc.empty())
        CC_AVERAGE_UV();
    ADD_TO_FILE("Ucc.tsv", Ucc);
    ADD_TO_FILE("Vcc.tsv", Vcc);
    ADD_TO_FILE("Pcc.tsv", Pcc);

    // remove the boundary edges of the domain from the flag array. Flag out is double to be able to use ADD_TO_FILE which expects a double vector
    vector<double> FLAG_OUT;
    for (auto j=1; j != jmax + 1; ++j){
        for (auto i=1; i != imax + 1; ++i){
            FLAG_OUT.push_back(FLAG[id(i,j)]);
        }
    }
    ADD_TO_FILE("flag.tsv", FLAG_OUT);
}


void grid::CLEAR_OUTPUT_FILES(){
    std::ofstream file;

    for (auto fname : {"Ucc.tsv","Vcc.tsv","Pcc.tsv","res.tsv","flag.tsv","temp.tsv"}){
        file.open(fname, std::ofstream::out | std::ofstream::trunc);
        file.close();
    }
}

// Initialize test data with increasing values on each line and -1 at boundary
void grid::INIT_TEST_DATA(){
    // inside
    for (auto j=1; j != jmax + 1; ++j){
        for (auto i=1; i != imax + 1; ++i){
            U[i + (imax+2)*j] = i + (imax+2)*j;
            V[i + (imax+2)*j] = i + (imax+2)*j;
            P[i + (imax+2)*j] = i + (imax+2)*j;
        }
    } 

    unsigned boundi[2] = {0,imax+1};
    unsigned boundj[2] = {0,jmax+1};
    
    for (auto i : boundi){
        for (auto j=0; j != jmax + 2; ++j){
            U[i + (imax+2)*j] = -1;
            V[i + (imax+2)*j] = -1;
            P[i + (imax+2)*j] = -1;
        }
    } 
    for (auto j : boundj){
        for (auto i=0; i != imax + 2; ++i){
            U[i + (imax+2)*j] = -1;
            V[i + (imax+2)*j] = -1;
            P[i + (imax+2)*j] = -1;
        }
    } 

    cout << "done" << endl;
}

unsigned grid::id(unsigned i, unsigned j){
    // function to convert i,j to 1d index. Used for better readability of the code
    return i + (imax+2)*j;

}