// main program
#include "grid.h"

int main()
{
    grid grid1;
    
    //grid1.ALG_BASE("Lid-Driven Cavity.in");
    //grid1.ALG_ALL("settings.in");

    grid1.ALG_TEST_POISSON("settings.in");

    return 0;
}