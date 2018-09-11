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
#include "include/matrixf.h"
#include <unistd.h>
#include <mppa_power.h>
#include <mppa_routing.h>
#include <mppa_remote.h>
#include <mppa_rpc.h>
#include <mppa_async.h>
#include <assert.h>
#include <utask.h>
#include <HAL/hal/board/boot_args.h>



#define NB_ELM 10

int main(__attribute__((unused)) int argc, const char **argv) {

	mppadesc_t pcie_fd = 0;
	if (__k1_spawn_type() == __MPPA_PCI_SPAWN) {
		pcie_fd = pcie_open(0);
		pcie_queue_init(pcie_fd);
		pcie_register_console(pcie_fd, stdin, stdout);
	}

	mppa_rpc_server_init(1, 0, NB_CLUSTER);
	mppa_async_server_init();
	mppa_remote_server_init(pcie_fd, NB_CLUSTER);
	mppa_remote_server_enable_scall();

	for(int i=0;i<NB_CLUSTER;i++){
		if (mppa_power_base_spawn(i, "cluster_bin", argv, NULL, MPPA_POWER_SHUFFLING_ENABLED) == -1)
			printf("# [IODDR0] Fail to Spawn cluster %d\n", i);
	}

	printf("[IODDR0] Starts %d Cluster(s)\n", NB_CLUSTER);

	utask_t t;
	utask_create(&t, NULL, (void*)mppa_rpc_server_start, NULL);

	int status = 0;
	for(unsigned i=0;i<NB_CLUSTER;i++){
		int ret;
		if (mppa_power_base_waitpid(i, &ret, 0) < 0) {
			printf("# [IODDR0] Waitpid failed on cluster %d\n", i);
		}
		status += ret;
	}
	if(status != 0)
		return -1;



	printf("[IODDR0] Goodbye\n");



	/* Send an exit message on pcie interface */
	if (__k1_spawn_type() == __MPPA_PCI_SPAWN) {
		int remote_status;
		pcie_queue_barrier(pcie_fd, 0, &remote_status);
		pcie_unregister_console(pcie_fd);
		pcie_queue_exit(pcie_fd, 0, NULL);
	}
	return 0;
}
