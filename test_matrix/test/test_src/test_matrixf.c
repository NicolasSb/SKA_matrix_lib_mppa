#include "../test_head/test_matrixf.h"
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

#define NB_TESTF 13


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

static void test_result(Matrix_f* mat, float *a, const char * func)
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

void testAllF()
{
	printf("***\tTesting float matrix Functions\t***\n\n");
	test_scaleMatrixF();
	test_addMatrixF();
	test_addMatrixTF();
	test_subMatrixF();
	test_scaleSubMatrixF();
	test_mulMatrixF();
	test_mulAddScaleMatrixF();
	test_matRefF();
	test_scaleLineF();
	test_subXLinesF();
	test_isDiagF();
	test_isTriUF();
	test_isTriLF();
	test_isSparseF();
}


void test_scaleMatrixF()
{
	Matrix_f *a = matrixAllocF(3,3);
	createIdentityF(a);

	scaleMatrixF(a, 2);

	float tab[9] = {2.f,0.f,0.f,0.f,2.f,0.f,0.f,0.f,2.f};
	test_result(a, tab, __FUNCTION__);

	freeMatrixF(a);

	#if TIME_DISPLAY == 1
	unsigned int i;	
	a = createRandomMatrixF(100,100);

	uint64_t start = __k1_read_dsu_timestamp();
	omp_set_num_threads(1);
	scaleMatrixF(a, 2);
	for(i = 0; i< 99; i++)
		scaleMatrixF(a, 1);

	uint64_t end = __k1_read_dsu_timestamp();


	float t1 = (float) ((end - start)/(float)CHIP_FREQ);
	printf("\ttime using 1 thread : %.3f ms\n", t1/100);
	
	start = __k1_read_dsu_timestamp();
	
	omp_set_num_threads(16);
	scaleMatrixF(a, 2);
	for(i = 0; i< 99; i++)
		scaleMatrixF(a, 1);
	
	end = __k1_read_dsu_timestamp();
	
	float t2 = (float) ((end - start)/(float)CHIP_FREQ);
	printf("\ttime using 16 threads : %.3f ms\n", t2/100);
	printf("\tspeedup is : %.3f\n", t1/t2);

	freeMatrixF(a);
	#endif
}

void test_addMatrixF()
{
	Matrix_f *a = matrixAllocF(3,3);
	createIdentityF(a);
	Matrix_f *b = matrixAllocF(3,3);
	createIdentityF(b);


	
	addMatrixF(a, b);

	float tab[9] = {2.f,0.f,0.f,0.f,2.f,0.f,0.f,0.f,2.f};
	test_result(a, tab, __FUNCTION__);

	freeMatrixF(a);
	freeMatrixF(b);

	#if TIME_DISPLAY == 1
	unsigned int i;	

	a = createRandomMatrixF(100,100);
	b = createRandomMatrixF(100,100);

	uint64_t start = __k1_read_dsu_timestamp();
	omp_set_num_threads(1);
	addMatrixF(a, b);
	for(i = 0; i< 99; i++)
		addMatrixF(b, b);

	uint64_t end = __k1_read_dsu_timestamp();

	float t1 = (float) ((end - start)/(float)CHIP_FREQ);
	printf("\ttime using 1 thread : %.3f ms\n", t1/100);
	
	start = __k1_read_dsu_timestamp();
	
	omp_set_num_threads(16);
	addMatrixF(a, b);
	for(i = 0; i< 99; i++)
		addMatrixF(b, b);
	
	end = __k1_read_dsu_timestamp();
	
	float t2 = (float) ((end - start)/(float)CHIP_FREQ);
	printf("\ttime using 16 threads : %.3f ms\n", t2/100);
	printf("\tspeedup is : %.3f\n", t1/t2);

	freeMatrixF(a);
	freeMatrixF(b);
	#endif
}

void test_addMatrixTF()
{
	Matrix_f *a = matrixAllocF(3,3);
	createIdentityF(a);
	Matrix_f *b = matrixAllocF(3,3);
	createIdentityF(b);


	
	addMatrixTF(a, b);

	float tab[9] = {2.f,0.f,0.f,0.f,2.f,0.f,0.f,0.f,2.f};
	test_result(a, tab, __FUNCTION__);

	freeMatrixF(a);
	freeMatrixF(b);

	#if TIME_DISPLAY == 1
	unsigned int i;	

	a = createRandomMatrixF(100,100);
	b = createRandomMatrixF(100,100);

	uint64_t start = __k1_read_dsu_timestamp();
	omp_set_num_threads(1);
	addMatrixTF(a, b);
	for(i = 0; i< 99; i++)
		addMatrixTF(b, b);

	uint64_t end = __k1_read_dsu_timestamp();

	float t1 = (float) ((end - start)/(float)CHIP_FREQ);
	printf("\ttime using 1 thread : %.3f ms\n", t1/100);
	
	start = __k1_read_dsu_timestamp();
	
	omp_set_num_threads(16);
	addMatrixTF(a, b);
	for(i = 0; i< 99; i++)
		addMatrixTF(b, b);
	
	end = __k1_read_dsu_timestamp();
	
	float t2 = (float) ((end - start)/(float)CHIP_FREQ);
	printf("\ttime using 16 threads : %.3f ms\n", t2/100);
	printf("\tspeedup is : %.3f\n", t1/t2);

	freeMatrixF(a);
	freeMatrixF(b);
	#endif
}

void test_subMatrixF()
{
	Matrix_f *a = matrixAllocF(3,3);
	createIdentityF(a);
	Matrix_f *b = matrixAllocF(3,3);
	createIdentityF(b);


	
	subMatrixF(a, b);

	float tab[9] = {0.f};
	test_result(a, tab, __FUNCTION__);

	freeMatrixF(a);
	freeMatrixF(b);

	#if TIME_DISPLAY == 1
	unsigned int i;	

	a = createRandomMatrixF(100,100);
	b = createRandomMatrixF(100,100);

	uint64_t start = __k1_read_dsu_timestamp();
	omp_set_num_threads(1);
	subMatrixF(a, b);
	for(i = 0; i< 99; i++)
		subMatrixF(b, b);

	uint64_t end = __k1_read_dsu_timestamp();


	float t1 = (float) ((end - start)/(float)CHIP_FREQ);
	printf("\ttime using 1 thread : %.3f ms\n", t1/100);
	
	start = __k1_read_dsu_timestamp();
	
	omp_set_num_threads(16);
	subMatrixF(a, b);
	for(i = 0; i< 99; i++)
		subMatrixF(b, b);
	
	end = __k1_read_dsu_timestamp();
	
	float t2 = (float) ((end - start)/(float)CHIP_FREQ);
	printf("\ttime using 16 threads : %.3f ms\n", t2/100);
	printf("\tspeedup is : %.3f\n", t1/t2);

	freeMatrixF(a);
	freeMatrixF(b);
	#endif
}

void test_scaleSubMatrixF()
{
	Matrix_f *a = matrixAllocF(3,3);
	createIdentityF(a);
	Matrix_f *b = matrixAllocF(3,3);
	createIdentityF(b);

	
	scaleSubMatrixF(a, b, 2);

	float tab[9] = {-1.f,0.f,0.f,0.f,-1.f,0.f,0.f,0.f,-1.f};
	test_result(a, tab, __FUNCTION__);

	freeMatrixF(a);
	freeMatrixF(b);

	#if TIME_DISPLAY == 1
	unsigned int i;	

	a = createRandomMatrixF(100,100);
	b = createRandomMatrixF(100,100);

	uint64_t start = __k1_read_dsu_timestamp();
	omp_set_num_threads(1);
	subMatrixF(a, b);
	for(i = 0; i< 99; i++)
		subMatrixF(b, b);

	uint64_t end = __k1_read_dsu_timestamp();


	float t1 = (float) ((end - start)/(float)CHIP_FREQ);
	printf("\ttime using 1 thread : %.3f ms\n", t1/100);
	
	start = __k1_read_dsu_timestamp();
	
	omp_set_num_threads(16);
	subMatrixF(a, b);
	for(i = 0; i< 99; i++)
		subMatrixF(b, b);
	
	end = __k1_read_dsu_timestamp();
	
	float t2 = (float) ((end - start)/(float)CHIP_FREQ);
	printf("\ttime using 16 threads : %.3f ms\n", t2/100);
	printf("\tspeedup is : %.3f\n", t1/t2);

	freeMatrixF(a);
	freeMatrixF(b);
	#endif
}

void test_mulMatrixF()
{
	Matrix_f *a = matrixAllocF(3,3);
	createIdentityF(a);
	Matrix_f *b = matrixAllocF(3,3);
	createIdentityF(b);


	
	mulMatrixF(a, a, b);

	float tab[9] = {1.f,0.f,0.f,0.f,1.f,0.f,0.f,0.f,1.f};
	test_result(a, tab, __FUNCTION__);

	freeMatrixF(a);
	freeMatrixF(b);

	#if TIME_DISPLAY == 1
	unsigned int i;	

	a = createRandomMatrixF(100,100);
	b = createRandomMatrixF(100,100);

	uint64_t start = __k1_read_dsu_timestamp();
	omp_set_num_threads(1);
	mulMatrixF(a, a, b);
	for(i = 0; i< 19; i++)
		mulMatrixF(a, b, b);

	uint64_t end = __k1_read_dsu_timestamp();


	float t1 = (float) ((end - start)/(float)CHIP_FREQ);
	printf("\ttime using 1 thread : %.3f ms\n", t1/20);
	
	start = __k1_read_dsu_timestamp();
	
	omp_set_num_threads(16);
	mulMatrixF(a, a, b);
	for(i = 0; i< 19; i++)
		mulMatrixF(a, b, b);
	
	end = __k1_read_dsu_timestamp();
	
	float t2 = (float) ((end - start)/(float)CHIP_FREQ);
	printf("\ttime using 16 threads : %.3f ms\n", t2/20);
	printf("\tspeedup is : %.3f\n", t1/t2);

	freeMatrixF(a);
	freeMatrixF(b);
	#endif
}

void test_mulAddScaleMatrixF()
{
	Matrix_f *a = matrixAllocF(3,3);
	createIdentityF(a);
	Matrix_f *b = matrixAllocF(3,3);
	createIdentityF(b);
	Matrix_f *c = matrixAllocF(3,3);
	createIdentityF(c);


	
	mulAddScaleMatrixF(a,a,b,c,2,2);

	float tab[9] = {6.f,0.f,0.f,0.f,6.f,0.f,0.f,0.f,6.f};
	test_result(a, tab, __FUNCTION__);

	freeMatrixF(a);
	freeMatrixF(b);
	freeMatrixF(c);

	#if TIME_DISPLAY == 1
	unsigned int i;	

	a = createRandomMatrixF(100,100);
	b = createRandomMatrixF(100,100);
	c = createRandomMatrixF(100,100);

	uint64_t start = __k1_read_dsu_timestamp();
	omp_set_num_threads(1);
	mulAddScaleMatrixF(a,a,b,c,2,2);
	for(i = 0; i< 19; i++)
		mulAddScaleMatrixF(a,a,b,c,2,2);

	uint64_t end = __k1_read_dsu_timestamp();


	float t1 = (float) ((end - start)/(float)CHIP_FREQ);
	printf("\ttime using 1 thread : %.3f ms\n", t1/20);
	
	start = __k1_read_dsu_timestamp();
	
	omp_set_num_threads(16);
	mulAddScaleMatrixF(a,a,b,c,2,2);
	for(i = 0; i< 19; i++)
		mulAddScaleMatrixF(a,a,b,c,2,2);
	
	end = __k1_read_dsu_timestamp();
	
	float t2 = (float) ((end - start)/(float)CHIP_FREQ);
	printf("\ttime using 16 threads : %.3f ms\n", t2/20);
	printf("\tspeedup is : %.3f\n", t1/t2);

	freeMatrixF(a);
	freeMatrixF(b);
	freeMatrixF(c);
	#endif
}

void test_matRefF()
{
	Matrix_f *a = matrixAllocF(3,3);
	createIdentityF(a);


	
	matRefF(a);

	float tab[9] = {1.f,0.f,0.f,0.f,1.f,0.f,0.f,0.f,1.f};
	test_result(a, tab, __FUNCTION__);

	freeMatrixF(a);

	#if TIME_DISPLAY == 1
	unsigned int i;	

	a = createRandomMatrixF(100,100);

	uint64_t start = __k1_read_dsu_timestamp();
	omp_set_num_threads(1);
	matRefF(a);
	for(i = 0; i< 99; i++)
		matRefF(a);

	uint64_t end = __k1_read_dsu_timestamp();


	float t1 = (float) ((end - start)/(float)CHIP_FREQ);
	printf("\ttime using 1 thread : %.3f ms\n", t1/100);
	
	start = __k1_read_dsu_timestamp();
	
	omp_set_num_threads(16);
	matRefF(a);
	for(i = 0; i< 99; i++)
		matRefF(a);
	
	end = __k1_read_dsu_timestamp();
	
	float t2 = (float) ((end - start)/(float)CHIP_FREQ);
	printf("\ttime using 16 threads : %.3f ms\n", t2/100);
	printf("\tspeedup is : %.3f\n", t1/t2);

	freeMatrixF(a);
	#endif
}

void test_scaleLineF()
{
	Matrix_f *a = matrixAllocF(3,3);
	createIdentityF(a);


	
	scaleLineF(a, 0,2);

	float tab[9] = {2.f,0.f,0.f,0.f,1.f,0.f,0.f,0.f,1.f};
	test_result(a, tab, __FUNCTION__);

	freeMatrixF(a);
	#if TIME_DISPLAY == 1
	unsigned int i;	

	a = createRandomMatrixF(1,10000);

	uint64_t start = __k1_read_dsu_timestamp();
	omp_set_num_threads(1);
	scaleLineF(a, 0,2);
	for(i = 0; i< 99; i++)
		scaleLineF(a, 0,2);

	uint64_t end = __k1_read_dsu_timestamp();


	float t1 = (float) ((end - start)/(float)CHIP_FREQ);
	printf("\ttime using 1 thread : %.3f ms\n", t1/100);
	
	start = __k1_read_dsu_timestamp();
	
	omp_set_num_threads(16);
	scaleLineF(a, 0,2);
	for(i = 0; i< 99; i++)
		scaleLineF(a, 0,2);
	
	end = __k1_read_dsu_timestamp();
	
	float t2 = (float) ((end - start)/(float)CHIP_FREQ);
	printf("\ttime using 16 threads : %.3f ms\n", t2/100);
	printf("\tspeedup is : %.3f\n", t1/t2);

	freeMatrixF(a);
	#endif
}

void test_subXLinesF()
{
	Matrix_f *a = matrixAllocF(3,3);
	createIdentityF(a);


	
	subXLinesF(a,1,0,2);

	float tab[9] = {1.f,0.f,0.f,-2.f,1.f,0.f,0.f,0.f,1.f};
	test_result(a, tab, __FUNCTION__);
	freeMatrixF(a);
	#if TIME_DISPLAY == 1
	unsigned int i;	

	a = createRandomMatrixF(2,5000);

	uint64_t start = __k1_read_dsu_timestamp();
	omp_set_num_threads(1);
	subXLinesF(a,1,0,2);
	for(i = 0; i< 99; i++)
		subXLinesF(a,1,0,2);

	uint64_t end = __k1_read_dsu_timestamp();


	float t1 = (float) ((end - start)/(float)CHIP_FREQ);
	printf("\ttime using 1 thread : %.3f ms\n", t1/100);
	
	start = __k1_read_dsu_timestamp();
	
	omp_set_num_threads(16);
	subXLinesF(a,1,0,2);
	for(i = 0; i< 99; i++)
		subXLinesF(a,1,0,2);
	
	end = __k1_read_dsu_timestamp();
	
	float t2 = (float) ((end - start)/(float)CHIP_FREQ);
	printf("\ttime using 16 threads : %.3f ms\n", t2/100);
	printf("\tspeedup is : %.3f\n", t1/t2);

	freeMatrixF(a);
	#endif
}

void test_isDiagF()
{
	Matrix_f * a = matrixAllocF(3,3);
	createIdentityF(a);
	if(isDiagF(a))
		printSuccess(__FUNCTION__);
	else
		printFailure(__FUNCTION__);
	matrixSetF(a,1,0,-3);
	if(!isDiagF(a))
		printSuccess(__FUNCTION__);
	else
		printFailure(__FUNCTION__);
	freeMatrixF(a);
}

void test_isTriUF()
{
	Matrix_f * a = matrixAllocF(3,3);
	createIdentityF(a);
	if(isTriUF(a))
		printSuccess(__FUNCTION__);
	else
		printFailure(__FUNCTION__);
	matrixSetF(a,1,0,-3);
	if(!isTriUF(a))
		printSuccess(__FUNCTION__);
	else
		printFailure(__FUNCTION__);
	freeMatrixF(a);
}

void test_isTriLF()
{
	Matrix_f * a = matrixAllocF(3,3);
	createIdentityF(a);
	if(isTriLF(a))
		printSuccess(__FUNCTION__);
	else
		printFailure(__FUNCTION__);
	matrixSetF(a,0,1,-3);
	if(!isTriLF(a))
		printSuccess(__FUNCTION__);
	else
		printFailure(__FUNCTION__);
	freeMatrixF(a);
}

void test_isSparseF()
{
	Matrix_f * a = matrixAllocF(3,3);
	matrixSetF(a,0,1,-3);
	if(isSparseF(a))
		printSuccess(__FUNCTION__);
	else
		printFailure(__FUNCTION__);
	createIdentityF(a);
	matrixSetF(a,0,2,-3);
	if(!isSparseF(a))
		printSuccess(__FUNCTION__);
	else
		printFailure(__FUNCTION__);
	freeMatrixF(a);
}
