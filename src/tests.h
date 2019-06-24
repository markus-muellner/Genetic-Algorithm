#ifndef tests_h
#define tests_h

#include "CuTest.h"
#include "crossover.h"
#include "mutate.h"
#include "selection.h"
#include "utility.h"
#include <stdio.h>
#include <stdlib.h>

int **Setup(void);
void TearDown(int **pop);
void TestFPS(CuTest *tc);
void TestTS(CuTest *tc);
void TestRBS(CuTest *tc);
void TestOX1(CuTest *tc);
void TestPMX(CuTest *tc);
void TestCX(CuTest *tc);
void TestSwap(CuTest *tc);
void TestInsert(CuTest *tc);
void TestInversion(CuTest *tc);
void TestGetScore(CuTest *tc);
void getTestDelta(CuTest *tc);
void TestMergeSort(CuTest *tc);
CuSuite *GetSuite(void);

#endif /* tests_h */
