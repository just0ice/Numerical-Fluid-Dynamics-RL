// main program
#include "grid.h"

int main()
{
    grid grid1;
    grid1.SETBCOND();
    //grid1.INIT_TEST_DATA();
    // grid1.PRINT_UVP();
    grid1.CLEAR_OUTPUT_FILES();
    grid1.OUTPUTVEC();
    return 0;
}