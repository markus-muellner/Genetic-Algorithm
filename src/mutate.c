#include "mutate.h"

void swap(int *organism, int numPlyrs, int mutation_p, int rand1, int rand2,
          bool verify) {
  /*
   Mutation is based on the "Swap mutation" principle.

   @param organism: Array of the organism that is mutated
   @param numPlyrs: Number of players in the organism
   @param mutation_p: Mutation probability
   @param rand1: Random number used for selecting the first index for swapping
   @param rand2: Random number used for selecting the second index for swapping
   @param verify: Ensures that the offspring is a valid permutation if true

   @return void: Organism array is modified directly
   */

  /* Declare local variables */
  int r, tmp;

  /* Random number between 1 and 100 */
  r = (rand() % 100) + 1;

  /* Mutate by a chance of mutation_p */
  if (r <= mutation_p) {

    /* Select two different random numbers */
    getIndexes(numPlyrs, &rand1, &rand2);

    /* Swap elements */
    tmp = organism[rand2];
    organism[rand2] = organism[rand1];
    organism[rand1] = tmp;

    if (verify) {
      assert(isValid(organism, numPlyrs));
    }
  }
}

void insert(int *organism, int numPlyrs, int mutation_p, int rand1, int rand2,
            bool verify) {
  /*
   Mutation is based on the "Insert mutation" principle.

   @param organism: Array of the organism that is mutated
   @param numPlyrs: Number of players in the organism
   @param mutation_p: Mutation probability
   @param rand1: Random number used for selecting the first index for swapping
   @param rand2: Random number used for selecting the second index for swapping
   @param verify: Ensures that the offspring is a valid permutation if true

   @return void: Organism array is modified directly
   */

  /* Declare local variables */
  int r, point1, i;

  /* Random number between 1 and 100 */
  r = (rand() % 100) + 1;

  /* Mutate by a chance of mutation_p */
  if (r <= mutation_p) {

    /* Select two different random numbers */
    getIndexes(numPlyrs, &rand1, &rand2);

    /* Temporarily store player that will be moved */
    point1 = organism[rand1];

    /* Shift the players left */
    for (i = 0; i < (rand2 - rand1); i++) {
      organism[rand1 + i] = organism[rand1 + i + 1];
    }

    /* Insert player */
    organism[rand2] = point1;

    if (verify) {
      assert(isValid(organism, numPlyrs));
    }
  }
}

void inversion(int *organism, int numPlyrs, int mutation_p, int rand1,
               int rand2, bool verify) {
  /*
   Mutation is based on the "inversion mutation" principle.
   @param organism: Array of the organism that is mutated
   @param numPlyrs: Number of players in the organism
   @param mutation_p: Mutation probability
   @param rand1: Random number used for selecting the first index for swapping
   @param rand2: Random number used for selecting the second index for swapping
   @param verify: Ensures that the offspring is a valid permutation if true

   @return void: Organism array is modified directly
   */

  /* Declare local variables */
  int r, i, j, mid;
  int *tmp;

  /* Random number between 1 and 100 */
  r = (rand() % 100) + 1;

  /* Mutate by a chance of mutation_p */
  if (r <= mutation_p) {

    /* Select two different random numbers */
    getIndexes(numPlyrs, &rand1, &rand2);

    /* Initialize helper index for the tmp array */
    if ((rand2 - rand1) % 2 == 0) {
      j = 0;
    } else {
      j = 1;
    }

    /* Initialize array for temporarily storing values */
    mid = (rand2 + 1 - rand1) / 2;
    tmp = malloc(mid * sizeof(int));

    /* Invert values in selected range */
    for (i = 0; i < (rand2 - rand1); i++) {
      if (i < mid) {
        tmp[i] = organism[rand1 + i];
        organism[rand1 + i] = organism[rand2 - i - 1];
      } else {
        organism[rand1 + i] = tmp[mid - ++j];
      }
    }

    /* De-allocate tmp array */
    free(tmp);

    if (verify) {
      assert(isValid(organism, numPlyrs));
    }
  }
}
