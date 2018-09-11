#ifndef TEST_MATRIXI_H
#define TEST_MATRIXI_H

#include "../../include/matrixi.h"
#include "../../include/matrix_type.h"

#ifndef TIME_IISPLAY
#define TIME_IISPLAY
#endif

void test_scaleMatrixI();

void test_addMatrixI();

void test_addMatrixTI();

void test_subMatrixI();

void test_mulMatrixI();

void test_mulAddScaleMatrixI();

void test_matRefI();

void test_scaleLineI();

void test_subXLinesI();

void test_isDiagI();

void test_isTriUI();

void test_isTriLI();

void test_isSparseI();

void testAllI();

#endif //MATRIXI_H
