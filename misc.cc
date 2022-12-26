#include "grid.h"

void grid::PRINT_U(){
    for (auto i=0; i != imax + 2; ++i){
        for (auto j=0; j != jmax + 2; ++j)
            cout << U[i + (imax+2)*j] << " ";
        cout << "\n";
    } 
    cout << "done" << endl;
}

// write UVP into a tsv file. 3 files for each U,V,P. one line per time stamp
void grid::ADD_TO_FILE(){
    std::ofstream file;
    
    file.open ("U.tsv", std::ios_base::app);
    file << imax << "\t" << jmax << endl;
    for (auto i = U.cbegin(); i != U.cend() ; ++i){
        if (i == U.cbegin())
            file << *i;
        else file << "\t" << *i;
    }
    file << endl;
    file.close();
    
    //Repeteat for V and P
    file.open ("V.tsv", std::ios_base::app);
    file << imax << "\t" << jmax << endl;
    for (auto i = V.cbegin(); i != V.cend() ; ++i){
        if (i == V.cbegin())
            file << *i;
        else file << "\t" << *i;
    }
    file << endl;
    file.close();

    file.open ("P.tsv", std::ios_base::app);
    file << imax << "\t" << jmax << endl;
    for (auto i = P.cbegin(); i != P.cend() ; ++i){
        if (i == P.cbegin())
            file << *i;
        else file << "\t" << *i;
    }
    file << endl;
    file.close();

    cout << "Complete: grid written to files" << endl;
}

void grid::CLEAR_OUTPUT_FILES(){
    std::ofstream file;
    file.open("U.tsv", std::ofstream::out | std::ofstream::trunc);
    file.close();
    file.open("V.tsv", std::ofstream::out | std::ofstream::trunc);
    file.close();
    file.open("P.tsv", std::ofstream::out | std::ofstream::trunc);
    file.close();
}

// Initialize test data with increasing values on each line and -1 at boundary
void grid::INIT_TEST_DATA(){
    // inside
    for (auto i=1; i != imax + 1; ++i){
        for (auto j=1; j != jmax + 1; ++j){
            U[i + (imax+2)*j] = i;
            V[i + (imax+2)*j] = i;
            P[i + (imax+2)*j] = i;
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