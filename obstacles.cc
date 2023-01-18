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

void grid::DOMAIN_BOUNDARY(){
    for (auto j=0; j != imax+2; j++){
        FLAG[id(0,j)] = wE;
        FLAG[id(imax+1,j)] = wW;
    }
    for (auto i=0; i != imax+2; i++){
        FLAG[id(i,jmax+1)] = wN;
        FLAG[id(i,0)] = wS;
    }
}

void grid::RECTANGLE(unsigned bcond, double x_lo, double y_lo, double hight, double width){
    // bcond for obstacle: 1 free-slip, 2 no-slip, 3 outflow
    // populate the FLAG array with a rectangle.
    for (auto j=1; j != jmax + 1; ++j){
        for (auto i=1; i != imax + 1; ++i){
            if ( x(i) >= x_lo && x(i) <= x_lo + width && y(j) <= y_lo && y(j) >= y_lo - hight) FLAG[id(i,j)] = bcond;
        }
    }
}

void grid::CIRCLE(unsigned bcond, double x_center, double y_center, double radius){
    // bcond for obstacle: 1 free-slip, 2 no-slip, 3 outflow
    // populate the FLAG array with a circle.
    for (auto j=1; j != jmax + 1; ++j){
        for (auto i=1; i != imax + 1; ++i){
            if ( pow(x(i) - x_center,2) + pow(y(j) - y_center,2) <= pow(radius,2)) FLAG[id(i,j)] = bcond;
        }
    }
}

void grid::FLAG_PP(){
    vector<unsigned int> new_FLAG = vector<unsigned int>((imax + 2)*(jmax + 2),0); 
    unsigned edge_bin;
    unsigned bcond;

    for (auto j=0; j != jmax + 2; ++j){
        for (auto i=0; i != imax + 2; ++i){
            edge_bin = 0;
            // obstacle cell
            if (FLAG[id(i,j)]){

                // if clauses to check for domain boundary 
                if (i != 0) edge_bin += !(bool)FLAG[id(i-1,j)] * pow(2,3);
                if (i != imax+1) edge_bin += !(bool)FLAG[id(i+1,j)] * pow(2,2);
                if (j != jmax+1) edge_bin += !(bool)FLAG[id(i,j+1)] * pow(2,1);
                if (j != 0) edge_bin += !(bool)FLAG[id(i,j-1)] * pow(2,0);

                bcond = FLAG[id(i,j)];
                // edge_bin: East 8, West 4, North 2, South 1, EN 10, ES 9, WN 6, WS 5 
                // k in obstacle: East 3, West 5, North 7, South 9, EN 11, ES 13, WN 15, WS 17 
                switch (edge_bin)
                {
                case 0:
                    // inside
                    new_FLAG[id(i,j)] = 1;
                    break;
                case 1:
                    // South
                    new_FLAG[id(i,j)] = 16 * (bcond - 1) + 9;
                    break;
                case 2:
                    // North
                    new_FLAG[id(i,j)] = 16 * (bcond - 1) + 7;
                    break;
                case 4:
                    // West
                    new_FLAG[id(i,j)] = 16 * (bcond - 1) + 5;
                    break;
                case 5:
                    // WS
                    new_FLAG[id(i,j)] = 16 * (bcond - 1) + 17;
                    break;
                case 6:
                    // WN
                    new_FLAG[id(i,j)] = 16 * (bcond - 1) + 15;
                    break;
                case 8:
                    // E
                    new_FLAG[id(i,j)] = 16 * (bcond - 1) + 3;
                    break;
                case 9:
                    // ES
                    new_FLAG[id(i,j)] = 16 * (bcond - 1) + 13;
                    break;
                case 10:
                    // EN
                    new_FLAG[id(i,j)] = 16 * (bcond - 1) + 11;
                    break;
                default:
                    cout << "WARNING! Boundary cell with inadmissable edges at i = " << i << ", j = " << j << endl;
                    break;
                }
            }
            // flow cell
            // lets see if it works without
        }
    }

    FLAG = new_FLAG;
}
