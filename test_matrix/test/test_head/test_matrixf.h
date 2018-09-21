#ifndef TEST_MATRIXF_H
#define TEST_MATRIXF_H

#include "../../include/matrixf.h"
#include "../../include/matrix_type.h"

#ifndef TIME_DISPLAY
#define TIME_DISPLAY
#endif


void test_scaleMatrixF();

void test_addMatrixF();

void test_addMatrixTF();

void test_subMatrixF();

void test_scaleSubMatrixF();

void test_mulMatrixF();

void test_mulAddScaleMatrixF();

void test_matRefF();

void test_scaleLineF();

void test_subXLinesF();

void test_isDiagF();

void test_isTriUF();

void test_isTriLF();

void test_isSparseF();

void testAllF();

#endif //MATRIXF_H
