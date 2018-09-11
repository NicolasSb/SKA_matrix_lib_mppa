/**
 * Copyright (C) 2016-2017 Kalray SA.
 *
 * All rights reserved.
 *
 * An application is implemented on a single cluster
 *     and on one core
 * 
 * The core reads an input and writes datas in in output file
 *
 * Exercise 3 / Training MPPA Distributed C/C++ Programming
 */

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
#include "test/test_head/test_matrixi.h"
#include "test/test_head/test_matrixf.h"
#include "test/test_head/test_matrixd.h"
#include "test/test_head/test_matrixcf.h"
#include "test/test_head/test_matrixcd.h"

#define N_CORES 1
#define NB_CLIENTS (N_CORES-1)
#define FETCH_LEN (4)
#define OFFSET 1000

int tmp[N_CORES][FETCH_LEN];
pthread_barrier_t barrier;

/* routine to be executed on each PE */
void* task_routine(void *arg __attribute__((unused)))
{

	pthread_barrier_wait(&barrier);

	testAllI();
	testAllF();
	testAllD();
	testAllCF();
	testAllCD();

	pthread_barrier_wait(&barrier);

	return NULL;
}

/* Main executed on PE0 */
int main(int argc, const char **argv)
{
	mppa_rpc_client_init();
	mppa_async_init();
	mppa_remote_client_init();
	/* don't do anything before library initialisation */

	int cid = __k1_get_cluster_id();

	if(cid == 0){
		int i;
		for(i=0;i<argc;i++){
			printf("[Cluster %d] Arg %d: %s\n", __k1_get_cluster_id(), i, argv[i]);
		}
	}

	mppa_rpc_barrier_all();	/* synchronize all PE0 of all booted cluster */

	pthread_barrier_init(&barrier, NULL, N_CORES);

	/*int i;
	
	pthread_t utask[NB_CLIENTS];

	for (i = 0; i < NB_CLIENTS; i++) {
		pthread_create(&utask[i], NULL, task_routine, NULL);
	}

	task_routine(NULL);

	for (i = 0; i < NB_CLIENTS; i++) {
		pthread_join(utask[i], NULL);
	}
*/
	task_routine(NULL);

	mppa_async_final();
	return 0;
}
