#ifndef crossover_h
#define crossover_h

#include "utility.h"
#include <assert.h>
#include <stdlib.h>

/* Crossover operators */
int *ox1(int *const p1, int *const p2, int *const plyrs, int numPlyrs,
         int numTms, int rand1, int rand2, bool verify);
int *pmx(int *const p1, int *const p2, int *const plyrs, int numPlyrs,
         int numTms, int rand1, int rand2, bool verify);
int *cx(int *const p1, int *const p2, int *const plyrs, int numPlyrs,
        int numTms, int rand1, int rand2, bool verify);

#endif /* crossover_h */
