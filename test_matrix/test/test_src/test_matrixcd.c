#include "../test_head/test_matrixcd.h"
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

#define NB_TESTCD 13



static void printSuccess(const char * func)
{
	printf("\033[32m%s succeed \033[39m \n", func);
}

static void printFailure(const char * func)
{
	printf("\033[31m%s failed \033[39m \n", func);
}

static int equals(Complex_d a, Complex_d b)
{
	return (equals_f(a.re,b.re,1e-6) && equals_f(a.im,b.im,1e-6));
}

static void test_result(Matrix_cd* mat, Complex_d *a, const char * func)
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


void testAllCD()
{
	printf("***\tTesting complex doubles matrix Functions\t***\n\n");
	test_scaleMatrixCD();
	test_addMatrixCD();
	test_addMatrixTCD();
	test_subMatrixCD();
	test_mulMatrixCD();
	test_mulAddScaleMatrixCD();
	test_matRefCD();
	test_scaleLineCD();
	test_subXLinesCD();
	test_isDiagCD();
	test_isTriUCD();
	test_isTriLCD();
	test_isSparseCD();
}


void test_scaleMatrixCD()
{
	Matrix_cd *a = matrixAllocCD(3,3);
	createIdentityCD(a);

	Complex_d cpx = {2,0};
	
	scaleMatrixCD(a, cpx);
	Complex_d c0 ={0,0};
	Complex_d tab[9] = {cpx,c0,c0,c0,cpx,c0,c0,c0,cpx};
	test_result(a, tab, __FUNCTION__);


	freeMatrixCD(a);

	#if TIME_DISPLAY == 1
	unsigned int i;	
	a = createRandomMatrixCD(100,100);

	uint64_t start = __k1_read_dsu_timestamp();
	omp_set_num_threads(1);
	scaleMatrixCD(a, cpx);
	for(i = 0; i< 99; i++)
		scaleMatrixCD(a, cpx);

	uint64_t end = __k1_read_dsu_timestamp();


	float t1 = (float) ((end - start)/(float)CHIP_FREQ);
	printf("\ttime using 1 thread : %.3f ms\n", t1/100);
	
	start = __k1_read_dsu_timestamp();
	
	omp_set_num_threads(16);
	scaleMatrixCD(a, cpx);
	for(i = 0; i< 99; i++)
		scaleMatrixCD(a, cpx);
	
	end = __k1_read_dsu_timestamp();
	
	float t2 = (float) ((end - start)/(float)CHIP_FREQ);
	printf("\ttime using 16 threads : %.3f ms\n", t2/100);
	printf("\tspeedup is : %.3f\n", t1/t2);

	freeMatrixCD(a);
	#endif
}

void test_addMatrixCD()
{
	Matrix_cd *a = matrixAllocCD(3,3);
	createIdentityCD(a);
	Matrix_cd *b = matrixAllocCD(3,3);
	createIdentityCD(b);


	
	addMatrixCD(a, b);
	Complex_d cpx ={2,0};
	Complex_d c0 ={0,0};
	Complex_d tab[9] = {cpx,c0,c0,c0,cpx,c0,c0,c0,cpx};
	test_result(a, tab, __FUNCTION__);

	freeMatrixCD(a);
	freeMatrixCD(b);

	#if TIME_DISPLAY == 1
	unsigned int i;	

	a = createRandomMatrixCD(100,100);
	b = createRandomMatrixCD(100,100);

	uint64_t start = __k1_read_dsu_timestamp();
	omp_set_num_threads(1);
	addMatrixCD(a, b);
	for(i = 0; i< 99; i++)
		addMatrixCD(b, b);

	uint64_t end = __k1_read_dsu_timestamp();

	float t1 = (float) ((end - start)/(float)CHIP_FREQ);
	printf("\ttime using 1 thread : %.3f ms\n", t1/100);
	
	start = __k1_read_dsu_timestamp();
	
	omp_set_num_threads(16);
	addMatrixCD(a, b);
	for(i = 0; i< 99; i++)
		addMatrixCD(b, b);
	
	end = __k1_read_dsu_timestamp();
	
	float t2 = (float) ((end - start)/(float)CHIP_FREQ);
	printf("\ttime using 16 threads : %.3f ms\n", t2/100);
	printf("\tspeedup is : %.3f\n", t1/t2);

	freeMatrixCD(a);
	freeMatrixCD(b);
	#endif
}

void test_addMatrixTCD()
{
	Matrix_cd *a = matrixAllocCD(3,3);
	createIdentityCD(a);
	Matrix_cd *b = matrixAllocCD(3,3);
	createIdentityCD(b);


	
	addMatrixTCD(a, b);
	Complex_d cpx ={2,0};
	Complex_d c0 ={0,0};
	Complex_d tab[9] = {cpx,c0,c0,c0,cpx,c0,c0,c0,cpx};
	test_result(a, tab, __FUNCTION__);

	freeMatrixCD(a);
	freeMatrixCD(b);

	#if TIME_DISPLAY == 1
	unsigned int i;	

	a = createRandomMatrixCD(100,100);
	b = createRandomMatrixCD(100,100);

	uint64_t start = __k1_read_dsu_timestamp();
	omp_set_num_threads(1);
	addMatrixTCD(a, b);
	for(i = 0; i< 99; i++)
		addMatrixTCD(b, b);

	uint64_t end = __k1_read_dsu_timestamp();

	float t1 = (float) ((end - start)/(float)CHIP_FREQ);
	printf("\ttime using 1 thread : %.3f ms\n", t1/100);
	
	start = __k1_read_dsu_timestamp();
	
	omp_set_num_threads(16);
	addMatrixTCD(a, b);
	for(i = 0; i< 99; i++)
		addMatrixTCD(b, b);
	
	end = __k1_read_dsu_timestamp();
	
	float t2 = (float) ((end - start)/(float)CHIP_FREQ);
	printf("\ttime using 16 threads : %.3f ms\n", t2/100);
	printf("\tspeedup is : %.3f\n", t1/t2);

	freeMatrixCD(a);
	freeMatrixCD(b);
	#endif
}

void test_subMatrixCD()
{
	Matrix_cd *a = matrixAllocCD(3,3);
	createIdentityCD(a);
	Matrix_cd *b = matrixAllocCD(3,3);
	createIdentityCD(b);


	
	subMatrixCD(a, b);

	Complex_d c0 ={0,0};
	Complex_d tab[9] = {c0,c0,c0,c0,c0,c0,c0,c0,c0};
	test_result(a, tab, __FUNCTION__);

	freeMatrixCD(a);
	freeMatrixCD(b);

	#if TIME_DISPLAY == 1
	unsigned int i;	

	a = createRandomMatrixCD(100,100);
	b = createRandomMatrixCD(100,100);

	uint64_t start = __k1_read_dsu_timestamp();
	omp_set_num_threads(1);
	subMatrixCD(a, b);
	for(i = 0; i< 99; i++)
		subMatrixCD(b, b);

	uint64_t end = __k1_read_dsu_timestamp();


	float t1 = (float) ((end - start)/(float)CHIP_FREQ);
	printf("\ttime using 1 thread : %.3f ms\n", t1/100);
	
	start = __k1_read_dsu_timestamp();
	
	omp_set_num_threads(16);
	subMatrixCD(a, b);
	for(i = 0; i< 99; i++)
		subMatrixCD(b, b);
	
	end = __k1_read_dsu_timestamp();
	
	float t2 = (float) ((end - start)/(float)CHIP_FREQ);
	printf("\ttime using 16 threads : %.3f ms\n", t2/100);
	printf("\tspeedup is : %.3f\n", t1/t2);

	freeMatrixCD(a);
	freeMatrixCD(b);
	#endif
}

void test_mulMatrixCD()
{
	Matrix_cd *a = matrixAllocCD(3,3);
	createIdentityCD(a);
	Matrix_cd *b = matrixAllocCD(3,3);
	createIdentityCD(b);


	
	mulMatrixCD(a, a, b);
	Complex_d cpx ={1,0};
	Complex_d c0 ={0,0};
	Complex_d tab[9] = {cpx,c0,c0,c0,cpx,c0,c0,c0,cpx};
	test_result(a, tab, __FUNCTION__);

	freeMatrixCD(a);
	freeMatrixCD(b);

	#if TIME_DISPLAY == 1
	unsigned int i;	

	a = createRandomMatrixCD(100,100);
	b = createRandomMatrixCD(100,100);

	uint64_t start = __k1_read_dsu_timestamp();
	omp_set_num_threads(1);
	mulMatrixCD(a, a, b);
	for(i = 0; i< 19; i++)
		mulMatrixCD(a, b, b);

	uint64_t end = __k1_read_dsu_timestamp();


	float t1 = (float) ((end - start)/(float)CHIP_FREQ);
	printf("\ttime using 1 thread : %.3f ms\n", t1/20);
	
	start = __k1_read_dsu_timestamp();
	
	omp_set_num_threads(16);
	mulMatrixCD(a, a, b);
	for(i = 0; i< 19; i++)
		mulMatrixCD(a, b, b);
	
	end = __k1_read_dsu_timestamp();
	
	float t2 = (float) ((end - start)/(float)CHIP_FREQ);
	printf("\ttime using 16 threads : %.3f ms\n", t2/20);
	printf("\tspeedup is : %.3f\n", t1/t2);

	freeMatrixCD(a);
	freeMatrixCD(b);
	#endif
}

void test_mulAddScaleMatrixCD()
{
	Matrix_cd *a = matrixAllocCD(3,3);
	createIdentityCD(a);
	Matrix_cd *b = matrixAllocCD(3,3);
	createIdentityCD(b);
	Matrix_cd *c = matrixAllocCD(3,3);
	createIdentityCD(c);


	Complex_d cpx = {2,0};
	mulAddScaleMatrixCD(a,a,b,c,cpx,cpx);

	Complex_d c1 ={6,0};
	Complex_d c0 ={0,0};
	Complex_d tab[9] = {c1,c0,c0,c0,c1,c0,c0,c0,c1};
	test_result(a, tab, __FUNCTION__);

	freeMatrixCD(a);
	freeMatrixCD(b);
	freeMatrixCD(c);

	#if TIME_DISPLAY == 1
	unsigned int i;	

	a = createRandomMatrixCD(100,100);
	b = createRandomMatrixCD(100,100);
	c = createRandomMatrixCD(100,100);

	uint64_t start = __k1_read_dsu_timestamp();
	omp_set_num_threads(1);
	mulAddScaleMatrixCD(a,a,b,c,cpx,cpx);
	for(i = 0; i< 19; i++)
		mulAddScaleMatrixCD(a,a,b,c,cpx,cpx);

	uint64_t end = __k1_read_dsu_timestamp();


	float t1 = (float) ((end - start)/(float)CHIP_FREQ);
	printf("\ttime using 1 thread : %.3f ms\n", t1/20);
	
	start = __k1_read_dsu_timestamp();
	
	omp_set_num_threads(16);
	mulAddScaleMatrixCD(a,a,b,c,cpx,cpx);
	for(i = 0; i< 19; i++)
		mulAddScaleMatrixCD(a,a,b,c,cpx,cpx);
	
	end = __k1_read_dsu_timestamp();
	
	float t2 = (float) ((end - start)/(float)CHIP_FREQ);
	printf("\ttime using 16 threads : %.3f ms\n", t2/20);
	printf("\tspeedup is : %.3f\n", t1/t2);

	freeMatrixCD(a);
	freeMatrixCD(b);
	freeMatrixCD(c);
	#endif
}

void test_matRefCD()
{
	Matrix_cd *a = matrixAllocCD(3,3);
	createIdentityCD(a);


	
	matRefCD(a);

	Complex_d cpx ={1,0};
	Complex_d c0 ={0,0};
	Complex_d tab[9] = {cpx,c0,c0,c0,cpx,c0,c0,c0,cpx};
	test_result(a, tab, __FUNCTION__);

	freeMatrixCD(a);

	#if TIME_DISPLAY == 1
	unsigned int i;	

	a = createRandomMatrixCD(100,100);

	uint64_t start = __k1_read_dsu_timestamp();
	omp_set_num_threads(1);
	matRefCD(a);
	for(i = 0; i< 99; i++)
		matRefCD(a);

	uint64_t end = __k1_read_dsu_timestamp();


	float t1 = (float) ((end - start)/(float)CHIP_FREQ);
	printf("\ttime using 1 thread : %.3f ms\n", t1/100);
	
	start = __k1_read_dsu_timestamp();
	
	omp_set_num_threads(16);
	matRefCD(a);
	for(i = 0; i< 99; i++)
		matRefCD(a);
	
	end = __k1_read_dsu_timestamp();
	
	float t2 = (float) ((end - start)/(float)CHIP_FREQ);
	printf("\ttime using 16 threads : %.3f ms\n", t2/100);
	printf("\tspeedup is : %.3f\n", t1/t2);

	freeMatrixCD(a);
	#endif
}

void test_scaleLineCD()
{
	Matrix_cd *a = matrixAllocCD(3,3);
	createIdentityCD(a);

	Complex_d cpx = {2,0};
	
	scaleLineCD(a, 0,cpx);

	Complex_d c1 ={1,0};
	Complex_d c0 ={0,0};
	Complex_d tab[9] = {cpx,c0,c0,c0,c1,c0,c0,c0,c1};
	test_result(a, tab, __FUNCTION__);

	freeMatrixCD(a);
	#if TIME_DISPLAY == 1
	unsigned int i;	

	a = createRandomMatrixCD(1,10000);

	uint64_t start = __k1_read_dsu_timestamp();
	omp_set_num_threads(1);
	scaleLineCD(a, 0,cpx);
	for(i = 0; i< 99; i++)
		scaleLineCD(a, 0,cpx);

	uint64_t end = __k1_read_dsu_timestamp();


	float t1 = (float) ((end - start)/(float)CHIP_FREQ);
	printf("\ttime using 1 thread : %.3f ms\n", t1/100);
	
	start = __k1_read_dsu_timestamp();
	
	omp_set_num_threads(16);
	scaleLineCD(a, 0,cpx);
	for(i = 0; i< 99; i++)
		scaleLineCD(a, 0,cpx);
	
	end = __k1_read_dsu_timestamp();
	
	float t2 = (float) ((end - start)/(float)CHIP_FREQ);
	printf("\ttime using 16 threads : %.3f ms\n", t2/100);
	printf("\tspeedup is : %.3f\n", t1/t2);

	freeMatrixCD(a);
	#endif
}

void test_subXLinesCD()
{
	Matrix_cd *a = matrixAllocCD(3,3);
	createIdentityCD(a);

	Complex_d cpx = {2,0};
	
	subXLinesCD(a,1,0,cpx);

	Complex_d c2 ={-2,0};	
	Complex_d c1 ={1,0};
	Complex_d c0 ={0,0};
	Complex_d tab[9] = {c1,c0,c0,c2,c1,c0,c0,c0,c1};
	test_result(a, tab, __FUNCTION__);

	freeMatrixCD(a);
	#if TIME_DISPLAY == 1
	unsigned int i;	

	a = createRandomMatrixCD(2,5000);

	uint64_t start = __k1_read_dsu_timestamp();
	omp_set_num_threads(1);
	subXLinesCD(a,1,0,cpx);
	for(i = 0; i< 99; i++)
		subXLinesCD(a,1,0,cpx);

	uint64_t end = __k1_read_dsu_timestamp();


	float t1 = (float) ((end - start)/(float)CHIP_FREQ);
	printf("\ttime using 1 thread : %.3f ms\n", t1/100);
	
	start = __k1_read_dsu_timestamp();
	
	omp_set_num_threads(16);
	subXLinesCD(a,1,0,cpx);
	for(i = 0; i< 99; i++)
		subXLinesCD(a,1,0,cpx);
	
	end = __k1_read_dsu_timestamp();
	
	float t2 = (float) ((end - start)/(float)CHIP_FREQ);
	printf("\ttime using 16 threads : %.3f ms\n", t2/100);
	printf("\tspeedup is : %.3f\n", t1/t2);

	freeMatrixCD(a);
	#endif
}

void test_isDiagCD()
{
	Matrix_cd * a = matrixAllocCD(3,3);
	Complex_d cpx = {2,0};
	createIdentityCD(a);
	if(isDiagCD(a))
		printSuccess(__FUNCTION__);
	else
		printFailure(__FUNCTION__);
	matrixSetCD(a,1,0,cpx);
	if(!isDiagCD(a))
		printSuccess(__FUNCTION__);
	else
		printFailure(__FUNCTION__);
	freeMatrixCD(a);
}

void test_isTriUCD()
{
	Matrix_cd * a = matrixAllocCD(3,3);
	Complex_d cpx = {2,0};
	createIdentityCD(a);
	if(isTriUCD(a))
		printSuccess(__FUNCTION__);
	else
		printFailure(__FUNCTION__);
	matrixSetCD(a,1,0,cpx);
	if(!isTriUCD(a))
		printSuccess(__FUNCTION__);
	else
		printFailure(__FUNCTION__);
	freeMatrixCD(a);
}

void test_isTriLCD()
{
	Matrix_cd * a = matrixAllocCD(3,3);
	createIdentityCD(a);
	Complex_d cpx = {2,0};
	if(isTriLCD(a))
		printSuccess(__FUNCTION__);
	else
		printFailure(__FUNCTION__);
	matrixSetCD(a,0,1,cpx);
	if(!isTriLCD(a))
		printSuccess(__FUNCTION__);
	else
		printFailure(__FUNCTION__);
	freeMatrixCD(a);
}

void test_isSparseCD()
{
	Matrix_cd * a = matrixAllocCD(3,3);
	Complex_d cpx = {2,0};
	matrixSetCD(a,0,1,cpx);
	if(isSparseCD(a))
		printSuccess(__FUNCTION__);
	else
		printFailure(__FUNCTION__);
	createIdentityCD(a);
	matrixSetCD(a,0,2,cpx);
	if(!isSparseCD(a))
		printSuccess(__FUNCTION__);
	else
		printFailure(__FUNCTION__);
	freeMatrixCD(a);
}
