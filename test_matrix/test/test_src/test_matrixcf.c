#include "../test_head/test_matrixcf.h"
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

#define NB_TESTCF 13



static void printSuccess(const char * func)
{
	printf("\033[32m%s succeed \033[39m \n", func);
}

static void printFailure(const char * func)
{
	printf("\033[31m%s failed \033[39m \n", func);
}

static int equals(Complex_f a, Complex_f b)
{
	return (equals_f(a.re,b.re,1e-6) && equals_f(a.im,b.im,1e-6));
}

static void test_result(Matrix_cf* mat, Complex_f *a, const char * func)
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

void testAllCF()
{
	printf("***\tTesting complex float matrix Functions\t***\n\n");
	test_scaleMatrixCF();
	test_addMatrixCF();
	test_addMatrixTCF();
	test_subMatrixCF();
	test_mulMatrixCF();
	test_mulAddScaleMatrixCF();
	test_matRefCF();
	test_scaleLineCF();
	test_subXLinesCF();
	test_isDiagCF();
	test_isTriUCF();
	test_isTriLCF();
	test_isSparseCF();
}


void test_scaleMatrixCF()
{
	Matrix_cf *a = matrixAllocCF(3,3);
	createIdentityCF(a);

	Complex_f cpx = {2,0};
	Complex_f c0 = {0,0};
	scaleMatrixCF(a, cpx);

	Complex_f tab[9] = {cpx,c0,c0,c0,cpx,c0,c0,c0,cpx};
	test_result(a, tab, __FUNCTION__);

	freeMatrixCF(a);

	#if TIME_DISPLAY == 1
	unsigned int i;	
	a = createRandomMatrixCF(100,100);

	uint64_t start = __k1_read_dsu_timestamp();
	omp_set_num_threads(1);
	scaleMatrixCF(a, cpx);
	for(i = 0; i< 99; i++)
		scaleMatrixCF(a, cpx);

	uint64_t end = __k1_read_dsu_timestamp();


	float t1 = (float) ((end - start)/(float)CHIP_FREQ);
	printf("\ttime using 1 thread : %.3f ms\n", t1/100);
	
	start = __k1_read_dsu_timestamp();
	
	omp_set_num_threads(16);
	scaleMatrixCF(a, cpx);
	for(i = 0; i< 99; i++)
		scaleMatrixCF(a, cpx);
	
	end = __k1_read_dsu_timestamp();
	
	float t2 = (float) ((end - start)/(float)CHIP_FREQ);
	printf("\ttime using 16 threads : %.3f ms\n", t2/100);
	printf("\tspeedup is : %.3f\n", t1/t2);

	freeMatrixCF(a);
	#endif
}

void test_addMatrixCF()
{
	Matrix_cf *a = matrixAllocCF(3,3);
	createIdentityCF(a);
	Matrix_cf *b = matrixAllocCF(3,3);
	createIdentityCF(b);


	
	addMatrixCF(a, b);

	Complex_f cpx ={2,0};
	Complex_f c0 ={0,0};
	Complex_f tab[9] = {cpx,c0,c0,c0,cpx,c0,c0,c0,cpx};
	test_result(a, tab, __FUNCTION__);

	freeMatrixCF(a);
	freeMatrixCF(b);

	#if TIME_DISPLAY == 1
	unsigned int i;	

	a = createRandomMatrixCF(100,100);
	b = createRandomMatrixCF(100,100);

	uint64_t start = __k1_read_dsu_timestamp();
	omp_set_num_threads(1);
	addMatrixCF(a, b);
	for(i = 0; i< 99; i++)
		addMatrixCF(b, b);

	uint64_t end = __k1_read_dsu_timestamp();

	float t1 = (float) ((end - start)/(float)CHIP_FREQ);
	printf("\ttime using 1 thread : %.3f ms\n", t1/100);
	
	start = __k1_read_dsu_timestamp();
	
	omp_set_num_threads(16);
	addMatrixCF(a, b);
	for(i = 0; i< 99; i++)
		addMatrixCF(b, b);
	
	end = __k1_read_dsu_timestamp();
	
	float t2 = (float) ((end - start)/(float)CHIP_FREQ);
	printf("\ttime using 16 threads : %.3f ms\n", t2/100);
	printf("\tspeedup is : %.3f\n", t1/t2);

	freeMatrixCF(a);
	freeMatrixCF(b);
	#endif
}

void test_addMatrixTCF()
{
	Matrix_cf *a = matrixAllocCF(3,3);
	createIdentityCF(a);
	Matrix_cf *b = matrixAllocCF(3,3);
	createIdentityCF(b);


	
	addMatrixTCF(a, b);

	Complex_f cpx ={2,0};
	Complex_f c0 ={0,0};
	Complex_f tab[9] = {cpx,c0,c0,c0,cpx,c0,c0,c0,cpx};
	test_result(a, tab, __FUNCTION__);

	freeMatrixCF(a);
	freeMatrixCF(b);

	#if TIME_DISPLAY == 1
	unsigned int i;	

	a = createRandomMatrixCF(100,100);
	b = createRandomMatrixCF(100,100);

	uint64_t start = __k1_read_dsu_timestamp();
	omp_set_num_threads(1);
	addMatrixTCF(a, b);
	for(i = 0; i< 99; i++)
		addMatrixTCF(b, b);

	uint64_t end = __k1_read_dsu_timestamp();

	float t1 = (float) ((end - start)/(float)CHIP_FREQ);
	printf("\ttime using 1 thread : %.3f ms\n", t1/100);
	
	start = __k1_read_dsu_timestamp();
	
	omp_set_num_threads(16);
	addMatrixTCF(a, b);
	for(i = 0; i< 99; i++)
		addMatrixTCF(b, b);
	
	end = __k1_read_dsu_timestamp();
	
	float t2 = (float) ((end - start)/(float)CHIP_FREQ);
	printf("\ttime using 16 threads : %.3f ms\n", t2/100);
	printf("\tspeedup is : %.3f\n", t1/t2);

	freeMatrixCF(a);
	freeMatrixCF(b);
	#endif
}

void test_subMatrixCF()
{
	Matrix_cf *a = matrixAllocCF(3,3);
	createIdentityCF(a);
	Matrix_cf *b = matrixAllocCF(3,3);
	createIdentityCF(b);


	
	subMatrixCF(a, b);

	Complex_f c0 ={0,0};
	Complex_f tab[9] = {c0,c0,c0,c0,c0,c0,c0,c0,c0};
	test_result(a, tab, __FUNCTION__);

	freeMatrixCF(a);
	freeMatrixCF(b);

	#if TIME_DISPLAY == 1
	unsigned int i;	

	a = createRandomMatrixCF(100,100);
	b = createRandomMatrixCF(100,100);

	uint64_t start = __k1_read_dsu_timestamp();
	omp_set_num_threads(1);
	subMatrixCF(a, b);
	for(i = 0; i< 99; i++)
		subMatrixCF(b, b);

	uint64_t end = __k1_read_dsu_timestamp();


	float t1 = (float) ((end - start)/(float)CHIP_FREQ);
	printf("\ttime using 1 thread : %.3f ms\n", t1/100);
	
	start = __k1_read_dsu_timestamp();
	
	omp_set_num_threads(16);
	subMatrixCF(a, b);
	for(i = 0; i< 99; i++)
		subMatrixCF(b, b);
	
	end = __k1_read_dsu_timestamp();
	
	float t2 = (float) ((end - start)/(float)CHIP_FREQ);
	printf("\ttime using 16 threads : %.3f ms\n", t2/100);
	printf("\tspeedup is : %.3f\n", t1/t2);

	freeMatrixCF(a);
	freeMatrixCF(b);
	#endif
}

void test_mulMatrixCF()
{
	Matrix_cf *a = matrixAllocCF(3,3);
	createIdentityCF(a);
	Matrix_cf *b = matrixAllocCF(3,3);
	createIdentityCF(b);


	
	mulMatrixCF(a, a, b);

	Complex_f cpx ={1,0};
	Complex_f c0 ={0,0};
	Complex_f tab[9] = {cpx,c0,c0,c0,cpx,c0,c0,c0,cpx};
	test_result(a, tab, __FUNCTION__);

	freeMatrixCF(a);
	freeMatrixCF(b);

	#if TIME_DISPLAY == 1
	unsigned int i;	

	a = createRandomMatrixCF(100,100);
	b = createRandomMatrixCF(100,100);

	uint64_t start = __k1_read_dsu_timestamp();
	omp_set_num_threads(1);
	mulMatrixCF(a, a, b);
	for(i = 0; i< 19; i++)
		mulMatrixCF(a, b, b);

	uint64_t end = __k1_read_dsu_timestamp();


	float t1 = (float) ((end - start)/(float)CHIP_FREQ);
	printf("\ttime using 1 thread : %.3f ms\n", t1/20);
	
	start = __k1_read_dsu_timestamp();
	
	omp_set_num_threads(16);
	mulMatrixCF(a, a, b);
	for(i = 0; i< 19; i++)
		mulMatrixCF(a, b, b);
	
	end = __k1_read_dsu_timestamp();
	
	float t2 = (float) ((end - start)/(float)CHIP_FREQ);
	printf("\ttime using 16 threads : %.3f ms\n", t2/20);
	printf("\tspeedup is : %.3f\n", t1/t2);

	freeMatrixCF(a);
	freeMatrixCF(b);
	#endif
}

void test_mulAddScaleMatrixCF()
{
	Matrix_cf *a = matrixAllocCF(3,3);
	createIdentityCF(a);
	Matrix_cf *b = matrixAllocCF(3,3);
	createIdentityCF(b);
	Matrix_cf *c = matrixAllocCF(3,3);
	createIdentityCF(c);


	Complex_f cpx = {2,0};
	mulAddScaleMatrixCF(a,a,b,c,cpx,cpx);

	cpx.re = 6;
	Complex_f c0 ={0,0};
	Complex_f tab[9] = {cpx,c0,c0,c0,cpx,c0,c0,c0,cpx};
	test_result(a, tab, __FUNCTION__);

	freeMatrixCF(a);
	freeMatrixCF(b);
	freeMatrixCF(c);

	#if TIME_DISPLAY == 1
	unsigned int i;	

	a = createRandomMatrixCF(100,100);
	b = createRandomMatrixCF(100,100);
	c = createRandomMatrixCF(100,100);

	uint64_t start = __k1_read_dsu_timestamp();
	omp_set_num_threads(1);
	mulAddScaleMatrixCF(a,a,b,c,cpx,cpx);
	for(i = 0; i< 19; i++)
		mulAddScaleMatrixCF(a,a,b,c,cpx,cpx);

	uint64_t end = __k1_read_dsu_timestamp();


	float t1 = (float) ((end - start)/(float)CHIP_FREQ);
	printf("\ttime using 1 thread : %.3f ms\n", t1/20);
	
	start = __k1_read_dsu_timestamp();
	
	omp_set_num_threads(16);
	mulAddScaleMatrixCF(a,a,b,c,cpx,cpx);
	for(i = 0; i< 19; i++)
		mulAddScaleMatrixCF(a,a,b,c,cpx,cpx);
	
	end = __k1_read_dsu_timestamp();
	
	float t2 = (float) ((end - start)/(float)CHIP_FREQ);
	printf("\ttime using 16 threads : %.3f ms\n", t2/20);
	printf("\tspeedup is : %.3f\n", t1/t2);

	freeMatrixCF(a);
	freeMatrixCF(b);
	freeMatrixCF(c);
	#endif
}

void test_matRefCF()
{
	Matrix_cf *a = matrixAllocCF(3,3);
	createIdentityCF(a);


	
	matRefCF(a);

	Complex_f cpx ={1,0};
	Complex_f c0 ={0,0};
	Complex_f tab[9] = {cpx,c0,c0,c0,cpx,c0,c0,c0,cpx};
	test_result(a, tab, __FUNCTION__);

	freeMatrixCF(a);

	#if TIME_DISPLAY == 1
	unsigned int i;	

	a = createRandomMatrixCF(100,100);

	uint64_t start = __k1_read_dsu_timestamp();
	omp_set_num_threads(1);
	matRefCF(a);
	for(i = 0; i< 99; i++)
		matRefCF(a);

	uint64_t end = __k1_read_dsu_timestamp();


	float t1 = (float) ((end - start)/(float)CHIP_FREQ);
	printf("\ttime using 1 thread : %.3f ms\n", t1/100);
	
	start = __k1_read_dsu_timestamp();
	
	omp_set_num_threads(16);
	matRefCF(a);
	for(i = 0; i< 99; i++)
		matRefCF(a);
	
	end = __k1_read_dsu_timestamp();
	
	float t2 = (float) ((end - start)/(float)CHIP_FREQ);
	printf("\ttime using 16 threads : %.3f ms\n", t2/100);
	printf("\tspeedup is : %.3f\n", t1/t2);

	freeMatrixCF(a);
	#endif
}

void test_scaleLineCF()
{
	Matrix_cf *a = matrixAllocCF(3,3);
	createIdentityCF(a);

	Complex_f cpx = {2,0};
	
	scaleLineCF(a, 0,cpx);

	Complex_f c1 ={1,0};
	Complex_f c0 ={0,0};
	Complex_f tab[9] = {cpx,c0,c0,c0,c1,c0,c0,c0,c1};
	test_result(a, tab, __FUNCTION__);

	freeMatrixCF(a);
	#if TIME_DISPLAY == 1
	unsigned int i;	

	a = createRandomMatrixCF(1,10000);

	uint64_t start = __k1_read_dsu_timestamp();
	omp_set_num_threads(1);
	scaleLineCF(a, 0,cpx);
	for(i = 0; i< 99; i++)
		scaleLineCF(a, 0,cpx);

	uint64_t end = __k1_read_dsu_timestamp();


	float t1 = (float) ((end - start)/(float)CHIP_FREQ);
	printf("\ttime using 1 thread : %.3f ms\n", t1/100);
	
	start = __k1_read_dsu_timestamp();
	
	omp_set_num_threads(16);
	scaleLineCF(a, 0,cpx);
	for(i = 0; i< 99; i++)
		scaleLineCF(a, 0,cpx);
	
	end = __k1_read_dsu_timestamp();
	
	float t2 = (float) ((end - start)/(float)CHIP_FREQ);
	printf("\ttime using 16 threads : %.3f ms\n", t2/100);
	printf("\tspeedup is : %.3f\n", t1/t2);

	freeMatrixCF(a);
	#endif
}

void test_subXLinesCF()
{
	Matrix_cf *a = matrixAllocCF(3,3);
	createIdentityCF(a);

	Complex_f cpx = {2,0};
	
	subXLinesCF(a,1,0,cpx);

	Complex_f c2 ={-2,0};
	Complex_f c1 = {1,0};
	Complex_f c0 ={0,0};
	Complex_f tab[9] = {c1,c0,c0,c2,c1,c0,c0,c0,c1};
	test_result(a, tab, __FUNCTION__);

	freeMatrixCF(a);
	#if TIME_DISPLAY == 1
	unsigned int i;	

	a = createRandomMatrixCF(2,5000);

	uint64_t start = __k1_read_dsu_timestamp();
	omp_set_num_threads(1);
	subXLinesCF(a,1,0,cpx);
	for(i = 0; i< 99; i++)
		subXLinesCF(a,1,0,cpx);

	uint64_t end = __k1_read_dsu_timestamp();


	float t1 = (float) ((end - start)/(float)CHIP_FREQ);
	printf("\ttime using 1 thread : %.3f ms\n", t1/100);
	
	start = __k1_read_dsu_timestamp();
	
	omp_set_num_threads(16);
	subXLinesCF(a,1,0,cpx);
	for(i = 0; i< 99; i++)
		subXLinesCF(a,1,0,cpx);
	
	end = __k1_read_dsu_timestamp();
	
	float t2 = (float) ((end - start)/(float)CHIP_FREQ);
	printf("\ttime using 16 threads : %.3f ms\n", t2/100);
	printf("\tspeedup is : %.3f\n", t1/t2);

	freeMatrixCF(a);
	#endif
}

void test_isDiagCF()
{
	Matrix_cf * a = matrixAllocCF(3,3);
	Complex_f cpx = {2,0};
	createIdentityCF(a);
	if(isDiagCF(a))
		printSuccess(__FUNCTION__);
	else
		printFailure(__FUNCTION__);
	matrixSetCF(a,1,0,cpx);
	if(!isDiagCF(a))
		printSuccess(__FUNCTION__);
	else
		printFailure(__FUNCTION__);
	freeMatrixCF(a);
}

void test_isTriUCF()
{
	Matrix_cf * a = matrixAllocCF(3,3);
	Complex_f cpx = {2,0};
	createIdentityCF(a);
	if(isTriUCF(a))
		printSuccess(__FUNCTION__);
	else
		printFailure(__FUNCTION__);
	matrixSetCF(a,1,0,cpx);
	if(!isTriUCF(a))
		printSuccess(__FUNCTION__);
	else
		printFailure(__FUNCTION__);
	freeMatrixCF(a);
}

void test_isTriLCF()
{
	Matrix_cf * a = matrixAllocCF(3,3);
	createIdentityCF(a);
	Complex_f cpx = {2,0};
	if(isTriLCF(a))
		printSuccess(__FUNCTION__);
	else
		printFailure(__FUNCTION__);
	matrixSetCF(a,0,1,cpx);
	if(!isTriLCF(a))
		printSuccess(__FUNCTION__);
	else
		printFailure(__FUNCTION__);
	freeMatrixCF(a);
}

void test_isSparseCF()
{
	Matrix_cf * a = matrixAllocCF(3,3);
	Complex_f cpx = {2,0};
	matrixSetCF(a,0,1,cpx);
	if(isSparseCF(a))
		printSuccess(__FUNCTION__);
	else
		printFailure(__FUNCTION__);
	createIdentityCF(a);
	matrixSetCF(a,0,2,cpx);
	if(!isSparseCF(a))
		printSuccess(__FUNCTION__);
	else
		printFailure(__FUNCTION__);
	freeMatrixCF(a);
}
