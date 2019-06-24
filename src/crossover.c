#include "crossover.h"

/* Crossover operators */
int *ox1(int *const p1, int *const p2, int *const plyrs, int numPlyrs,
         int numTms, int rand1, int rand2, bool verify) {
  /*
   Crossover is based on the "order one crossover" principle.
   @param p1: Array of the first organism used for crossover
   @param p2: Array of the second organism used for crossover
   @param plyrs: Array with the hcps of all players; eg: 0 1 3 5 7 9
   @param numPlyrs: Number of players
   @param numTms: Number of teams
   @param rand1: Random number used for determining the lower segment bound
   @param rand2: Random number used for determining the upper segment bound
   @param verify: Ensures that the offspring is a valid permutation if true

   @return offsprg: Returns the generated offspring
   */

  /* Declare local variables */
  int lower, upper, i, cIdx;
  int *offsprg;

  /* Allocate offsprg */
  offsprg = initialize(numPlyrs, numTms, false);

  /* Select bounds */
  getBounds(numPlyrs, &lower, &upper, rand1, rand2);

  /* Copy p1 in selected range */
  for (i = lower; i <= upper; i++) {
    offsprg[i % numPlyrs] = p1[i % numPlyrs];
  }

  /* Helper index for offsprg array */
  cIdx = (upper + 1);

  /* Copy p2 in remaining range */
  for (i = upper + 1; i <= (numPlyrs + upper); i++) {

    /*  Set offsprg element to value if value is not already in offsprg */
    int value = p2[i % numPlyrs];
    if (!contains(offsprg, value, numPlyrs)) {
      offsprg[cIdx % numPlyrs] = value;
      cIdx++;
    }
  }

  /* Verify offspring and return */
  if (verify) {
    assert(isValid(offsprg, numPlyrs));
  }
  return offsprg;
}

int *pmx(int *const p1, int *const p2, int *const plyrs, int numPlyrs,
         int numTms, int rand1, int rand2, bool verify) {
  /*
   Crossover is based on the "partially matched crossover" principle:

   @param p1: Array of the first organism used for crossover
   @param p2: Array of the second organism used for crossover
   @param plyrs: Array with the hcps of all players; eg: 0 1 3 5 7 9
   @param numPlyrs: Number of players
   @param numTms: Number of teams
   @param rand1: Random number used for determining the lower segment bound
   @param rand2: Random number used for determining the upper segment bound
   @param verify: Ensures that the offspring is a valid permutation if true

   @return offsprg: Returns the generated offspring
   */

  /* Declare local variables */
  int *offsprg, *a;
  int i, lower, upper, l, aIdx;

  /* Allocate offsprg */
  offsprg = initialize(numPlyrs, numTms, false);

  /* Select bounds */
  getBounds(numPlyrs, &lower, &upper, rand1, rand2);

  /* Copy segment from p1 to offsprg */
  for (i = lower; i <= upper; i++) {
    offsprg[i % numPlyrs] = p1[i % numPlyrs];
  }

  /* Determine length for array a */
  l = 0;
  for (i = lower; i <= upper; i++) {
    if (!contains(offsprg, p2[i % numPlyrs], numPlyrs)) {
      l++;
    }
  }

  /* Allocate a */
  a = malloc(l * sizeof(int));
  for (i = 0; i < l; i++) {
    a[i] = -1;
  }

  /* Helper index for array a */
  aIdx = 0;

  /* Looking in the same segment positions in p2, select each value that is not
     already in child */
  for (i = lower; i <= upper; i++) {
    if (!contains(offsprg, p2[i % numPlyrs], numPlyrs)) {
      a[aIdx] = p2[i % numPlyrs];
      aIdx++;
    }
  }

  /* For each of the values in a... */
  for (i = 0; i < l; i++) {
    /* Helper variables */
    int posA, posB, b;
    bool first_run;

    posA = -1;
    posB = lower;
    b = -1;
    first_run = true;

    /* (iii) If the index of this value in p2 is part of the segment, go
       to step (i) with this value b */
    do {
      /* (i) Note the index of this value in p2... */
      if (first_run) {
        posA = getPos(p2, a[i], numPlyrs);
      } else {
        posA = getPos(p2, b, numPlyrs);
      }

      /* ... and locate the value from p1 at the same position */
      b = p1[posA];

      /* (ii) Locate this same value in p2 */
      posB = getPos(p2, b, numPlyrs);
      first_run = false;

    } while (posB >= lower && posB <= upper);

    /* (iv) Insert value at this new position */
    offsprg[posB] = a[i];
  }

  /* Copy any remaining positions from p2 to offsprg */
  for (i = 0; i < numPlyrs; i++) {
    if (isOcc(offsprg, i) == -1) {
      offsprg[i] = p2[i];
    }
  }

  /* Verify offspring and return */
  if (verify) {
    assert(isValid(offsprg, numPlyrs));
  }

  free(a);
  return offsprg;
}

int *cx(int *const p1, int *const p2, int *const plyrs, int numPlyrs,
        int numTms, int rand1, int rand2, bool verify) {
  /*
   Crossover is based on the "cx crossover" principle.

   @param p1: Array of the first organism used for crossover
   @param p2: Array of the second organism used for crossover
   @param plyrs: Array with the hcps of all players; eg: 0 1 3 5 7 9
   @param numPlyrs: Number of players
   @param numTms: Number of teams
   @param rand1: Random number used for determining the lower segment bound
   @param rand2: Random number used for determining the upper segment bound
   @param verify: Ensures that the offspring is a valid permutation if true

   @return offsprg: Returns the generated offspring
   */

  /* Declare local variables */
  int *offsprg1, *offsprg2, *better_offsprg, *entry;
  int i, ep, c;
  int **p1c;
  int **p2c;

  /* Allocate cycles */
  p1c = malloc(numPlyrs * sizeof(int *));
  p2c = malloc(numPlyrs * sizeof(int *));
  for (i = 0; i < numPlyrs; i++) {
    p1c[i] = malloc(numPlyrs * sizeof(int));
    p2c[i] = malloc(numPlyrs * sizeof(int));
  }

  /* Allocate offsprg */
  offsprg1 = initialize(numPlyrs, numTms, false);
  offsprg2 = initialize(numPlyrs, numTms, false);
  entry = malloc(numPlyrs * sizeof(int));

  /* Create and initialize arrays for the cycles with worst case sizes */
  for (i = 0; i < numPlyrs; i++) {
    int j = 0;
    for (j = 0; j < numPlyrs; j++) {
      p1c[i][j] = -1;
      p2c[i][j] = -1;
    }
  }

  /* Create array to store already used entry points */
  for (i = 0; i < numPlyrs; i++) {
    entry[i] = -1;
  }

  /* Initial entry point and cycle counter */
  ep = 0;
  c = 0;

  /* While there are entry points left */
  do {
    /* Declare local variables */
    int first, p, current;

    /* Mark entry point as used */
    entry[ep] = 1;

    /* Initiate cycle */
    first = p1[ep];
    p = ep;
    p1c[c][p] = first;
    current = -1;

    /* Identify cycle */
    do {
      current = p2[p];
      p2c[c][p] = current;
      p = getPos(p1, current, numPlyrs);
      p1c[c][p] = current;

      /* Mark entry point as used */
      entry[p] = 1;
    } while (current != first);

    /* Find new entry point and increase cycle counter */
    ep = getPos(entry, -1, numPlyrs);
    c++;
  } while (ep != -1);

  /* Loop through cycles */
  for (i = 0; i < c; i++) {
    if (i % 2 == 0) {
      /* Even cycles; copy 1 to 1 and 2 to 2 */
      int j = 0;
      for (j = 0; j < numPlyrs; j++) {
        if (p1c[i][j] != -1) {
          offsprg1[j] = p1c[i][j];
          offsprg2[j] = p2c[i][j];
        }
      }
    } else {
      /* Odd cycles; copy 1 to 2 and 2 to 1 */
      int j = 0;
      for (j = 0; j < numPlyrs; j++) {
        if (p1c[i][j] != -1) {
          offsprg1[j] = p2c[i][j];
          offsprg2[j] = p1c[i][j];
        }
      }
    }
  }

  /* De-allocate memory */
  for (i = 0; i < numPlyrs; i++) {
    free(p1c[i]);
    free(p2c[i]);
  }
  free(p1c);
  free(p2c);
  free(entry);

  /* Verify offsprings */
  if (verify) {
    assert(isValid(offsprg1, numPlyrs));
    assert(isValid(offsprg2, numPlyrs));
  }

  better_offsprg =
      getBetterOffsprg(offsprg1, offsprg2, plyrs, numPlyrs / numTms, numTms);

  return better_offsprg;
}
