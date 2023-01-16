// Functions to generate a flag array for an obstacle course 

#include "grid.h"

void grid::PRINT_FLAG(){
    for (auto j=jmax+2; j-- > 0; ){
        for (auto i=0; i != imax + 2; ++i){
            cout << FLAG[i + (imax+2)*j] << " ";
        }
        cout << "\n";
    } 
}

double grid::x(unsigned i){
    return  xlength/imax * (i - 0.5); // need 0.5 instead of 1/2 for correct result
}

double grid::y(unsigned j){
    return  ylength/jmax * (j - 0.5); // need 0.5 instead of 1/2 for correct result
}

void grid::RECTANGLE(unsigned bcond, double x_lo, double y_lo, double hight, double width){
    // bcond for obstacle: 1 free-slip, 2 no-slip
    // populate the FLAG array with a rectangle.
    for (auto j=1; j != jmax + 1; ++j){
        for (auto i=1; i != imax + 1; ++i){
            if ( x(i) >= x_lo && x(i) <= x_lo + width && y(j) >= y_lo && y(j) <= y_lo + hight) FLAG[id(i,j)] = bcond;
        }
    }

    for (auto j=1; j != jmax + 1; ++j){
        for (auto i=1; i != imax + 1; ++i){
            if ( x(i) >= x_lo && x(i) <= x_lo + width && y(j) >= y_lo && y(j) <= y_lo + hight) FLAG[id(i,j)] = bcond;
        }
    }
}

void grid::FLAG_PP(){
    vector<unsigned int> new_FLAG = vector<unsigned int>((imax + 2)*(jmax + 2),0); 
    unsigned edge_bin;

    for (auto j=1; j != jmax + 1; ++j){
        for (auto i=1; i != imax + 1; ++i){
            edge_bin = 0;
            // obstacle cell
            if (FLAG[id(i,j)]){
                edge_bin += !(bool)FLAG[id(i+1,j)] * pow(2,3);
                edge_bin += !(bool)FLAG[id(i-1,j)] * pow(2,2);
                edge_bin += !(bool)FLAG[id(i,j+1)] * pow(2,1);
                edge_bin += !(bool)FLAG[id(i,j-1)] * pow(2,0);
                new_FLAG[id(i,j)] = edge_bin;
            }
            // flow cell
            if (!FLAG[id(i,j)]){
                
            }
        }
    }

    FLAG = new_FLAG;
}
