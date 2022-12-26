#include "grid.h"

void grid::PRINT_U(){
    for (auto i=0; i != imax + 1; ++i){
        for (auto j=0; j != jmax + 1; ++j)
            cout << U[i + (imax+1)*j] << " ";
        cout << "\n";
    } cout << endl;
}

// write UVP into a tsv file. 3 files for each U,V,P. one line per time stamp
void grid::ADD_TO_FILE(){
    std::ofstream file;
    
    file.open ("U.tsv", std::ios_base::app);
    for (auto i = U.cbegin(); i != U.cend() ; ++i)
        file << *i << "\t";
    file << endl;
    file.close();
    
    //Repeteat for V and P
    file.open ("V.tsv", std::ios_base::app);
    for (auto i= V.cbegin(); i!= V.cend() ; ++it)
        file << *i<< "\t";
    file << endl;
    file.close();

    file.open ("P.tsv", std::ios_base::app);
    for (auto i= P.cbegin(); i!= P.cend() ; ++it)
        file << *i<< "\t";
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