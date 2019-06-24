#ifndef mutate_h
#define mutate_h

#include "utility.h"
#include <assert.h>
#include <stdlib.h>

void swap(int *organism, int numPlyrs, int mutation_p, int rand1, int rand2,
          bool verify);
void insert(int *organism, int numPlyrs, int mutation_p, int rand1, int rand2,
            bool verify);
void inversion(int *organism, int numPlyrs, int mutation_p, int rand1,
               int rand2, bool verify);

#endif /* mutate_h */
