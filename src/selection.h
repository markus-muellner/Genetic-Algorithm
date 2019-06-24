#ifndef selection_h
#define selection_h

#include "sort.h"
#include "utility.h"
#include <stdlib.h>

void fps(double *const scrs, int pop_size, int *p1, int *p2, int rand1,
         int rand2);
void ts(double *const scrs, int pop_size, int *p1, int *p2, int rand1,
        int rand2);
void rbs(double *const scrs, int pop_size, int *p1, int *p2, int rand1,
         int rand2);

#endif /* selection_h */
