#include "rand.h"
#include <stdlib.h>

unsigned int _seed;

unsigned int myrand()
{
    return (unsigned int)(rand() + _seed);
}

void mysrand(unsigned int seed)
{
    _seed = seed;
}