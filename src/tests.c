#include "tests.h"
#define POP_SIZE 10

int PLYRS[32] = {5,  29, 37, 5, 18, 2,  2,  2,  0,  1,  33, 23, 34, 40, 34, 35,
                 30, 23, 37, 0, 22, 41, 22, 33, 43, 35, 44, 15, 23, 19, 4,  43};
double SCRS[POP_SIZE];

int **Setup() {
  int **pop = malloc(POP_SIZE * sizeof(int *));

  int numPlyrs = 32;
  int numTms = 8;
  int i;
  for (i = 0; i < POP_SIZE; i++) {
    pop[i] = initialize(numPlyrs, numTms, true);
  }

  swap(pop[1], 32, 100, 6, 10, true);
  swap(pop[1], 32, 100, 9, 16, true);
  swap(pop[1], 32, 100, 16, 0, true);
  swap(pop[1], 32, 100, 17, 10, true);
  swap(pop[1], 32, 100, 24, 3, true);

  for (i = 0; i < POP_SIZE; i++) {
    SCRS[i] = getScore(pop[i], PLYRS, 4, 8);
  }

  return pop;
}

void TearDown(int **pop) {
  int i;
  for (i = 0; i < POP_SIZE; i++) {
    free(pop[i]);
  }
  free(pop);
}

void TestFPS(CuTest *tc) {
  /* GIVEN: population and different random numbers */
  int **pop;
  int p1, p2, rand1, rand2, expected1, expected2;
  rand1 = (double)RAND_MAX * 0.05;
  rand2 = (double)RAND_MAX * 0.95;

  pop = Setup();

  /* WHEN: parent organisms are selected */
  fps(SCRS, POP_SIZE, &p1, &p2, rand1, rand2);

  /* THEN: they must be chosen correctly according to the random numbers */
  expected1 = 0;
  expected2 = 9;
  CuAssertIntEquals(tc, expected1, p1);
  CuAssertIntEquals(tc, expected2, p2);

  /* GIVEN: different random numbers */
  rand1 = (double)RAND_MAX * 0.1;
  rand2 = (double)RAND_MAX * 0.9;

  /* WHEN: parent organisms are selected */
  fps(SCRS, POP_SIZE, &p1, &p2, rand1, rand2);

  /* THEN: they must be chosen correctly according to the random numbers */
  expected1 = 1;
  expected2 = 9;
  CuAssertIntEquals(tc, expected1, p1);
  CuAssertIntEquals(tc, expected2, p2);

  /* GIVEN: different random numbers */
  rand1 = (double)RAND_MAX * 0.65;
  rand2 = (double)RAND_MAX * 0.75;

  /* WHEN: parent organisms are selected */
  fps(SCRS, POP_SIZE, &p1, &p2, rand1, rand2);

  /* THEN: they must be chosen correctly according to the random numbers */
  expected1 = 6;
  expected2 = 7;
  CuAssertIntEquals(tc, expected1, p1);
  CuAssertIntEquals(tc, expected2, p2);

  /* GIVEN: different random numbers */
  rand1 = (double)RAND_MAX * 0.35;
  rand2 = (double)RAND_MAX * 0.35;

  /* WHEN: parent organisms are selected */
  fps(SCRS, POP_SIZE, &p1, &p2, rand1, rand2);

  /* THEN: they must be chosen correctly according to the random numbers */
  expected1 = 3;
  expected2 = 2;
  CuAssertIntEquals(tc, expected1, p1);
  CuAssertIntEquals(tc, expected2, p2);

  TearDown(pop);
}

void TestTS(CuTest *tc) {
  /* GIVEN: population and different random numbers */
  int **pop = Setup();
  int p1, p2, rand1, rand2, expected1, expected2;
  rand1 = (double)RAND_MAX * 0.15;
  rand2 = (double)RAND_MAX * 0.90;

  /* WHEN: parent organisms are selected */
  ts(SCRS, POP_SIZE, &p1, &p2, rand1, rand2);

  /* THEN: they must be chosen correctly according to the random numbers */
  expected1 = 7;
  expected2 = 2;
  CuAssertIntEquals(tc, expected1, p1);
  CuAssertIntEquals(tc, expected2, p2);

  /* GIVEN: different random numbers */
  rand1 = (double)RAND_MAX * 0.40;
  rand2 = (double)RAND_MAX * 0.40;

  /* WHEN: parent organisms are selected */
  ts(SCRS, POP_SIZE, &p1, &p2, rand1, rand2);

  /* THEN: they must be chosen correctly according to the random numbers */
  expected1 = 8;
  expected2 = 8;
  CuAssertIntEquals(tc, expected1, p1);
  CuAssertIntEquals(tc, expected2, p2);

  /* GIVEN: different random numbers */
  rand1 = (double)RAND_MAX * 0.20;
  rand2 = (double)RAND_MAX * 0.55;

  /* WHEN: parent organisms are selected */
  ts(SCRS, POP_SIZE, &p1, &p2, rand1, rand2);

  /* THEN: they must be chosen correctly according to the random numbers */
  expected1 = 9;
  expected2 = 5;
  CuAssertIntEquals(tc, expected1, p1);
  CuAssertIntEquals(tc, expected2, p2);

  TearDown(pop);
}

void TestRBS(CuTest *tc) {
  /* GIVEN: population and different random numbers */
  int **pop;
  int p1, p2, rand1, rand2, expected1, expected2;

  pop = Setup();
  swap(pop[6], 32, 100, 0, 9, true);
  SCRS[6] = getScore(pop[6], PLYRS, 4, 8);

  swap(pop[3], 32, 100, 7, 9, true);
  SCRS[3] = getScore(pop[3], PLYRS, 4, 8);

  rand1 = (double)RAND_MAX * 0.01;
  rand2 = (double)RAND_MAX * 0.82;

  /* WHEN: parent organisms are selected */
  rbs(SCRS, POP_SIZE, &p1, &p2, rand1, rand2);

  /* THEN: they must be chosen correctly according to the random numbers */
  expected1 = 3;
  expected2 = 6;
  CuAssertIntEquals(tc, expected1, p1);
  CuAssertIntEquals(tc, expected2, p2);

  /* GIVEN: different random numbers */
  rand1 = (double)RAND_MAX * 0.20;
  rand2 = (double)RAND_MAX * 0.20;

  /* WHEN: parent organisms are selected */
  rbs(SCRS, POP_SIZE, &p1, &p2, rand1, rand2);

  /* THEN: they must be chosen correctly according to the random numbers */
  expected1 = 7;
  expected2 = 5;
  CuAssertIntEquals(tc, expected1, p1);
  CuAssertIntEquals(tc, expected2, p2);

  /* GIVEN: different random numbers */
  rand1 = (double)RAND_MAX * 0.55;
  rand2 = (double)RAND_MAX * 0.12;

  /* WHEN: parent organisms are selected */
  rbs(SCRS, POP_SIZE, &p1, &p2, rand1, rand2);

  /* THEN: they must be chosen correctly according to the random numbers */
  expected1 = 2;
  expected2 = 8;
  CuAssertIntEquals(tc, expected1, p1);
  CuAssertIntEquals(tc, expected2, p2);

  TearDown(pop);
}

void TestOX1(CuTest *tc) {
  /* GIVEN: population */
  int **pop = Setup();
  int *p1 = pop[0];
  int *p2 = pop[1];

  /* WHEN: an offspring is generated */
  int rand1 = 20;
  int rand2 = 33;
  int *offsprg = ox1(p1, p2, PLYRS, 32, 8, rand1, rand2, true);

  /* THEN: the offspring must look as expected */
  int expected[32] = {0,  1,  2,  3,  4,  5,  17, 7,  8,  26, 10,
                      11, 12, 13, 14, 15, 6,  28, 18, 19, 20, 21,
                      22, 23, 16, 25, 9,  27, 24, 29, 30, 31};

  int i;
  for (i = 0; i < 32; i++) {
    CuAssertIntEquals(tc, expected[i], offsprg[i]);
  }

  free(offsprg);
  TearDown(pop);
}

void TestPMX(CuTest *tc) {
  /* GIVEN: population */
  int **pop = Setup();
  int *p1 = pop[0];
  int *p2 = pop[1];

  /* WHEN: an offspring is generated */
  int rand1 = 15;
  int rand2 = 5;
  int *offsprg = pmx(p1, p2, PLYRS, 32, 8, rand1, rand2, true);

  /* THEN: the offspring must look as expected */
  int expected[32] = {0,  1,  2,  3,  4,  5,  28, 7,  8,  26, 10,
                      11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21,
                      22, 23, 6,  25, 9,  27, 24, 29, 30, 31};

  int i;
  for (i = 0; i < 32; i++) {
    CuAssertIntEquals(tc, expected[i], offsprg[i]);
  }

  free(offsprg);
  TearDown(pop);
}

void TestCX(CuTest *tc) {
  /* GIVEN: population */
  int **pop = Setup();
  int *p1 = pop[0];
  int *p2 = pop[1];

  /* WHEN: an offspring is generated */
  int rand1 = 20;
  int rand2 = 33;
  int *offsprg = cx(p1, p2, PLYRS, 32, 8, rand1, rand2, true);

  /* THEN: the offspring must look as expected */
  int expected[32] = {0,  1,  2,  3,  4,  5,  17, 7,  8,  9,  10,
                      11, 12, 13, 14, 15, 6,  28, 18, 19, 20, 21,
                      22, 23, 16, 25, 26, 27, 24, 29, 30, 31};

  int i;
  for (i = 0; i < 32; i++) {
    CuAssertIntEquals(tc, expected[i], offsprg[i]);
  }

  free(offsprg);
  TearDown(pop);
}

void TestSwap(CuTest *tc) {
  /* GIVEN: population */
  int **pop;
  int i, j;
  int changedIdxs[2];
  int expected[2];

  pop = Setup();

  /* WHEN: swap mutation is applied with two numbers */
  swap(pop[0], 32, 100, 10, 16, true);

  j = 0;
  for (i = 0; i < 32; i++) {
    int p = pop[0][i];
    if (PLYRS[i] != PLYRS[p]) {
      changedIdxs[j++] = i;
    }
  }

  /* THEN: the corresponding players must be swapped */
  expected[0] = 10;
  expected[1] = 27;
  for (i = 0; i < 2; i++) {
    CuAssertIntEquals(tc, expected[i], changedIdxs[i]);
  }

  CuAssertIntEquals(tc, PLYRS[changedIdxs[0]], PLYRS[pop[0][changedIdxs[1]]]);
  CuAssertIntEquals(tc, PLYRS[changedIdxs[1]], PLYRS[pop[0][changedIdxs[0]]]);

  TearDown(pop);
}

void TestInsert(CuTest *tc) {
  /* GIVEN: population */
  int **pop;
  int i;
  int expected1[32] = {0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  11,
                       12, 13, 14, 15, 10, 16, 17, 18, 19, 20, 21,
                       22, 23, 24, 25, 26, 27, 28, 29, 30, 31};
  int expected2[32] = {1,  2,  0,  3,  4,  5,  6,  7,  8,  9,  10,
                       11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21,
                       22, 23, 24, 25, 26, 27, 28, 29, 30, 31};
  pop = Setup();

  /* WHEN: insert mutation is applied */
  insert(pop[0], 32, 100, 10, 4, true);

  /* THEN: the player at the corresponding index must have been inserted
     correctly */

  for (i = 0; i < 32; i++) {
    CuAssertIntEquals(tc, expected1[i], pop[0][i]);
  }

  /* WHEN: insert mutation is applied */
  insert(pop[2], 32, 100, 0, 32, true);

  /* THEN: the player at the corresponding index must have been inserted
    correctly */

  for (i = 0; i < 32; i++) {
    CuAssertIntEquals(tc, expected2[i], pop[2][i]);
  }

  TearDown(pop);
}

void TestInversion(CuTest *tc) {
  /* GIVEN: population */
  int **pop;
  int i;
  int expected1[32] = {0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  14,
                       13, 12, 11, 10, 15, 16, 17, 18, 19, 20, 21,
                       22, 23, 24, 25, 26, 27, 28, 29, 30, 31};
  int expected2[32] = {0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  15,
                       14, 13, 12, 11, 10, 16, 17, 18, 19, 20, 21,
                       22, 23, 24, 25, 26, 27, 28, 29, 30, 31};
  pop = Setup();

  /* WHEN: inversion mutation is applied */
  inversion(pop[0], 32, 100, 10, 4, true);

  /* THEN: the players' order in the selected range must have been inverted */
  for (i = 0; i < 32; i++) {
    CuAssertIntEquals(tc, expected1[i], pop[0][i]);
  }

  /* WHEN: inversion mutation is applied */
  inversion(pop[2], 32, 100, 10, 5, true);

  /* THEN: the players' order in the selected range must have been inverted */
  for (i = 0; i < 32; i++) {
    CuAssertIntEquals(tc, expected2[i], pop[2][i]);
  }

  TearDown(pop);
}

void TestGetScore(CuTest *tc) {
  /* GIVEN: population */
  int **pop = Setup();

  /* WHEN: an organism's score is retireved */
  double score = SCRS[0];

  /* THEN: it must be equivalent to the expected value */
  double expected = 1427.43750;
  CuAssertDblEquals(tc, expected, score, 0.000001);

  /* WHEN: an organism's score is retireved */
  score = SCRS[1];

  /* THEN: the score must be equivalent to the expected value */
  expected = 889.4375;
  CuAssertDblEquals(tc, expected, score, 0.000001);

  TearDown(pop);
}

void getTestDelta(CuTest *tc) {
  /* GIVEN: population */
  int **pop = Setup();

  /* WHEN: an organism's delta is retrieved */
  int delta = getDelta(pop[0], PLYRS, 4, 8);

  /* THEN: the delta must be equivalent to the expected value */
  int expected = 119;
  CuAssertIntEquals(tc, expected, delta);

  TearDown(pop);
}

void TestMergeSort(CuTest *tc) {
  /* GIVEN: array to sort and population */
  int **pop;
  int i;
  int order_mask[POP_SIZE] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
  int expected1[POP_SIZE] = {1, 9, 8, 7, 6, 5, 4, 3, 2, 0};
  int expected2[POP_SIZE] = {1, 0, 2, 3, 4, 6, 7, 8, 9, 5};

  pop = Setup();

  /* WHEN: the array is sorted */
  mergeSort(order_mask, SCRS, POP_SIZE);

  /* THEN: the values must have following order */
  for (i = 0; i < POP_SIZE; i++) {
    CuAssertIntEquals(tc, expected1[i], order_mask[i]);
  }

  /* GIVEN: different population */
  swap(pop[5], 32, 100, 0, 16, true);
  SCRS[5] = getScore(pop[5], PLYRS, 4, 8);

  /* WHEN: the array is sorted */
  mergeSort(order_mask, SCRS, POP_SIZE);

  /* THEN: the values must have following order */
  for (i = 0; i < POP_SIZE; i++) {
    CuAssertIntEquals(tc, expected2[i], order_mask[i]);
  }

  TearDown(pop);
}

void TestGetBounds(CuTest *tc) {
  /* GIVEN: two random numbers */
  int upper, lower;
  int rand1, rand2;

  rand1 = 3945;
  rand2 = 8459;

  /* WHEN: random bounds are selected */
  getBounds(32, &lower, &upper, rand1, rand2);

  /* THEN: they must have the following values */
  CuAssertIntEquals(tc, 9, lower);
  CuAssertIntEquals(tc, 5, upper);

  /* GIVEN: two random numbers */
  rand1 = 204;
  rand2 = 204;

  /* WHEN: random bounds are selected */
  getIndexes(32, &rand1, &rand2);

  /* THEN: they must have the following values */
  CuAssertIntEquals(tc, 12, rand1);
  CuAssertIntEquals(tc, 31, rand2);

  /* GIVEN: two random numbers */
  rand1 = 94735;
  rand2 = 2;

  /* WHEN: random bounds are selected */
  getIndexes(32, &rand1, &rand2);

  /* THEN: they must have the following values */
  CuAssertIntEquals(tc, 15, rand1);
  CuAssertIntEquals(tc, 18, rand2);
}

void TestGetIndexes(CuTest *tc) {
  /* GIVEN: two random numbers */
  int rand1, rand2;

  rand1 = 3945;
  rand2 = 8459;

  /* WHEN: random bounds are selected */
  getIndexes(32, &rand1, &rand2);

  /* THEN: they must have the following values */
  CuAssertIntEquals(tc, 5, rand1);
  CuAssertIntEquals(tc, 9, rand2);

  /* GIVEN: two random numbers */
  rand1 = 204;
  rand2 = 204;

  /* WHEN: random bounds are selected */
  getIndexes(32, &rand1, &rand2);

  /* THEN: they must have the following values */
  CuAssertIntEquals(tc, 12, rand1);
  CuAssertIntEquals(tc, 31, rand2);

  /* GIVEN: two random numbers */
  rand1 = 94735;
  rand2 = 2;

  /* WHEN: random bounds are selected */
  getIndexes(32, &rand1, &rand2);

  /* THEN: they must have the following values */
  CuAssertIntEquals(tc, 15, rand1);
  CuAssertIntEquals(tc, 18, rand2);
}

CuSuite *GetSuite() {
  CuSuite *suite = CuSuiteNew();
  SUITE_ADD_TEST(suite, TestFPS);
  SUITE_ADD_TEST(suite, TestTS);
  SUITE_ADD_TEST(suite, TestRBS);
  SUITE_ADD_TEST(suite, TestOX1);
  SUITE_ADD_TEST(suite, TestPMX);
  SUITE_ADD_TEST(suite, TestCX);
  SUITE_ADD_TEST(suite, TestSwap);
  SUITE_ADD_TEST(suite, TestInsert);
  SUITE_ADD_TEST(suite, TestInversion);
  SUITE_ADD_TEST(suite, TestGetScore);
  SUITE_ADD_TEST(suite, getTestDelta);
  SUITE_ADD_TEST(suite, TestMergeSort);
  SUITE_ADD_TEST(suite, TestGetBounds);
  SUITE_ADD_TEST(suite, TestGetIndexes);

  return suite;
}
