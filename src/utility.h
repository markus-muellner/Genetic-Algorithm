#ifndef utility_h
#define utility_h

#include <assert.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

/* General helper functions */
bool contains(int *const arr, int el, int numEl);
int getPos(int *const arr, int el, int numEl);
int isOcc(int *const arr, int pos);
int *initialize(int numPlyrs, int numTms, bool idx);
void randomize(int numPlyrs, int *tms, int numTms);
int getItem(int t, int p, int *const tms, int pPerT, int numTms);
void getBounds(int numPlyrs, int *lower, int *upper, int rand1, int rand2);
void getIndexes(int numPlyrs, int *rand1, int *rand2);

/* Score related helper functions */
void getMinMax(int *const tms, int *const plyrs, int pPerT, int numTms,
               int *min, int *max);
int determineBest(double *const scrs, int pop_size);
int determineWorst(double *const scrs, int pop_size);
double getDelta(int *const tms, int *const plyrs, int pPerT, int numTms);
double getScore(int *const tms, int *const plyrs, int pPerT, int numTms);

/* Result validation and visualization */
bool isValid(int *const arr, int numEl);
void printArray(int *const arr, int l);
void printTeams(int *const tms, int *const plyrs, int pPerT, int numTms,
                bool withScr);

/* Crossover helpers */
int *getBetterOffsprg(int *const offsprg1, int *const offsprg2,
                      int *const plyrs, int pPerT, int numTms);

#endif /* utility_h */
