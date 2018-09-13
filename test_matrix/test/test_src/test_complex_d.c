#include "../test_head/test_complex_d.h"


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

static void printDailure(const char * func)
{
	printf("\033[31m%s failed \033[39m \n", func);
}

static int equals(Complex_d a, Complex_d b)
{
	return (equals_d(a.re,b.re,1e-6) && equals_d(a.im,b.im,1e-6));
}

static void test_result(Complex_d a, Complex_d b, const char * func)
{
	unsigned int i;
	for (i =0 ; i< 9; i++)
	{	
		if(!equals(a,b))
		{
			printDailure(func);
			return;		
		}
	}	
	printSuccess(func);
}

void testAll_cpxD()
{
	printf("\n\n***\tTesting complex double functions\t***\n\n");
	test_equals_d();
	test_isNull_d();
	test_conjugateD();
	test_addD();
	test_subD();
	test_scaleD();
	test_multiplyD();
	test_divD();
	test_invD();
	test_multiplyByConjD();
	test_gainD();
	test_createNulComplexD();
	test_createUnitaryComplexD();
	test_createComplexD();
}


void test_equals_d()
{
	float precis = 1e-6;
	float a = 1.2;
	float b = 1.20000001;
	float c = 1.2001;
	if(equals_d(a,b, precis) && !equals_d(a,c, precis))
	{
		printSuccess(__FUNCTION__);
		return;
	}
	printDailure(__FUNCTION__);

	#if TIME_DISPLAY == 1
	unsigned int i;	
	a = 0.8;
	b = 0.9;

	uint64_t start = __k1_read_dsu_timestamp();
	
	for(i = 0; i< 100; i++)
		equals_d(a,b,precis);
	
	uint64_t end = __k1_read_dsu_timestamp();
	
	float t2 = (float) ((end - start)/(float)CHIP_FREQ);
	printf("\ttime : %.3f ms\n", t2/100);

	#endif
}

void test_isNull_d()
{
	Complex_d a = {0,0};
	Complex_d b = {0.0000000001,0.0000000001};
	Complex_d c = {0.001,0.001};
	if(isNull_d(a) && isNull_d(b)&& !isNull_d(c))
	{
		printSuccess(__FUNCTION__);
		return;
	}
	printDailure(__FUNCTION__);

	#if TIME_DISPLAY == 1
	unsigned int i;	
	a = createRandomComplexD();

	uint64_t start = __k1_read_dsu_timestamp();
	
	for(i = 0; i< 100; i++)
		isNull_d(a);
	
	uint64_t end = __k1_read_dsu_timestamp();
	
	float t2 = (float) ((end - start)/(float)CHIP_FREQ);
	printf("\ttime : %.3f ns\n", t2/100);

	#endif
}

void test_conjugateD()
{
	Complex_d a = {1,1};
	Complex_d aconj = {1,-1};
	test_result(conjugateD(a), aconj, __FUNCTION__);

	#if TIME_DISPLAY == 1
	unsigned int i;	
	a = createRandomComplexD();

	uint64_t start = __k1_read_dsu_timestamp();
	
	for(i = 0; i< 100; i++)
		conjugateD(a);		
	
	uint64_t end = __k1_read_dsu_timestamp();
	
	float t2 = (float) ((end - start)/(float)CHIP_FREQ);
	printf("\ttime : %.3f ns\n", t2/100);

	#endif
}

void test_addD()
{
	Complex_d a = {1,1};
	Complex_d res = {2,2};
	test_result(addD(a,a), res, __FUNCTION__);

	#if TIME_DISPLAY == 1
	unsigned int i;	
	a = createRandomComplexD();

	uint64_t start = __k1_read_dsu_timestamp();
	
	for(i = 0; i< 100; i++)
		addD(a,a);		
	
	uint64_t end = __k1_read_dsu_timestamp();
	
	float t2 = (float) ((end - start)/(float)CHIP_FREQ);
	printf("\ttime : %.3f ns\n", t2/100);

	#endif
}

void test_subD()
{
	Complex_d a = {1,1};
	Complex_d res = {0,0};
	test_result(subD(a,a), res, __FUNCTION__);

	#if TIME_DISPLAY == 1
	unsigned int i;	
	a = createRandomComplexD();

	uint64_t start = __k1_read_dsu_timestamp();
	
	for(i = 0; i< 100; i++)
		subD(a,a);
	
	uint64_t end = __k1_read_dsu_timestamp();
	
	float t2 = (float) ((end - start)/(float)CHIP_FREQ);
	printf("\ttime : %.3f ns\n", t2/100);

	#endif
}

void test_scaleD()
{
	Complex_d a = {1,1};
	Complex_d res = {2.5,2.5};
	test_result(scaleD(a,2.5), res, __FUNCTION__);

	#if TIME_DISPLAY == 1
	unsigned int i;	
	a = createRandomComplexD();

	uint64_t start = __k1_read_dsu_timestamp();
	
	for(i = 0; i< 100; i++)
		scaleD(a,2.56513);		
	
	uint64_t end = __k1_read_dsu_timestamp();
	
	float t2 = (float) ((end - start)/(float)CHIP_FREQ);
	printf("\ttime : %.3f ns\n", t2/100);

	#endif
}

void test_multiplyD()
{
	Complex_d a = {1,2};
	Complex_d b = {2,1};
	Complex_d res = {0,5};
	test_result(multiplyD(a,b), res, __FUNCTION__);

	#if TIME_DISPLAY == 1
	unsigned int i;	
	a = createRandomComplexD();

	uint64_t start = __k1_read_dsu_timestamp();
	
	for(i = 0; i< 100; i++)
		multiplyD(a,b);		
	
	uint64_t end = __k1_read_dsu_timestamp();
	
	float t2 = (float) ((end - start)/(float)CHIP_FREQ);
	printf("\ttime : %.3f ns\n", t2/100);

	#endif
}

void test_divD()
{
	Complex_d a = {1,2};
	Complex_d b = {2,1};
	Complex_d res = {0.8,0.6};
	test_result(divD(a,b), res, __FUNCTION__);

	#if TIME_DISPLAY == 1
	unsigned int i;	
	a = createRandomComplexD();

	uint64_t start = __k1_read_dsu_timestamp();
	
	for(i = 0; i< 100; i++)
		divD(a,b);		
	
	uint64_t end = __k1_read_dsu_timestamp();
	
	float t2 = (float) ((end - start)/(float)CHIP_FREQ);
	printf("\ttime : %.3f ns\n", t2/100);

	#endif
}

void test_invD()
{
	Complex_d a = {1,2};
	Complex_d res = {0.2,-0.4};
	test_result(invD(a), res, __FUNCTION__);

	#if TIME_DISPLAY == 1
	unsigned int i;	
	a = createRandomComplexD();

	uint64_t start = __k1_read_dsu_timestamp();
	
	for(i = 0; i< 100; i++)
		invD(a);
	
	uint64_t end = __k1_read_dsu_timestamp();
	
	float t2 = (float) ((end - start)/(float)CHIP_FREQ);
	printf("\ttime : %.3f ns\n", t2/100);

	#endif
}

void test_multiplyByConjD()
{
	Complex_d a = {1,2};
	Complex_d res = {5,0};
	test_result(multiplyByConjD(a), res, __FUNCTION__);

	#if TIME_DISPLAY == 1
	unsigned int i;	
	a = createRandomComplexD();

	uint64_t start = __k1_read_dsu_timestamp();
	
	for(i = 0; i< 100; i++)
		multiplyByConjD(a);		
	
	uint64_t end = __k1_read_dsu_timestamp();
	
	float t2 = (float) ((end - start)/(float)CHIP_FREQ);
	printf("\ttime : %.3f ns\n", t2/100);

	#endif
}

void test_gainD()
{
	Complex_d a = {4,3};
	Complex_d b = {0,0};
	b.re = gainD(a);
	Complex_d res = {5,0};
	test_result(b, res, __FUNCTION__);

	#if TIME_DISPLAY == 1
	unsigned int i;	
	a = createRandomComplexD();

	uint64_t start = __k1_read_dsu_timestamp();
	
	for(i = 0; i< 100; i++)
		gainD(a);		
	
	uint64_t end = __k1_read_dsu_timestamp();
	
	float t2 = (float) ((end - start)/(float)CHIP_FREQ);
	printf("\ttime : %.3f ns\n", t2/100);

	#endif
}

void test_createNulComplexD()
{
	Complex_d a = {0,0};
	test_result(createNulComplexD(), a, __FUNCTION__);

	#if TIME_DISPLAY == 1
	unsigned int i;	

	uint64_t start = __k1_read_dsu_timestamp();
	
	for(i = 0; i< 100; i++)
		createNulComplexD();
	
	uint64_t end = __k1_read_dsu_timestamp();
	
	float t2 = (float) ((end - start)/(float)CHIP_FREQ);
	printf("\ttime : %.3f ns\n", t2/100);

	#endif
}

void test_createUnitaryComplexD()
{
	Complex_d a = {1,1};
	test_result(createUnitaryComplexD(), a, __FUNCTION__);

	#if TIME_DISPLAY == 1
	unsigned int i;	

	uint64_t start = __k1_read_dsu_timestamp();
	
	for(i = 0; i< 100; i++)
		createUnitaryComplexD();
	
	uint64_t end = __k1_read_dsu_timestamp();
	
	float t2 = (float) ((end - start)/(float)CHIP_FREQ);
	printf("\ttime : %.3f ns\n", t2/100);

	#endif
}

void test_createComplexD()
{
	Complex_d a = {1,2};
	test_result(createComplexD(1,2), a, __FUNCTION__);

	#if TIME_DISPLAY == 1
	unsigned int i;	

	uint64_t start = __k1_read_dsu_timestamp();
	
	for(i = 0; i< 100; i++)
		createComplexD(5.1651651,3.216584);
	
	uint64_t end = __k1_read_dsu_timestamp();
	
	float t2 = (float) ((end - start)/(float)CHIP_FREQ);
	printf("\ttime : %.3f ns\n", t2/100);

	#endif
}


