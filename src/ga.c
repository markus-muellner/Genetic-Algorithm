#include "ga.h"
#include <string.h>

/* Constants that determine general algorithm performance */
#define EXIT_SCORE 3
#define TERMINATE_AT 75000
#define POPULATION_SIZE 100
#define NUM_THREADS 8
#define NUM_CONFIGS 5
#define MIGRATION_INTERVAL 1000

/* Supported operators */
/*
 OX1: Order 1 Crossover
 PMX: Partially Matched Crossover
 CX:  Cycle Crossover

 SWP: Swap Mutation
 INS: Insert Mutation
 INV: inversion Mutation

 FPS: Fitness Proportionate Selection
 TS: Tournament Selection
 RBS: Rank Based Selection
 */
static const char *CROSSOVER[] = {
    "PMX",
    "OX1",
    "CX",
};

static const char *MUTATION[] = {
    "SWP",
    "INS",
    "INV",
};

static const char *SELECTION[] = {
    "FPS",
    "TS",
    "RBS",
};

/* Switch interim solution verification on or off */
bool const VERIFY = false;

/* Pointers to operators */
int *(*_cross)(int *const, int *const, int *const, int, int, int, int, bool);
void (*_mutate)(int *, int, int, int, int, bool);
void (*_select)(double *const, int, int *, int *, int, int);

/* Shared memory, mutexes and barriers */
int **MIGRATION;
bool FINISHED, CAN_EXIT;
pthread_mutex_t CA_MUTEX;
int W_COUNT = 0, R_COUNT = 0;
pthread_mutex_t M_MUTEX;

pthread_barrier_t ALL_WRITTEN_B;
pthread_barrier_t NEXT_MIGRATION_B;

struct arguments_struct {
  /*
   @param numPlyrs: Represents the number of elements in plyrs
   @param plyrs: Array with the hcps of all players; eg: 0 1 3 5 7 9
   @param numTms: Number of teams to be built by the algorithm
   @param c: Crossover operator
   @param m: Mutation operator
   @param s: Selection operator
   @param variance: Pointer to double for returning the variance
                    of the final solution
   @param result: Pointer to an int array for returning the final
                  result
   @param mutate_prop: Mutation probability (ranging from 0 to 100)
   @param idx: Index of the thread
   */

  int numPlyrs;
  int *plyrs;
  int numTms;
  char *cross;
  char *mutate;
  char *select;
  double *variance;
  int *result;
  int mutate_prop;
  int idx;
};

int *run(int numPlyrs, int *plyrs, int numTms, double *variance) {
  /*
   @param numPlyrs: Represents the number of elements in plyrs
   @param plyrs: Array with the hcps of all players; eg: 0 1 3 5 7 9
   @param numTms: Number of teams to be built by the algorithm
   @param variance: Pointer to double for returning the variance
                    of the final solution

   @return result: Array representing players in teams eg: 0 1 5 2 3 4
                   Delimiters for tms can be derived from numPlyrs and
                   numTms.
   */

  /* Declare local variables */
  int delta, best, i;

  /* Threads and thread argument declaration */
  struct arguments_struct args[NUM_THREADS];
  pthread_t threads[NUM_THREADS];
  double variances[NUM_THREADS];
  int *results[NUM_THREADS];

  /* Algorithm configuration */
  char *cs[NUM_CONFIGS] = {"OX1", "PMX", "PMX", "OX1", "PMX"};
  char *ms[NUM_CONFIGS] = {"SWP", "INV", "SWP", "INV", "SWP"};
  char *ss[NUM_CONFIGS] = {"TS", "TS", "RBS", "TS", "RBS"};
  int mutate_props[NUM_CONFIGS] = {20, 20, 20, 100, 20};
  FINISHED = false;
  CAN_EXIT = false;

  /* Initialize migrations barrier */
  pthread_barrier_init(&NEXT_MIGRATION_B, NULL, NUM_THREADS);

  /* Allocate shared memory for migrations */
  MIGRATION = malloc(NUM_THREADS * sizeof(int *));
  if (numPlyrs < 4 || numPlyrs > 300 || numPlyrs % numTms != 0) {
    printf("Argument constraint(s) violated.\n");
    exit(1);
  }

  /* Initialize random generator */
  srand((unsigned)time(NULL));

  /* Start algorithms as threads */
  for (i = 0; i < NUM_THREADS; i++) {
    MIGRATION[i] = malloc(numPlyrs * sizeof(int));

    variances[i] = 0;
    results[i] = malloc((numPlyrs + 2) * sizeof(int));
    args[i].numPlyrs = numPlyrs;
    args[i].plyrs = plyrs;
    args[i].numTms = numTms;
    args[i].cross = cs[i % NUM_CONFIGS];
    args[i].mutate = ms[i % NUM_CONFIGS];
    args[i].select = ss[i % NUM_CONFIGS];
    args[i].variance = &variances[i];
    args[i].result = results[i];
    args[i].mutate_prop = mutate_props[i % NUM_CONFIGS];
    args[i].idx = i;

    if (pthread_create(&threads[i], NULL, &optimize, (void *)&args[i]) != 0) {
      printf("Fatal error.\n");
      exit(1);
    }
  }

  /* Find best solution */
  delta = INT_MAX;
  best = 0;
  for (i = 0; i < NUM_THREADS; i++) {
    if (pthread_join(threads[i], NULL) == 0) {
      if (results[i][numPlyrs] < delta) {
        delta = results[i][numPlyrs];
        *variance = variances[i];
        best = i;
      }
    } else {
      printf("Unexpected error while joining thread %d.", i);
      exit(1);
    }
  }

  /* De-allocate memory */
  for (i = 0; i < NUM_THREADS; i++) {
    free(MIGRATION[i]);
    if (i != best) {
      free(results[i]);
    }
  }
  free(MIGRATION);

  return results[best];
}

void *optimize(void *arguments) {
  /*
   @param: arguments: Pointer to an instance of arguments_struct

   @return void: Result array is modified directly
   */

  /* Declare local variables */
  int **pop;
  double scrs[POPULATION_SIZE];
  int pPerT, i, it, best, delta;

  /* Copy argumrnts */
  struct arguments_struct *args = arguments;
  int numPlyrs = args->numPlyrs;
  int *plyrs = args->plyrs;
  int numTms = args->numTms;
  char *c = args->cross;
  char *m = args->mutate;
  char *s = args->select;
  double *variance = args->variance;
  int *result = args->result;
  int mutate_prop = args->mutate_prop;
  int idx = args->idx;

  /* Alias crossover function */
  if (strcmp(CROSSOVER[0], c) == 0) {
    _cross = &pmx;
  } else if (strcmp(CROSSOVER[1], c) == 0) {
    _cross = &ox1;
  } else if (strcmp(CROSSOVER[2], c) == 0) {
    _cross = &cx;
  } else {
    printf("No such crossover operator %s.", c);
    exit(1);
  }

  /* Alias mutate function */
  if (strcmp(MUTATION[0], m) == 0) {
    _mutate = &swap;
  } else if (strcmp(MUTATION[1], m) == 0) {
    _mutate = &insert;
  } else if (strcmp(MUTATION[2], m) == 0) {
    _mutate = &inversion;
  } else {
    printf("No such mutation operator %s.", m);
    exit(1);
  }

  /* Alias select function */
  if (strcmp(SELECTION[0], s) == 0) {
    _select = &fps;
  } else if (strcmp(SELECTION[1], s) == 0) {
    _select = &ts;
  } else if (strcmp(SELECTION[2], s) == 0) {
    _select = &rbs;
  } else {
    printf("No such selection operator %s.", s);
    exit(1);
  }

  /* Calculate players per team */
  pPerT = numPlyrs / numTms;

  /* Initialize population and scores */
  pop = malloc(POPULATION_SIZE * sizeof(int *));

  /* Randomize initial population and determine scores */
  for (i = 0; i < POPULATION_SIZE; i++) {
    pop[i] = initialize(numPlyrs, numTms, true);
    if (VERIFY) {
      assert(isValid(pop[i], numPlyrs));
    }
    randomize(numPlyrs, pop[i], numTms);
    scrs[i] = getScore(pop[i], plyrs, pPerT, numTms);
  }

  /* Perform iterations and save best organism */
  best = iterate(scrs, pop, plyrs, numPlyrs, numTms, &it, variance, mutate_prop,
                 idx);
  assert(isValid(pop[best], numPlyrs));

  /* Prepare result array with last two indexes being meta information */
  for (i = 0; i < numPlyrs; i++) {
    result[i] = pop[best][i];
  }

  /* Get delta and append to result */
  delta = getDelta(pop[best], plyrs, pPerT, numTms);
  result[numPlyrs] = delta;

  /* Get number of iterations and append to result */
  result[++numPlyrs] = it;

  /* De-alllocate population */
  for (i = 0; i < POPULATION_SIZE; i++) {
    free(pop[i]);
  }
  free(pop);
  return NULL;
}

int iterate(double *scrs, int **pop, int *const plyrs, int numPlyrs, int numTms,
            int *it, double *variance, int mutate_prob, int idx) {
  /*
   @param scrs: Array with the score of each organism in the population
   @param pop: Matrix representing all teams
   @param plyrs: Array with the hcps of all players; eg: 0 1 3 5 7 9
   @param numPlyrs: Represents the number of elements in plyrs
   @param numTms: Number of teams to be built by the algorithm
   @param it: Pointer to int for returning the number of iterations
              that were performed
   @param variance: Pointer to double for returning the variance
                    of the final solution
   @param mutate_prop: Mutation probability
   @param idx: Index of the thread


   @return best: Index of the determined best organism in regard to pop
   */

  /* Declare local variables */
  int pPerT, bestIdx, wIdx, delta, i, j, k;
  int p1Idx, p2Idx, rand1, rand2, eliteDelta;
  int *offsprg, *elite;
  double offsprgScore, eliteScore;

  /* Initialize index of worst organism */
  wIdx = -1;

  /* Calculate players per team */
  pPerT = numPlyrs / numTms;

  /* Store index of best and its delta */
  bestIdx = determineBest(scrs, POPULATION_SIZE);
  delta = getDelta(pop[bestIdx], plyrs, pPerT, numTms);

  /* Initialize elite */
  elite = malloc(numPlyrs * sizeof(int));
  eliteDelta = delta;
  eliteScore = scrs[bestIdx];
  for (i = 0; i < numPlyrs; i++) {
    elite[i] = pop[bestIdx][i];
  }

  /* Perform iterations */
  i = 0;
  *it = 0;
  while (!FINISHED && *it < TERMINATE_AT) {
    /* Determine index of worst organism */
    wIdx = determineWorst(scrs, POPULATION_SIZE);

    /* Migration */
    if (i > MIGRATION_INTERVAL) {
      /* Wait for all threads to enter the migration */
      pthread_barrier_wait(&NEXT_MIGRATION_B);

      /* Check if next migration has already started */
      pthread_mutex_lock(&M_MUTEX);
      if (W_COUNT == 0) {
        pthread_barrier_init(&ALL_WRITTEN_B, NULL, NUM_THREADS);
      }

      /* Write organism */
      W_COUNT++;
      bestIdx = determineBest(scrs, POPULATION_SIZE);

      for (k = 0; k < numPlyrs; k++) {
        MIGRATION[idx][k] = pop[bestIdx][k];
      }
      pthread_mutex_unlock(&M_MUTEX);

      /* Wait for all threads to write */
      pthread_barrier_wait(&ALL_WRITTEN_B);

      /* Read organisms */
      pthread_mutex_lock(&M_MUTEX);
      for (j = 0; j < NUM_THREADS; j++) {

        if (j != idx) {

          double score;
          score = getScore(MIGRATION[j], plyrs, pPerT, numTms);

          assert(isValid(MIGRATION[j], numPlyrs));
          wIdx = determineWorst(scrs, POPULATION_SIZE);

          if (score < scrs[wIdx]) {

            /* Copy organism */
            for (k = 0; k < numPlyrs; k++) {
              pop[wIdx][k] = MIGRATION[j][k];
            }
            scrs[wIdx] = score;
          }
        }
      }

      /* Reset state for next migration */
      if (W_COUNT == NUM_THREADS) {

        /* If exit condition is fulfilled => stop iterations */
        if (CAN_EXIT) {
          FINISHED = true;
        }

        pthread_barrier_init(&NEXT_MIGRATION_B, NULL, NUM_THREADS);
        W_COUNT = 0;
      }

      i = 0;
      pthread_mutex_unlock(&M_MUTEX);
    }

    /* Selection */
    rand1 = rand();
    rand2 = rand();
    _select(scrs, POPULATION_SIZE, &p1Idx, &p2Idx, rand1, rand2);
    assert(p1Idx >= 0 && p1Idx < POPULATION_SIZE);
    assert(p2Idx >= 0 && p2Idx < POPULATION_SIZE);

    /* Crossover */
    rand1 = rand();
    rand2 = rand();
    offsprg = _cross(pop[p1Idx], pop[p2Idx], plyrs, numPlyrs, numTms, rand1,
                     rand2, VERIFY);

    /* Mutation */
    rand1 = rand();
    rand2 = rand();
    _mutate(offsprg, numPlyrs, mutate_prob, rand1, rand2, VERIFY);

    /* Calculate score of offsprg */
    offsprgScore = getScore(offsprg, plyrs, pPerT, numTms);

    /* Replace worst with offsprg if offsprg is better */
    wIdx = determineWorst(scrs, POPULATION_SIZE);
    if (offsprgScore < scrs[wIdx]) {
      for (j = 0; j < numPlyrs; j++) {
        pop[wIdx][j] = offsprg[j];
      }

      /* Set score of offsprg */
      scrs[wIdx] = offsprgScore;

      /* Set best index if best score has improved */
      if (offsprgScore < scrs[bestIdx]) {
        bestIdx = wIdx;
        delta = getDelta(pop[bestIdx], plyrs, pPerT, numTms);
      }
    }
    free(offsprg);

    /* Check exit score */
    pthread_mutex_lock(&CA_MUTEX);
    if (delta <= EXIT_SCORE && !CAN_EXIT) {
      CAN_EXIT = true;
    }
    pthread_mutex_unlock(&CA_MUTEX);

    /* Elitism */
    if (delta < eliteDelta) {
      eliteDelta = delta;
      eliteScore = scrs[bestIdx];
      for (j = 0; j < numPlyrs; j++) {
        elite[j] = pop[bestIdx][j];
      }
    }

    i++;
    *it = *it + 1;
  }

  /* Put elite back into population */
  if (wIdx == -1) {
    printf("Fatal error: wIdx uninitialized. This must never happen.\n");
    exit(1);
  }
  scrs[wIdx] = eliteScore;
  *variance = eliteScore;
  for (i = 0; i < numPlyrs; i++) {
    pop[wIdx][i] = elite[i];
  }
  free(elite);

  return wIdx;
}
