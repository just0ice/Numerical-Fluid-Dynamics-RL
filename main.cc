// main program
#include "grid.h"

int main()
{
    grid grid1;
    
    //grid1.ALG_BASE();
    //grid1.ALG_ALL();

    //grid1.ALG_TEST_POISSON();
    //grid1.ALG_TEST_POISSON();
    grid1.dev = true;
    grid1.CLEAR_OUTPUT_FILES();

    grid1.READ_PARAMETER("settings.in");
    grid1.INIT_UVP();

    grid1.INIT_TEST_DATA();

    // set RHS to 0 (for which the solution is known)
    for (auto j=1; j != grid1.jmax+1; ++j){
        for (auto i=1; i != grid1.imax+1; ++i){
            grid1.RHS[grid1.id(i,j)] = 0;
        }
    }

    grid1.POISSON();

    grid1.ADAP_UV();

    grid1.OUTPUTVEC();


    return 0;
}