#include "grid.h"

void grid::PRINT_U(){
    for (auto i=n_ghost; i != imax + n_ghost; ++i){
        for (auto j=n_ghost; j != jmax + n_ghost; ++j)
            cout << U[i + imax*j] << " ";
        cout << "\n";
    } cout << endl;
}

// write UVP into a tsv file. 3 files for each U,V,P. one line per time stamp
void grid::ADD_TO_FILE(){
    std::ofstream file;
    
    file.open ("U.tsv", std::ios_base::app);
    for (auto it = U.cbegin(); it != U.cend() ; ++it)
        file << *it << "\t";
    file << endl;
    file.close();
    
    //Repeteat for V and P
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