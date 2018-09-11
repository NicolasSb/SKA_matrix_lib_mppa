#ifndef TEST_MATRIXD_H
#define TEST_MATRIXD_H

#include "../../include/matrixd.h"
#include "../../include/matrix_type.h"

#ifndef TIME_DISPLAY
#define TIME_DISPLAY
#endif

void test_scaleMatrixD();

void test_addMatrixD();

void test_addMatrixTD();

void test_subMatrixD();

void test_mulMatrixD();

void test_mulAddScaleMatrixD();

void test_matRefD();

void test_scaleLineD();

void test_subXLinesD();

void test_isDiagD();

void test_isTriUD();

void test_isTriLD();

void test_isSparseD();

void testAllD();


#endif //MATRIXD_H
