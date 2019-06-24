#include "utility.h"

/* General helper functions */
bool contains(int *const arr, int el, int numEl) {
  /*
   @param arr: Array to be searched
   @param el: The value that is searched
   @param numEl: The number of elements in arr

   @return: true if el is in arr; false otherwise
   */

  /* Check if arr contains el */
  int i;

  for (i = 0; i < numEl; i++) {
    if (arr[i] == el) {
      return true;
    }
  }
  return false;
}

int getPos(int *const arr, int el, int numEl) {
  /*
   @param arr: Array to be searched
   @param el: The value that is searched
   @param numEl: The number of elements in arr

   @return pos: The position of el in arr if it is there, -1 otherwise
   */

  /* Find element in array and return its position */
  int i;

  for (i = 0; i < numEl; i++) {
    if (arr[i] == el) {
      return i;
    }
  }
  return -1;
}

int isOcc(int *const arr, int pos) {
  /*
   @param arr: Array to be searched
   @param pos: The position in arr

   @return: arr[pos] if pos in arr is occupied; -1 otherwise
   */

  /* Check if the given position is occupied */
  if (arr[pos] != -1) {
    return arr[pos];
  } else {
    return -1;
  }
}

int *initialize(int numPlyrs, int numTms, bool idx) {
  /*
   @param numPlyrs: Represents the number of elements in plyrs
   @param numTms: Number of teams to be built by the algorithm
   @param idx: Specifies if the values should be initialized with
               their index; -1 is used otherwise

   @return tms: Array representing players in initial
                teams: 0 1 2 4 4 5 ...
   */

  int i;
  int *tms;

  tms = malloc(numPlyrs * sizeof(int));

  for (i = 0; i < numPlyrs; i++) {
    if (idx) {
      tms[i] = i;
    } else {
      tms[i] = -1;
    }
  }
  return tms;
}

void randomize(int numPlyrs, int *tms, int numTms) {
  /*
   @param numPlyrs: Represents the number of elements in tms
   @param tms: Array representing the players in teams
   @param numTms: Represents the number of teams

   @return void: Manipulates the input array tms directly
   */

  int i, r, q, tmp;

  /* Swap numPlyrs elements */
  for (i = 0; i < numPlyrs; i++) {

    /* Initialize random numbers */
    r = -1;
    q = -1;

    /* Select two different random numbers */
    do {
      r = rand() % numPlyrs;
      q = rand() % numPlyrs;
    } while (r == q);

    /* Swap elements */
    tmp = tms[q];
    tms[q] = tms[r];
    tms[r] = tmp;
  }
}

int getItem(int t, int p, int *const tms, int pPerT, int numTms) {
  /*
   @param t: Represents the team to be selected
   @param p: Represents the player in the team to be selected
   @param tms: Array that represents the players in teams
   @param pPerT: Represents the number of playser per team
   @param numTms: Represents the number of teams in tms

   @return player: Returns the player at the selected position
   */

  assert(t < numTms);
  assert(p < pPerT);
  return tms[t * pPerT + p];
}

void getBounds(int numPlyrs, int *lower, int *upper, int rand1, int rand2) {
  /*
   Selects a random interval of numPlyrs defined by its upper and lower bounds.

   @param: numPlyrs: Number of players
   @param lower: Reference to int that will store the lower bound
   @param upper: Reference to int that will store the upper bound
   @param rand1: Random number used for determining the lower segment bound
   @param rand2: Random number used for determining the upper segment bound

   @return void: Stores result in lower and upper directly
   */

  /* Select lower bound */
  *lower = rand1 % numPlyrs;

  /* Select upper bound */
  *upper = rand2 % (numPlyrs - 1) + 1;
  *upper = *lower + *upper;
  *upper = *upper % numPlyrs;

  assert(*lower != *upper);
  assert(*lower < numPlyrs && *upper < numPlyrs);
  assert(0 <= *lower && 0 <= *upper);
}

void getIndexes(int numPlyrs, int *rand1, int *rand2) {
  /*
   Selects two distinct player indexes and stores them directly in the passed
   arguments.

   @param rand1: Random number that is used to determine the first player index
   @param rand2: Random number that is used to determine the second player index
   @param numPlyrs: Number of players

   @return void: The selected indexes are stored directly in the corresponding
                 arguments
   */

  /* Select two different random numbers */
  int lower, upper;

  lower = *rand1;
  upper = *rand2;

  getBounds(numPlyrs, &lower, &upper, *rand1, *rand2);

  /* Order the indexes */
  if (lower > upper) {
    int tmp = lower;
    lower = upper;
    upper = tmp;
  }

  assert(lower < upper);

  *rand1 = lower;
  *rand2 = upper;
}

/* Score related helper functions */
void getMinMax(int *const tms, int *const plyrs, int pPerT, int numTms,
               int *min, int *max) {
  /*
   @param tms: Array that represents the players in teams
   @param plyrs: Array with the hcps of all players; eg: 0 1 3 5 7 9
   @param pPerT: Represents the number of players per team
   @param numTms: Represents the number of teams
   @param min: Reference to int that will store the minimum score
   @param max: Reference to int that will store the maximum score

   @return void: Stores result in min and max directly
   */

  int t, team_sum;

  /* Initialize max */
  *min = INT_MAX;

  /* Initialize min */
  *max = 0;

  /* Loop through all teams */
  for (t = 0; t < numTms; t++) {
    int p;

    /* Initialize team sum */
    team_sum = 0;

    /* Calculate team sum */
    for (p = 0; p < pPerT; p++) {
      int plus = plyrs[getItem(t, p, tms, pPerT, numTms)];
      team_sum += plus;
    }

    /* Replace old min value */
    if (team_sum < *min) {
      *min = team_sum;
    }

    /* Replace old max value */
    if (team_sum > *max) {
      *max = team_sum;
    }
  }
}

int determineBest(double *const scrs, int pop_size) {
  /*
   @param scrs: Array with the score of each organism in the population
   @param pop_size: Population size

   @return idx: Index of the best organism in regard to scrs
   */

  /* Initialize high score and index of best */
  int idx, i;
  double lowestScore;

  idx = 0;
  lowestScore = scrs[0];

  /* Find and return best */
  for (i = 1; i < pop_size; i++) {
    if (scrs[i] < lowestScore) {
      lowestScore = scrs[i];
      idx = i;
    }
  }
  return idx;
}

int determineWorst(double *const scrs, int pop_size) {
  /*
   @param scrs: Array with the score of each organism in the population
   @param pop_size: Population size

   @return idx: Index of the worst organism in regard to scrs
   */

  /* Initialize low score and index of worst */
  int idx, i;
  double highestScore;

  idx = pop_size - 1;
  highestScore = scrs[pop_size - 1];

  /* Find and return worst */
  for (i = pop_size - 2; i >= 0; i--) {
    if (scrs[i] > highestScore) {
      highestScore = scrs[i];
      idx = i;
    }
  }
  return idx;
}

double getDelta(int *const tms, int *const plyrs, int pPerT, int numTms) {
  /*
   @param tms: Array that represents the players in teams
   @param plyrs: Array with the hcps of all players; eg: 0 1 3 5 7 9
   @param pPerT: Represents the number of players per team
   @param numTms: Represents the number of teams

   @return score: Double value representing the score of an organism; lower
                  scores are better
   */

  /* Declare min and max */
  int min, max, diff;

  /* Get min and max values */
  getMinMax(tms, plyrs, pPerT, numTms, &min, &max);

  /* Calculate and return score */
  diff = max - min;
  return diff;
}

double getScore(int *const tms, int *const plyrs, int pPerT, int numTms) {
  /*
   @param tms: Array that represents the players in teams
   @param plyrs: Array with the hcps of all players; eg: 0 1 3 5 7 9
   @param pPerT: Represents the number of players per team
   @param numTms: Represents the number of teams

   @return score: Double value representing the score of an organism; lower
                  scores are better
   */

  int t, p, teamSum;
  double sum = 0, sumSq = 0;

  /* Calculate sum and squared sum of scores */
  for (t = 0; t < numTms; t++) {
    teamSum = 0;
    for (p = 0; p < pPerT; p++) {
      int currentP = plyrs[getItem(t, p, tms, pPerT, numTms)];
      teamSum += currentP;
    }

    sum += teamSum;
    sumSq += teamSum * teamSum;
  }

  /* Calculate and return variance */
  return (sumSq - (sum * sum) / numTms) / numTms;
}

/* Result validation and visualization */
bool isValid(int *const arr, int numEl) {
  /*
   @param arr: Array to be verified
   @param numEl: Number of elements in arr

   @return isValid: Returns true if every value in arr is unique and every value
                    from 0 to numEl-1 is present in arr; false otherwise
   */

  int i, j, p;
  int *count;

  /* Allocate count */
  count = malloc(numEl * sizeof(int));

  /* Define and initialize counter array */
  for (i = 0; i < numEl; i++) {
    count[i] = 0;
  }

  /* Verify array */
  for (p = 0; p < numEl; p++) {
    int currentVal;
    currentVal = arr[p];

    /* Check if all values from 0 to numEl-1 are in arr */
    if (getPos(arr, p, numEl) == -1) {
      return false;
    }

    /* Count all values */
    for (j = 0; i < numEl; j++) {
      if (arr[j] == currentVal) {
        count[p] = count[p] + 1;
        if (count[p] > 1) {
          return false;
        }
      }
    }
  }

  free(count);
  return true;
}

void printArray(int *const arr, int l) {
  /*
   @param arr: Array to be printed
   @param l: Number of elements in arr

   @return void: Only prints elements in arr to console
   */

  int i;

  for (i = 0; i < l; i++) {
    printf("%d ", arr[i]);
  }
  printf("\n");
}

void printTeams(int *const tms, int *const plyrs, int pPerT, int numTms,
                bool withScr) {
  /*
   @param tms: Array that represents the players in teams
   @param plyrs: Array with the hcps of all players; eg: 0 1 3 5 7 9
   @param pPerT: Represents the number of players per team
   @param numTms: Represents the number of teams
   @param withScr: Determines if the index or the hcp of players
                   are printed

   @return void: Only prints the teams array with player times to console
   */

  int t, p;

  for (t = 0; t < numTms; t++) {
    printf("TEAM %d: ", t);
    for (p = 0; p < pPerT; p++) {
      if (withScr) {
        printf("%d ", plyrs[getItem(t, p, tms, pPerT, numTms)]);
      } else {
        printf("%d ", getItem(t, p, tms, pPerT, numTms));
      }
    }
    printf("\n");
  }
}

/* Crossover helpers */
int *getBetterOffsprg(int *const offsprg1, int *const offsprg2,
                      int *const plyrs, int pPerT, int numTms) {
  /*
   @param offsprg1: First offspring used for comparison
   @param offsprg2: Second offspring used for comparison
   @param plyrs: Array with the hcps of all players; eg: 0 1 3 5 7 9
   @param pPerT: Represents the number of players per team
   @param numTms: Represents the number of teams

   @return offspring: Returns the better of the two offspring
  */

  /* Determine scores of both offsprings*/
  double scr1, scr2;

  scr1 = getScore(offsprg1, plyrs, pPerT, numTms);
  scr2 = getScore(offsprg2, plyrs, pPerT, numTms);

  /* Return the better offspring */
  if (scr1 >= scr2) {
    free(offsprg2);
    return offsprg1;
  } else {
    free(offsprg1);
    return offsprg2;
  }
}
