#ifndef TEST_MATRIXCD_H
#define TEST_MATRIXCD_H

#include "../../include/matrixcd.h"
#include "../../include/matrix_type.h"
#include <math.h>
#ifndef TIME_DISPLAY
#define TIME_DISPLAY
#endif

void test_scaleMatrixCD();

void test_addMatrixCD();

void test_addMatrixTCD();

void test_subMatrixCD();

void test_mulMatrixCD();

void test_mulAddScaleMatrixCD();

void test_matRefCD();

void test_scaleLineCD();

void test_subXLinesCD();

void test_isDiagCD();

void test_isTriUCD();

void test_isTriLCD();

void test_isSparseCD();

void testAllCD();


#endif //MATRIXCD_H
