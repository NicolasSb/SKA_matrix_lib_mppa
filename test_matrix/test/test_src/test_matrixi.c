#include "../test_head/test_matrixi.h"
#include <omp.h>


#include <mOS_common_types_c.h>
#include <mOS_vcore_u.h>
#include <mOS_segment_manager_u.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/unistd.h>
#include <sys/_default_fcntl.h>
#include <vbsp.h>
#include <utask.h>
#include <HAL/hal/cluster/dnoc.h>
#include <HAL/hal/hal_ext.h>
#include <math.h>
#include <stdlib.h>
#include <mppa_power.h>
#include <mppa_async.h>
#include <mppa_remote.h>
#include <vbsp.h>
#include <string.h>
#include <assert.h>
#include <pthread.h>
#include <utask.h>

#define CHIP_FREQ (__bsp_frequency/1000)

#define NB_TESTI 13



static void printSuccess(const char * func)
{
	printf("\033[32m%s succeed \033[39m \n", func);
}

static void printFailure(const char * func)
{
	printf("\033[31m%s failed \033[39m \n", func);
}

static void test_result(Matrix_i* mat, int *a, const char * func)
{
	unsigned int i;
	for (i =0 ; i< 9; i++)
	{	
		if(mat->data[i] != a[i])
		{
			printFailure(func);
			return;		
		}
	}	
	printSuccess(func);
}

void testAllI()
{
	printf("***\tTesting integer matrix Functions\t***\n\n");
	test_scaleMatrixI();
	test_addMatrixI();
	test_addMatrixTI();
	test_subMatrixI();
	test_mulMatrixI();
	test_mulAddScaleMatrixI();
	test_matRefI();
	test_scaleLineI();
	test_subXLinesI();
	test_isDiagI();
	test_isTriUI();
	test_isTriLI();
	test_isSparseI();
}


void test_scaleMatrixI()
{
	Matrix_i *a = matrixAllocI(3,3);
	createIdentityI(a);

	scaleMatrixI(a, 2);
	int tab[9] = {2,0,0,0,2,0,0,0,2};
	test_result(a, tab, __FUNCTION__);
	

	freeMatrixI(a);

	#if TIME_DISPLAY == 1
	unsigned int i;	
	a = createRandomMatrixI(100,100);

	uint64_t start = __k1_read_dsu_timestamp();
	omp_set_num_threads(1);
	scaleMatrixI(a, 2);
	for(i = 0; i< 99; i++)
		scaleMatrixI(a, 1);

	uint64_t end = __k1_read_dsu_timestamp();


	float t1 = (float) ((end - start)/(float)CHIP_FREQ);
	printf("\ttime using 1 thread : %.3f ms\n", t1/100);
	
	start = __k1_read_dsu_timestamp();
	
	omp_set_num_threads(16);
	scaleMatrixI(a, 2);
	for(i = 0; i< 99; i++)
		scaleMatrixI(a, 1);
	
	end = __k1_read_dsu_timestamp();
	
	float t2 = (float) ((end - start)/(float)CHIP_FREQ);
	printf("\ttime using 16 threads : %.3f ms\n", t2/100);
	printf("\tspeedup is : %.3f\n", t1/t2);

	freeMatrixI(a);
	#endif
}

void test_addMatrixI()
{
	Matrix_i *a = matrixAllocI(3,3);
	createIdentityI(a);
	Matrix_i *b = matrixAllocI(3,3);
	createIdentityI(b);


	
	addMatrixI(a, b);

	int tab[9] = {2,0,0,0,2,0,0,0,2};
	test_result(a, tab, __FUNCTION__);

	freeMatrixI(a);
	freeMatrixI(b);

	#if TIME_DISPLAY == 1
	unsigned int i;	

	a = createRandomMatrixI(100,100);
	b = createRandomMatrixI(100,100);

	uint64_t start = __k1_read_dsu_timestamp();
	omp_set_num_threads(1);
	addMatrixI(a, b);
	for(i = 0; i< 99; i++)
		addMatrixI(b, b);

	uint64_t end = __k1_read_dsu_timestamp();

	float t1 = (float) ((end - start)/(float)CHIP_FREQ);
	printf("\ttime using 1 thread : %.3f ms\n", t1/100);
	
	start = __k1_read_dsu_timestamp();
	
	omp_set_num_threads(16);
	addMatrixI(a, b);
	for(i = 0; i< 99; i++)
		addMatrixI(b, b);
	
	end = __k1_read_dsu_timestamp();
	
	float t2 = (float) ((end - start)/(float)CHIP_FREQ);
	printf("\ttime using 16 threads : %.3f ms\n", t2/100);
	printf("\tspeedup is : %.3f\n", t1/t2);

	freeMatrixI(a);
	freeMatrixI(b);
	#endif
}

void test_addMatrixTI()
{
	Matrix_i *a = matrixAllocI(3,3);
	createIdentityI(a);
	Matrix_i *b = matrixAllocI(3,3);
	createIdentityI(b);


	
	addMatrixTI(a, b);

	int tab[9] = {2,0,0,0,2,0,0,0,2};
	test_result(a, tab, __FUNCTION__);

	freeMatrixI(a);
	freeMatrixI(b);

	#if TIME_DISPLAY == 1
	unsigned int i;	

	a = createRandomMatrixI(100,100);
	b = createRandomMatrixI(100,100);

	uint64_t start = __k1_read_dsu_timestamp();
	omp_set_num_threads(1);
	addMatrixTI(a, b);
	for(i = 0; i< 99; i++)
		addMatrixTI(b, b);

	uint64_t end = __k1_read_dsu_timestamp();

	float t1 = (float) ((end - start)/(float)CHIP_FREQ);
	printf("\ttime using 1 thread : %.3f ms\n", t1/100);
	
	start = __k1_read_dsu_timestamp();
	
	omp_set_num_threads(16);
	addMatrixTI(a, b);
	for(i = 0; i< 99; i++)
		addMatrixTI(b, b);
	
	end = __k1_read_dsu_timestamp();
	
	float t2 = (float) ((end - start)/(float)CHIP_FREQ);
	printf("\ttime using 16 threads : %.3f ms\n", t2/100);
	printf("\tspeedup is : %.3f\n", t1/t2);

	freeMatrixI(a);
	freeMatrixI(b);
	#endif
}

void test_subMatrixI()
{
	Matrix_i *a = matrixAllocI(3,3);
	createIdentityI(a);
	Matrix_i *b = matrixAllocI(3,3);
	createIdentityI(b);


	
	subMatrixI(a, b);

	int tab[9] = {0};
	test_result(a, tab, __FUNCTION__);

	freeMatrixI(a);
	freeMatrixI(b);

	#if TIME_DISPLAY == 1
	unsigned int i;	

	a = createRandomMatrixI(100,100);
	b = createRandomMatrixI(100,100);

	uint64_t start = __k1_read_dsu_timestamp();
	omp_set_num_threads(1);
	subMatrixI(a, b);
	for(i = 0; i< 99; i++)
		subMatrixI(b, b);

	uint64_t end = __k1_read_dsu_timestamp();


	float t1 = (float) ((end - start)/(float)CHIP_FREQ);
	printf("\ttime using 1 thread : %.3f ms\n", t1/100);
	
	start = __k1_read_dsu_timestamp();
	
	omp_set_num_threads(16);
	subMatrixI(a, b);
	for(i = 0; i< 99; i++)
		subMatrixI(b, b);
	
	end = __k1_read_dsu_timestamp();
	
	float t2 = (float) ((end - start)/(float)CHIP_FREQ);
	printf("\ttime using 16 threads : %.3f ms\n", t2/100);
	printf("\tspeedup is : %.3f\n", t1/t2);

	freeMatrixI(a);
	freeMatrixI(b);
	#endif
}

void test_mulMatrixI()
{
	Matrix_i *a = matrixAllocI(3,3);
	createIdentityI(a);
	Matrix_i *b = matrixAllocI(3,3);
	createIdentityI(b);


	
	mulMatrixI(a, a, b);

	int tab[9] = {1,0,0,0,1,0,0,0,1};
	test_result(a, tab, __FUNCTION__);

	freeMatrixI(a);
	freeMatrixI(b);

	#if TIME_DISPLAY == 1
	unsigned int i;	

	a = createRandomMatrixI(100,100);
	b = createRandomMatrixI(100,100);

	uint64_t start = __k1_read_dsu_timestamp();
	omp_set_num_threads(1);
	mulMatrixI(a, a, b);
	for(i = 0; i< 19; i++)
		mulMatrixI(a, b, b);

	uint64_t end = __k1_read_dsu_timestamp();


	float t1 = (float) ((end - start)/(float)CHIP_FREQ);
	printf("\ttime using 1 thread : %.3f ms\n", t1/20);
	
	start = __k1_read_dsu_timestamp();
	
	omp_set_num_threads(16);
	mulMatrixI(a, a, b);
	for(i = 0; i< 19; i++)
		mulMatrixI(a, b, b);
	
	end = __k1_read_dsu_timestamp();
	
	float t2 = (float) ((end - start)/(float)CHIP_FREQ);
	printf("\ttime using 16 threads : %.3f ms\n", t2/20);
	printf("\tspeedup is : %.3f\n", t1/t2);

	freeMatrixI(a);
	freeMatrixI(b);
	#endif
}

void test_mulAddScaleMatrixI()
{
	Matrix_i *a = matrixAllocI(3,3);
	createIdentityI(a);
	Matrix_i *b = matrixAllocI(3,3);
	createIdentityI(b);
	Matrix_i *c = matrixAllocI(3,3);
	createIdentityI(c);


	
	mulAddScaleMatrixI(a,a,b,c,2,2);

	int tab[9] = {6,0,0,0,6,0,0,0,6};
	test_result(a, tab, __FUNCTION__);

	freeMatrixI(a);
	freeMatrixI(b);
	freeMatrixI(c);

	#if TIME_DISPLAY == 1
	unsigned int i;	

	a = createRandomMatrixI(100,100);
	b = createRandomMatrixI(100,100);
	c = createRandomMatrixI(100,100);

	uint64_t start = __k1_read_dsu_timestamp();
	omp_set_num_threads(1);
	mulAddScaleMatrixI(a,a,b,c,2,2);
	for(i = 0; i< 19; i++)
		mulAddScaleMatrixI(a,a,b,c,2,2);

	uint64_t end = __k1_read_dsu_timestamp();


	float t1 = (float) ((end - start)/(float)CHIP_FREQ);
	printf("\ttime using 1 thread : %.3f ms\n", t1/20);
	
	start = __k1_read_dsu_timestamp();
	
	omp_set_num_threads(16);
	mulAddScaleMatrixI(a,a,b,c,2,2);
	for(i = 0; i< 19; i++)
		mulAddScaleMatrixI(a,a,b,c,2,2);
	
	end = __k1_read_dsu_timestamp();
	
	float t2 = (float) ((end - start)/(float)CHIP_FREQ);
	printf("\ttime using 16 threads : %.3f ms\n", t2/20);
	printf("\tspeedup is : %.3f\n", t1/t2);

	freeMatrixI(a);
	freeMatrixI(b);
	freeMatrixI(c);
	#endif
}

void test_matRefI()
{
	Matrix_i *a = matrixAllocI(3,3);
	createIdentityI(a);


	
	matRefI(a);

	int tab[9] = {1,0,0,0,1,0,0,0,1};
	test_result(a, tab, __FUNCTION__);

	freeMatrixI(a);

	#if TIME_DISPLAY == 1
	unsigned int i;	

	a = createRandomMatrixI(100,100);

	uint64_t start = __k1_read_dsu_timestamp();
	omp_set_num_threads(1);
	matRefI(a);
	for(i = 0; i< 99; i++)
		matRefI(a);

	uint64_t end = __k1_read_dsu_timestamp();


	float t1 = (float) ((end - start)/(float)CHIP_FREQ);
	printf("\ttime using 1 thread : %.3f ms\n", t1/100);
	
	start = __k1_read_dsu_timestamp();
	
	omp_set_num_threads(16);
	matRefI(a);
	for(i = 0; i< 99; i++)
		matRefI(a);
	
	end = __k1_read_dsu_timestamp();
	
	float t2 = (float) ((end - start)/(float)CHIP_FREQ);
	printf("\ttime using 16 threads : %.3f ms\n", t2/100);
	printf("\tspeedup is : %.3f\n", t1/t2);

	freeMatrixI(a);
	#endif
}

void test_scaleLineI()
{
	Matrix_i *a = matrixAllocI(3,3);
	createIdentityI(a);


	
	scaleLineI(a, 0,2);

	int tab[9] = {2,0,0,0,1,0,0,0,1};
	test_result(a, tab, __FUNCTION__);

	freeMatrixI(a);
	#if TIME_DISPLAY == 1
	unsigned int i;	

	a = createRandomMatrixI(1,10000);

	uint64_t start = __k1_read_dsu_timestamp();
	omp_set_num_threads(1);
	scaleLineI(a, 0,2);
	for(i = 0; i< 99; i++)
		scaleLineI(a, 0,2);

	uint64_t end = __k1_read_dsu_timestamp();


	float t1 = (float) ((end - start)/(float)CHIP_FREQ);
	printf("\ttime using 1 thread : %.3f ms\n", t1/100);
	
	start = __k1_read_dsu_timestamp();
	
	omp_set_num_threads(16);
	scaleLineI(a, 0,2);
	for(i = 0; i< 99; i++)
		scaleLineI(a, 0,2);
	
	end = __k1_read_dsu_timestamp();
	
	float t2 = (float) ((end - start)/(float)CHIP_FREQ);
	printf("\ttime using 16 threads : %.3f ms\n", t2/100);
	printf("\tspeedup is : %.3f\n", t1/t2);

	freeMatrixI(a);
	#endif
}

void test_subXLinesI()
{
	Matrix_i *a = matrixAllocI(3,3);
	createIdentityI(a);


	
	subXLinesI(a,1,0,2);

	int tab[9] = {1,0,0,-2,1,0,0,0,1};
	test_result(a, tab, __FUNCTION__);

	freeMatrixI(a);
	#if TIME_DISPLAY == 1
	unsigned int i;	

	a = createRandomMatrixI(2,5000);

	uint64_t start = __k1_read_dsu_timestamp();
	omp_set_num_threads(1);
	subXLinesI(a,1,0,2);
	for(i = 0; i< 99; i++)
		subXLinesI(a,1,0,2);

	uint64_t end = __k1_read_dsu_timestamp();


	float t1 = (float) ((end - start)/(float)CHIP_FREQ);
	printf("\ttime using 1 thread : %.3f ms\n", t1/100);
	
	start = __k1_read_dsu_timestamp();
	
	omp_set_num_threads(16);
	subXLinesI(a,1,0,2);
	for(i = 0; i< 99; i++)
		subXLinesI(a,1,0,2);
	
	end = __k1_read_dsu_timestamp();
	
	float t2 = (float) ((end - start)/(float)CHIP_FREQ);
	printf("\ttime using 16 threads : %.3f ms\n", t2/100);
	printf("\tspeedup is : %.3f\n", t1/t2);

	freeMatrixI(a);
	#endif
}

void test_isDiagI()
{
	Matrix_i * a = matrixAllocI(3,3);
	createIdentityI(a);
	if(isDiagI(a))
		printSuccess(__FUNCTION__);
	else
		printFailure(__FUNCTION__);
	matrixSetI(a,1,0,-3);
	if(!isDiagI(a))
		printSuccess(__FUNCTION__);
	else
		printFailure(__FUNCTION__);
	freeMatrixI(a);
}

void test_isTriUI()
{
	Matrix_i * a = matrixAllocI(3,3);
	createIdentityI(a);
	if(isTriUI(a))
		printSuccess(__FUNCTION__);
	else
		printFailure(__FUNCTION__);
	matrixSetI(a,1,0,-3);
	if(!isTriUI(a))
		printSuccess(__FUNCTION__);
	else
		printFailure(__FUNCTION__);
	freeMatrixI(a);
}

void test_isTriLI()
{
	Matrix_i * a = matrixAllocI(3,3);
	createIdentityI(a);
	if(isTriLI(a))
		printSuccess(__FUNCTION__);
	else
		printFailure(__FUNCTION__);
	matrixSetI(a,0,1,-3);
	if(!isTriLI(a))
		printSuccess(__FUNCTION__);
	else
		printFailure(__FUNCTION__);
	freeMatrixI(a);
}

void test_isSparseI()
{
	Matrix_i * a = matrixAllocI(3,3);
	matrixSetI(a,0,1,-3);
	if(isSparseI(a))
		printSuccess(__FUNCTION__);
	else
		printFailure(__FUNCTION__);
	createIdentityI(a);
	matrixSetI(a,0,2,-3);
	if(!isSparseI(a))
		printSuccess(__FUNCTION__);
	else
		printFailure(__FUNCTION__);
	freeMatrixI(a);
}
