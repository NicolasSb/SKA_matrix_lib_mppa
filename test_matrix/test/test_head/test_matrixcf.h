#ifndef TEST_MATRIXCF_H
#define TEST_MATRIXCF_H

#include "../../include/matrixcf.h"
#include "../../include/matrix_type.h"

#ifndef TIME_CFISPLAY
#define TIME_CFISPLAY
#endif

void test_scaleMatrixCF();

void test_addMatrixCF();

void test_addMatrixTCF();

void test_subMatrixCF();

void test_mulMatrixCF();

void test_mulAddScaleMatrixCF();

void test_matRefCF();

void test_scaleLineCF();

void test_subXLinesCF();

void test_isDiagCF();

void test_isTriUCF();

void test_isTriLCF();

void test_isSparseCF();

void testAllCF();

#endif //MATRIXCF_H
