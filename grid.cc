#include "grid.h"

grid::grid(/* args */)
{
    READ_PARAMETER("settings.in"); // file doesnt exist yet
    INIT_UVP();
}

grid::~grid()
{
}