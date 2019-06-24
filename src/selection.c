#include "selection.h"

void fps(double *const scrs, int pop_size, int *p1, int *p2, int rand1,
         int rand2) {
  /*
   Selection is based on the "fitness proportionate selection" principle.

   @param scrs: Array with the score of each organism in the population
   @param pop_size: Population size
   @param p1: Reference to int that will store the index of p1
   @param p2: Reference to int that will store the index of p2
   @param rand1: Random number used for determining the lower segment bound
   @param rand2: Random number used for determining the upper segment bound

   @return void: Stores result in p1 and p2 directly
   */

  /* Declare local variables */
  int i, j;
  double scoreSum, r1, r2;
  double *wheel;

  /* Select first parent */
  /* Calculate sum of scores */
  scoreSum = 0.0;
  i = 0;
  for (i = 0; i < pop_size; i++) {
    scoreSum += (1 / scrs[i]);
  }

  /* Calculate ranges for the roulette wheel */
  wheel = malloc((pop_size + 1) * sizeof(double));
  wheel[0] = 0;
  for (i = 1; i <= pop_size; i++) {
    double range = (1 / scrs[i - 1]) / scoreSum;
    wheel[i] = range + wheel[i - 1];
  }

  /* Initialize index for p1 */
  *p1 = 0;

  /* Get random number between 0 and 1 */
  r1 = rand1 / (double)RAND_MAX;

  /* Get p1 index */
  for (i = 1; i < pop_size; i++) {
    if (r1 > wheel[i]) {
      *p1 = i;
    }
  }

  /* Select second parent */
  /* Calculate sum of scores */
  scoreSum -= (1 / scrs[*p1]);

  /* Calculate ranges for the roulette wheel */
  j = 1;
  for (i = 0; i < pop_size; i++) {
    if (i != *p1) {
      double range = (1 / scrs[i]) / scoreSum;
      wheel[j] = range + wheel[j - 1];
      j++;
    }
  }

  /* Initialize index for p2 */
  *p2 = 0;

  /* Get random number between 0 and 1 */
  r2 = rand2 / (double)RAND_MAX;

  /* Get p2 index */
  for (i = 1; i < pop_size - 1; i++) {
    if (r2 > wheel[i]) {
      *p2 = i;
    }
  }
  if (*p2 >= *p1) {
    *p2 = *p2 + 1;
  }

  free(wheel);
  assert(*p1 != *p2);
}

void ts(double *const scrs, int pop_size, int *p1, int *p2, int rand1,
        int rand2) {
  /*
   Selection is based on the "Tournament selection" principle.

   @param scrs: Array with the score of each organism in the population
   @param pop_size: Population size
   @param p1: Reference to int that will store the index of p1
   @param p2: Reference to int that will store the index of p2
   @param rand1: Random number used for determining the lower segment bound
   @param rand2: Random number used for determining the upper segment bound

   @return void: Stores result in p1 and p2 directly
   */

  /* Declare local variables */
  int k, lower, upper, i, best;

  /* Tournament size */
  k = pop_size / 10 - 1;

  /* Get p1 index */
  getBounds(pop_size, &lower, &upper, rand1, k);

  best = lower;
  for (i = lower + 1; i < upper; i++) {
    if (scrs[i % pop_size] < scrs[best]) {
      best = i;
    }
  }
  *p1 = best;

  /* Get p2 index */
  getBounds(pop_size, &lower, &upper, rand2, k);

  best = lower;
  for (i = lower + 1; i < upper; i++) {
    if (scrs[i % pop_size] < scrs[best]) {
      best = i;
    }
  }
  *p2 = best;
}

void rbs(double *const scrs, int pop_size, int *p1, int *p2, int rand1,
         int rand2) {
  /*
   Selection is based on the "Rank based selection" principle.

   @param scrs: Array with the score of each organism in the population
   @param pop_size: Population size
   @param p1: Reference to int that will store the index of p1
   @param p2: Reference to int that will store the index of p2
   @param rand1: Random number used for determining the lower segment bound
   @param rand2: Random number used for determining the upper segment bound

   @return void: Stores result in p1 and p2 directly
   */

  /* Declare local variables */
  int i, indexP1, p;
  double r1, r2;
  double *wheel;
  int *order_mask;
  const double s = 2.0;

  /* Order population by score */
  order_mask = malloc(pop_size * sizeof(int));
  for (i = 0; i < pop_size; i++) {
    order_mask[i] = i;
  }
  mergeSort(order_mask, scrs, pop_size);

  /* Calculate relative ranges */
  wheel = malloc((pop_size + 1) * sizeof(double));
  wheel[0] = 0;
  for (i = 1; i <= pop_size; i++) {
    double range = ((2 - s) / pop_size) +
                   (2 * (i - 1) * (s - 1)) / (pop_size * (pop_size - 1));
    wheel[i] = range + wheel[i - 1];
  }

  /* Initialize indexes for p1 */
  *p1 = order_mask[0];
  indexP1 = 0;

  /* Get random number between 0 and 1 */
  r1 = rand1 / (double)RAND_MAX;

  /* Get p1 index */
  for (i = 1; i < pop_size; i++) {
    if (r1 > wheel[i]) {
      *p1 = order_mask[i];
      indexP1 = i;
    }
  }

  /* Select second parent */
  p = (pop_size - 1);
  for (i = 1; i <= p; i++) {
    double range = ((2 - s) / p) + (2 * (i - 1) * (s - 1)) / (p * (p - 1));
    wheel[i] = range + wheel[i - 1];
  }

  /* Initialize p2 index */
  *p2 = order_mask[0];

  /* Get random number between 0 and 1 */
  r2 = rand2 / (double)RAND_MAX;

  /* Get p2 index */
  for (i = 1; i < pop_size - 1; i++) {
    if (r2 > wheel[i]) {
      if (i >= indexP1) {
        *p2 = order_mask[i + 1];
      } else {
        *p2 = order_mask[i];
      }
    }
  }

  free(wheel);
  free(order_mask);

  assert(*p1 != *p2);
}
