#include "../test_head/test_complex_f.h"


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

#define CHIP_FREQ (__bsp_frequency/1000000)


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

static void test_result(Complex_f a, Complex_f b, const char * func)
{
	unsigned int i;
	for (i =0 ; i< 9; i++)
	{	
		if(!equals(a,b))
		{
			printFailure(func);
			return;		
		}
	}	
	printSuccess(func);
}

void testAll_cpxF()
{
	printf("\n\n***\tTesting complex float functions\t***\n\n");
	test_equals_f();
	test_isNull_f();
	test_conjugateF();
	test_addF();
	test_subF();
	test_scaleF();
	test_multiplyF();
	test_divF();
	test_invF();
	test_multiplyByConjF();
	test_gainF();
	test_createNulComplexF();
	test_createUnitaryComplexF();
	test_createComplexF();
}


void test_equals_f()
{
	float precis = 1e-6;
	float a = 1.2;
	float b = 1.20000001;
	float c = 1.2001;
	if(equals_f(a,b, precis) && !equals_f(a,c, precis))
	{
		printSuccess(__FUNCTION__);
		return;
	}
	printFailure(__FUNCTION__);

	#if TIME_DISPLAY == 1
	unsigned int i;	
	a = 0.8;
	b = 0.9;

	uint64_t start = __k1_read_dsu_timestamp();
	
	for(i = 0; i< 100; i++)
		equals_f(a,b, precis);
	
	uint64_t end = __k1_read_dsu_timestamp();
	
	float t2 = (float) ((end - start)/(float)CHIP_FREQ);
	printf("\ttime : %.3f ns\n", t2/100);

	#endif
}

void test_isNull_f()
{
	Complex_f a = {0,0};
	Complex_f b = {0.0000000001,0.0000000001};
	Complex_f c = {0.001,0.001};
	if(isNull_f(a) && isNull_f(b)&& !isNull_f(c))
	{
		printSuccess(__FUNCTION__);
		return;
	}
	printFailure(__FUNCTION__);
	
	#if TIME_DISPLAY == 1
	unsigned int i;	
	a = createRandomComplexF();

	uint64_t start = __k1_read_dsu_timestamp();
	
	for(i = 0; i< 100; i++)
		isNull_f(a);
	
	uint64_t end = __k1_read_dsu_timestamp();
	
	float t2 = (float) ((end - start)/(float)CHIP_FREQ);
	printf("\ttime : %.3f ns\n", t2/100);

	#endif
}

void test_conjugateF()
{
	Complex_f a = {1,1};
	Complex_f aconj = {1,-1};
	test_result(conjugateF(a), aconj, __FUNCTION__);

	#if TIME_DISPLAY == 1
	unsigned int i;	
	a = createRandomComplexF();

	uint64_t start = __k1_read_dsu_timestamp();
	
	for(i = 0; i< 100; i++)
		conjugateF(a);
	
	uint64_t end = __k1_read_dsu_timestamp();
	
	float t2 = (float) ((end - start)/(float)CHIP_FREQ);
	printf("\ttime : %.3f ns\n", t2/100);

	#endif
}

void test_addF()
{
	Complex_f a = {1,1};
	Complex_f res = {2,2};
	test_result(addF(a,a), res, __FUNCTION__);

	#if TIME_DISPLAY == 1
	unsigned int i;	
	a = createRandomComplexF();

	uint64_t start = __k1_read_dsu_timestamp();
	
	for(i = 0; i< 100; i++)
		addF(a,a);
	
	uint64_t end = __k1_read_dsu_timestamp();
	
	float t2 = (float) ((end - start)/(float)CHIP_FREQ);
	printf("\ttime : %.3f ns\n", t2/100);

	#endif
}

void test_subF()
{
	Complex_f a = {1,1};
	Complex_f res = {0,0};
	test_result(subF(a,a), res, __FUNCTION__);

	#if TIME_DISPLAY == 1
	unsigned int i;	
	a = createRandomComplexF();

	uint64_t start = __k1_read_dsu_timestamp();
	
	for(i = 0; i< 100; i++)
		subF(a,a);
	
	uint64_t end = __k1_read_dsu_timestamp();
	
	float t2 = (float) ((end - start)/(float)CHIP_FREQ);
	printf("\ttime : %.3f ns\n", t2/100);

	#endif
}

void test_scaleF()
{
	Complex_f a = {1,1};
	Complex_f res = {2.5,2.5};
	test_result(scaleF(a,2.5), res, __FUNCTION__);

	#if TIME_DISPLAY == 1
	unsigned int i;	
	a = createRandomComplexF();

	uint64_t start = __k1_read_dsu_timestamp();
	
	for(i = 0; i< 100; i++)
		scaleF(a,2.63546);
	
	uint64_t end = __k1_read_dsu_timestamp();
	
	float t2 = (float) ((end - start)/(float)CHIP_FREQ);
	printf("\ttime : %.3f ns\n", t2/100);

	#endif
}

void test_multiplyF()
{
	Complex_f a = {1,2};
	Complex_f b = {2,1};
	Complex_f res = {0,5};
	test_result(multiplyF(a,b), res, __FUNCTION__);

	#if TIME_DISPLAY == 1
	unsigned int i;	
	a = createRandomComplexF();

	uint64_t start = __k1_read_dsu_timestamp();
	
	for(i = 0; i< 100; i++)
		multiplyF(a,b);
	
	uint64_t end = __k1_read_dsu_timestamp();
	
	float t2 = (float) ((end - start)/(float)CHIP_FREQ);
	printf("\ttime : %.3f ns\n", t2/100);

	#endif
}

void test_divF()
{
	Complex_f a = {1,2};
	Complex_f b = {2,1};
	Complex_f res = {0.8,0.6};
	test_result(divF(a,b), res, __FUNCTION__);

	#if TIME_DISPLAY == 1
	unsigned int i;	
	a = createRandomComplexF();

	uint64_t start = __k1_read_dsu_timestamp();
	
	for(i = 0; i< 100; i++)
		divF(a,b);
	
	uint64_t end = __k1_read_dsu_timestamp();
	
	float t2 = (float) ((end - start)/(float)CHIP_FREQ);
	printf("\ttime : %.3f ns\n", t2/100);

	#endif
}

void test_invF()
{
	Complex_f a = {1,2};
	Complex_f res = {0.2,-0.4};
	test_result(invF(a), res, __FUNCTION__);

	#if TIME_DISPLAY == 1
	unsigned int i;	
	a = createRandomComplexF();

	uint64_t start = __k1_read_dsu_timestamp();
	
	for(i = 0; i< 100; i++)
		invF(a);
	
	uint64_t end = __k1_read_dsu_timestamp();
	
	float t2 = (float) ((end - start)/(float)CHIP_FREQ);
	printf("\ttime : %.3f ns\n", t2/100);

	#endif
}

void test_multiplyByConjF()
{
	Complex_f a = {1,2};
	Complex_f res = {5,0};
	test_result(multiplyByConjF(a), res, __FUNCTION__);

	#if TIME_DISPLAY == 1
	unsigned int i;	
	a = createRandomComplexF();

	uint64_t start = __k1_read_dsu_timestamp();
	
	for(i = 0; i< 100; i++)
		multiplyByConjF(a);
	
	uint64_t end = __k1_read_dsu_timestamp();
	
	float t2 = (float) ((end - start)/(float)CHIP_FREQ);
	printf("\ttime : %.3f ns\n", t2/100);

	#endif
}

void test_gainF()
{
	Complex_f a = {4,3};
	Complex_f b = {0,0};
	b.re = gainF(a);
	Complex_f res = {5,0};
	test_result(b, res, __FUNCTION__);

	#if TIME_DISPLAY == 1
	unsigned int i;	
	a = createRandomComplexF();

	uint64_t start = __k1_read_dsu_timestamp();
	
	for(i = 0; i< 100; i++)
		gainF(a);
	
	uint64_t end = __k1_read_dsu_timestamp();
	
	float t2 = (float) ((end - start)/(float)CHIP_FREQ);
	printf("\ttime : %.3f ns\n", t2/100);

	#endif
}

void test_createNulComplexF()
{
	Complex_f a = {0,0};
	test_result(createNulComplexF(), a, __FUNCTION__);

	#if TIME_DISPLAY == 1
	unsigned int i;	

	uint64_t start = __k1_read_dsu_timestamp();
	
	for(i = 0; i< 100; i++)
		createNulComplexF();
	
	uint64_t end = __k1_read_dsu_timestamp();
	
	float t2 = (float) ((end - start)/(float)CHIP_FREQ);
	printf("\ttime : %.3f ns\n", t2/100);

	#endif
}

void test_createUnitaryComplexF()
{
	Complex_f a = {1,1};
	test_result(createUnitaryComplexF(), a, __FUNCTION__);

	#if TIME_DISPLAY == 1
	unsigned int i;	

	uint64_t start = __k1_read_dsu_timestamp();
	
	for(i = 0; i< 100; i++)
		createUnitaryComplexF();
	
	uint64_t end = __k1_read_dsu_timestamp();
	
	float t2 = (float) ((end - start)/(float)CHIP_FREQ);
	printf("\ttime : %.3f ns\n", t2/100);

	#endif
}

void test_createComplexF()
{
	Complex_f a = {1,2};
	test_result(createComplexF(1,2), a, __FUNCTION__);


	#if TIME_DISPLAY == 1
	unsigned int i;	

	uint64_t start = __k1_read_dsu_timestamp();
	
	for(i = 0; i< 100; i++)
		createComplexF(1,2);
	
	uint64_t end = __k1_read_dsu_timestamp();
	
	float t2 = (float) ((end - start)/(float)CHIP_FREQ);
	printf("\ttime : %.3f ns\n", t2/100);

	#endif
}


