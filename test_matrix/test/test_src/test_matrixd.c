#include "../test_head/test_matrixd.h"
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

#define NB_TESTD 13


static void printSuccess(const char * func)
{
	printf("\033[32m%s succeed \033[39m \n", func);
}

static void printFailure(const char * func)
{
	printf("\033[31m%s failed \033[39m \n", func);
}

static int equals(float a, float b)
{
	return (fabs(a-b) < 1e-6);
}

static void test_result(Matrix_d* mat, float *a, const char * func)
{
	unsigned int i;
	for (i =0 ; i< 9; i++)
	{	
		if(!equals(mat->data[i],a[i]))
		{
			printFailure(func);
			return;		
		}
	}	
	printSuccess(func);
}


void testAllD()
{
	printf("***\tTesting double matrix Dunctions\t***\n\n");
	test_scaleMatrixD();
	test_addMatrixD();
	test_addMatrixTD();
	test_subMatrixD();
	test_mulMatrixD();
	test_mulAddScaleMatrixD();
	test_matRefD();
	test_scaleLineD();
	test_subXLinesD();
	test_isDiagD();
	test_isTriUD();
	test_isTriLD();
	test_isSparseD();
}


void test_scaleMatrixD()
{
	Matrix_d *a = matrixAllocD(3,3);
	createIdentityD(a);

	scaleMatrixD(a, 2);

	float tab[9] = {2.f,0.f,0.f,0.f,2.f,0.f,0.f,0.f,2.f};
	test_result(a, tab, __FUNCTION__);

	freeMatrixD(a);

	#if TIME_DISPLAY == 1
	unsigned int i;	
	a = createRandomMatrixD(100,100);

	uint64_t start = __k1_read_dsu_timestamp();
	omp_set_num_threads(1);
	scaleMatrixD(a, 2);
	for(i = 0; i< 99; i++)
		scaleMatrixD(a, 1);

	uint64_t end = __k1_read_dsu_timestamp();


	float t1 = (float) ((end - start)/(float)CHIP_FREQ);
	printf("\ttime using 1 thread : %.3f ms\n", t1/100);
	
	start = __k1_read_dsu_timestamp();
	
	omp_set_num_threads(16);
	scaleMatrixD(a, 2);
	for(i = 0; i< 99; i++)
		scaleMatrixD(a, 1);
	
	end = __k1_read_dsu_timestamp();
	
	float t2 = (float) ((end - start)/(float)CHIP_FREQ);
	printf("\ttime using 16 threads : %.3f ms\n", t2/100);
	printf("\tspeedup is : %.3f\n", t1/t2);

	freeMatrixD(a);
	#endif
}

void test_addMatrixD()
{
	Matrix_d *a = matrixAllocD(3,3);
	createIdentityD(a);
	Matrix_d *b = matrixAllocD(3,3);
	createIdentityD(b);


	
	addMatrixD(a, b);

	float tab[9] = {2.f,0.f,0.f,0.f,2.f,0.f,0.f,0.f,2.f};
	test_result(a, tab, __FUNCTION__);

	freeMatrixD(a);
	freeMatrixD(b);

	#if TIME_DISPLAY == 1
	unsigned int i;	

	a = createRandomMatrixD(100,100);
	b = createRandomMatrixD(100,100);

	uint64_t start = __k1_read_dsu_timestamp();
	omp_set_num_threads(1);
	addMatrixD(a, b);
	for(i = 0; i< 99; i++)
		addMatrixD(b, b);

	uint64_t end = __k1_read_dsu_timestamp();

	float t1 = (float) ((end - start)/(float)CHIP_FREQ);
	printf("\ttime using 1 thread : %.3f ms\n", t1/100);
	
	start = __k1_read_dsu_timestamp();
	
	omp_set_num_threads(16);
	addMatrixD(a, b);
	for(i = 0; i< 99; i++)
		addMatrixD(b, b);
	
	end = __k1_read_dsu_timestamp();
	
	float t2 = (float) ((end - start)/(float)CHIP_FREQ);
	printf("\ttime using 16 threads : %.3f ms\n", t2/100);
	printf("\tspeedup is : %.3f\n", t1/t2);

	freeMatrixD(a);
	freeMatrixD(b);
	#endif
}

void test_addMatrixTD()
{
	Matrix_d *a = matrixAllocD(3,3);
	createIdentityD(a);
	Matrix_d *b = matrixAllocD(3,3);
	createIdentityD(b);


	
	addMatrixTD(a, b);

	float tab[9] = {2.f,0.f,0.f,0.f,2.f,0.f,0.f,0.f,2.f};
	test_result(a, tab, __FUNCTION__);

	freeMatrixD(a);
	freeMatrixD(b);

	#if TIME_DISPLAY == 1
	unsigned int i;	

	a = createRandomMatrixD(100,100);
	b = createRandomMatrixD(100,100);

	uint64_t start = __k1_read_dsu_timestamp();
	omp_set_num_threads(1);
	addMatrixTD(a, b);
	for(i = 0; i< 99; i++)
		addMatrixTD(b, b);

	uint64_t end = __k1_read_dsu_timestamp();

	float t1 = (float) ((end - start)/(float)CHIP_FREQ);
	printf("\ttime using 1 thread : %.3f ms\n", t1/100);
	
	start = __k1_read_dsu_timestamp();
	
	omp_set_num_threads(16);
	addMatrixTD(a, b);
	for(i = 0; i< 99; i++)
		addMatrixTD(b, b);
	
	end = __k1_read_dsu_timestamp();
	
	float t2 = (float) ((end - start)/(float)CHIP_FREQ);
	printf("\ttime using 16 threads : %.3f ms\n", t2/100);
	printf("\tspeedup is : %.3f\n", t1/t2);

	freeMatrixD(a);
	freeMatrixD(b);
	#endif
}

void test_subMatrixD()
{
	Matrix_d *a = matrixAllocD(3,3);
	createIdentityD(a);
	Matrix_d *b = matrixAllocD(3,3);
	createIdentityD(b);


	
	subMatrixD(a, b);

	float tab[9] = {0.f};
	test_result(a, tab, __FUNCTION__);

	freeMatrixD(a);
	freeMatrixD(b);

	#if TIME_DISPLAY == 1
	unsigned int i;	

	a = createRandomMatrixD(100,100);
	b = createRandomMatrixD(100,100);

	uint64_t start = __k1_read_dsu_timestamp();
	omp_set_num_threads(1);
	subMatrixD(a, b);
	for(i = 0; i< 99; i++)
		subMatrixD(b, b);

	uint64_t end = __k1_read_dsu_timestamp();


	float t1 = (float) ((end - start)/(float)CHIP_FREQ);
	printf("\ttime using 1 thread : %.3f ms\n", t1/100);
	
	start = __k1_read_dsu_timestamp();
	
	omp_set_num_threads(16);
	subMatrixD(a, b);
	for(i = 0; i< 99; i++)
		subMatrixD(b, b);
	
	end = __k1_read_dsu_timestamp();
	
	float t2 = (float) ((end - start)/(float)CHIP_FREQ);
	printf("\ttime using 16 threads : %.3f ms\n", t2/100);
	printf("\tspeedup is : %.3f\n", t1/t2);

	freeMatrixD(a);
	freeMatrixD(b);
	#endif
}

void test_mulMatrixD()
{
	Matrix_d *a = matrixAllocD(3,3);
	createIdentityD(a);
	Matrix_d *b = matrixAllocD(3,3);
	createIdentityD(b);


	
	mulMatrixD(a, a, b);

	float tab[9] = {1.f,0.f,0.f,0.f,1.f,0.f,0.f,0.f,1.f};
	test_result(a, tab, __FUNCTION__);

	freeMatrixD(a);
	freeMatrixD(b);

	#if TIME_DISPLAY == 1
	unsigned int i;	

	a = createRandomMatrixD(100,100);
	b = createRandomMatrixD(100,100);

	uint64_t start = __k1_read_dsu_timestamp();
	omp_set_num_threads(1);
	mulMatrixD(a, a, b);
	for(i = 0; i< 19; i++)
		mulMatrixD(a, b, b);

	uint64_t end = __k1_read_dsu_timestamp();


	float t1 = (float) ((end - start)/(float)CHIP_FREQ);
	printf("\ttime using 1 thread : %.3f ms\n", t1/20);
	
	start = __k1_read_dsu_timestamp();
	
	omp_set_num_threads(16);
	mulMatrixD(a, a, b);
	for(i = 0; i< 19; i++)
		mulMatrixD(a, b, b);
	
	end = __k1_read_dsu_timestamp();
	
	float t2 = (float) ((end - start)/(float)CHIP_FREQ);
	printf("\ttime using 16 threads : %.3f ms\n", t2/20);
	printf("\tspeedup is : %.3f\n", t1/t2);

	freeMatrixD(a);
	freeMatrixD(b);
	#endif
}

void test_mulAddScaleMatrixD()
{
	Matrix_d *a = matrixAllocD(3,3);
	createIdentityD(a);
	Matrix_d *b = matrixAllocD(3,3);
	createIdentityD(b);
	Matrix_d *c = matrixAllocD(3,3);
	createIdentityD(c);


	
	mulAddScaleMatrixD(a,a,b,c,2,2);

	float tab[9] = {6.f,0.f,0.f,0.f,6.f,0.f,0.f,0.f,6.f};
	test_result(a, tab, __FUNCTION__);

	freeMatrixD(a);
	freeMatrixD(b);
	freeMatrixD(c);

	#if TIME_DISPLAY == 1
	unsigned int i;	

	a = createRandomMatrixD(100,100);
	b = createRandomMatrixD(100,100);
	c = createRandomMatrixD(100,100);

	uint64_t start = __k1_read_dsu_timestamp();
	omp_set_num_threads(1);
	mulAddScaleMatrixD(a,a,b,c,2,2);
	for(i = 0; i< 19; i++)
		mulAddScaleMatrixD(a,a,b,c,2,2);

	uint64_t end = __k1_read_dsu_timestamp();


	float t1 = (float) ((end - start)/(float)CHIP_FREQ);
	printf("\ttime using 1 thread : %.3f ms\n", t1/20);
	
	start = __k1_read_dsu_timestamp();
	
	omp_set_num_threads(16);
	mulAddScaleMatrixD(a,a,b,c,2,2);
	for(i = 0; i< 19; i++)
		mulAddScaleMatrixD(a,a,b,c,2,2);
	
	end = __k1_read_dsu_timestamp();
	
	float t2 = (float) ((end - start)/(float)CHIP_FREQ);
	printf("\ttime using 16 threads : %.3f ms\n", t2/20);
	printf("\tspeedup is : %.3f\n", t1/t2);

	freeMatrixD(a);
	freeMatrixD(b);
	freeMatrixD(c);
	#endif
}

void test_matRefD()
{
	Matrix_d *a = matrixAllocD(3,3);
	createIdentityD(a);


	
	matRefD(a);

	float tab[9] = {1.f,0.f,0.f,0.f,1.f,0.f,0.f,0.f,1.f};
	test_result(a, tab, __FUNCTION__);

	freeMatrixD(a);

	#if TIME_DISPLAY == 1
	unsigned int i;	

	a = createRandomMatrixD(100,100);

	uint64_t start = __k1_read_dsu_timestamp();
	omp_set_num_threads(1);
	matRefD(a);
	for(i = 0; i< 99; i++)
		matRefD(a);

	uint64_t end = __k1_read_dsu_timestamp();


	float t1 = (float) ((end - start)/(float)CHIP_FREQ);
	printf("\ttime using 1 thread : %.3f ms\n", t1/100);
	
	start = __k1_read_dsu_timestamp();
	
	omp_set_num_threads(16);
	matRefD(a);
	for(i = 0; i< 99; i++)
		matRefD(a);
	
	end = __k1_read_dsu_timestamp();
	
	float t2 = (float) ((end - start)/(float)CHIP_FREQ);
	printf("\ttime using 16 threads : %.3f ms\n", t2/100);
	printf("\tspeedup is : %.3f\n", t1/t2);

	freeMatrixD(a);
	#endif
}

void test_scaleLineD()
{
	Matrix_d *a = matrixAllocD(3,3);
	createIdentityD(a);


	
	scaleLineD(a, 0,2);

	float tab[9] = {2.f,0.f,0.f,0.f,1.f,0.f,0.f,0.f,1.f};
	test_result(a, tab, __FUNCTION__);

	freeMatrixD(a);
	#if TIME_DISPLAY == 1
	unsigned int i;	

	a = createRandomMatrixD(1,10000);

	uint64_t start = __k1_read_dsu_timestamp();
	omp_set_num_threads(1);
	scaleLineD(a, 0,2);
	for(i = 0; i< 99; i++)
		scaleLineD(a, 0,2);

	uint64_t end = __k1_read_dsu_timestamp();


	float t1 = (float) ((end - start)/(float)CHIP_FREQ);
	printf("\ttime using 1 thread : %.3f ms\n", t1/100);
	
	start = __k1_read_dsu_timestamp();
	
	omp_set_num_threads(16);
	scaleLineD(a, 0,2);
	for(i = 0; i< 99; i++)
		scaleLineD(a, 0,2);
	
	end = __k1_read_dsu_timestamp();
	
	float t2 = (float) ((end - start)/(float)CHIP_FREQ);
	printf("\ttime using 16 threads : %.3f ms\n", t2/100);
	printf("\tspeedup is : %.3f\n", t1/t2);

	freeMatrixD(a);
	#endif
}

void test_subXLinesD()
{
	Matrix_d *a = matrixAllocD(3,3);
	createIdentityD(a);


	
	subXLinesD(a,1,0,2);

	float tab[9] = {1.f,0.f,0.f,-2.f,1.f,0.f,0.f,0.f,1.f};
	test_result(a, tab, __FUNCTION__);

	freeMatrixD(a);
	#if TIME_DISPLAY == 1
	unsigned int i;	

	a = createRandomMatrixD(2,5000);

	uint64_t start = __k1_read_dsu_timestamp();
	omp_set_num_threads(1);
	subXLinesD(a,1,0,2);
	for(i = 0; i< 99; i++)
		subXLinesD(a,1,0,2);

	uint64_t end = __k1_read_dsu_timestamp();


	float t1 = (float) ((end - start)/(float)CHIP_FREQ);
	printf("\ttime using 1 thread : %.3f ms\n", t1/100);
	
	start = __k1_read_dsu_timestamp();
	
	omp_set_num_threads(16);
	subXLinesD(a,1,0,2);
	for(i = 0; i< 99; i++)
		subXLinesD(a,1,0,2);
	
	end = __k1_read_dsu_timestamp();
	
	float t2 = (float) ((end - start)/(float)CHIP_FREQ);
	printf("\ttime using 16 threads : %.3f ms\n", t2/100);
	printf("\tspeedup is : %.3f\n", t1/t2);

	freeMatrixD(a);
	#endif
}

void test_isDiagD()
{
	Matrix_d * a = matrixAllocD(3,3);
	createIdentityD(a);
	if(isDiagD(a))
		printSuccess(__FUNCTION__);
	else
		printFailure(__FUNCTION__);
	matrixSetD(a,1,0,-3);
	if(!isDiagD(a))
		printSuccess(__FUNCTION__);
	else
		printFailure(__FUNCTION__);
	freeMatrixD(a);
}

void test_isTriUD()
{
	Matrix_d * a = matrixAllocD(3,3);
	createIdentityD(a);
	if(isTriUD(a))
		printSuccess(__FUNCTION__);
	else
		printFailure(__FUNCTION__);
	matrixSetD(a,1,0,-3);
	if(!isTriUD(a))
		printSuccess(__FUNCTION__);
	else
		printFailure(__FUNCTION__);
	freeMatrixD(a);
}

void test_isTriLD()
{
	Matrix_d * a = matrixAllocD(3,3);
	createIdentityD(a);
	if(isTriLD(a))
		printSuccess(__FUNCTION__);
	else
		printFailure(__FUNCTION__);
	matrixSetD(a,0,1,-3);
	if(!isTriLD(a))
		printSuccess(__FUNCTION__);
	else
		printFailure(__FUNCTION__);
	freeMatrixD(a);
}

void test_isSparseD()
{
	Matrix_d * a = matrixAllocD(3,3);
	matrixSetD(a,0,1,-3);
	if(isSparseD(a))
		printSuccess(__FUNCTION__);
	else
		printFailure(__FUNCTION__);
	createIdentityD(a);
	matrixSetD(a,0,2,-3);
	if(!isSparseD(a))
		printSuccess(__FUNCTION__);
	else
		printFailure(__FUNCTION__);
	freeMatrixD(a);
}
