#ifndef ga_h
#define ga_h

#include "crossover.h"
#include "mutate.h"
#include "selection.h"
#include "utility.h"

#include <assert.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

int *run(int numPlyrs, int *plyrs, int numTms, double *variance);
void *optimize(void *arguments);
int iterate(double *scrs, int **pop, int *const plyrs, int numPlyrs, int numTms,
            int *it, double *variance, int mutate_prob, int idx);

#endif /* ga_h */
