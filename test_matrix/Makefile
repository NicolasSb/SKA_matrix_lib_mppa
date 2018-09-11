#
# Copyright (C) 2015-2017 Kalray SA. All rights reserved.
#
ARCH ?= k1b
nb_cluster ?= 1
time_display ?= 0
cluster_system ?= bare

arch := $(ARCH)
board := developer

VPATH=include:test/test_head:test/test_src

SRC = test_matrixf.c test_matrixd.c test_matrixi.c test_matrixcf.c test_matrixcd.c

system-name := bare
COMPILE_OPTI := -O3

cflags := -Wall -Wextra -Winit-self -Wswitch-default -Wfloat-equal -Wundef -Wshadow -Werror -Wuninitialized
# Cluster rules
cluster-bin := cluster_bin
cluster-system := $(cluster_system)
cluster_bin-srcs := cluster.c $(SRC)
cluster-cflags := -g -DNB_CLUSTER=$(nb_cluster) -DTIME_DISPLAY=$(time_display) ${COMPILE_OPTI} -mhypervisor -I . -std=gnu99 -fopenmp
cluster-lflags := -g -mhypervisor -Wl,--defsym=USER_STACK_SIZE=0x2000 -Wl,--defsym=KSTACK_SIZE=0x1000 ../matrix/output/lib/cluster/libmatrix_c.a -fopenmp
ifeq ($(cluster_system), bare)
cluster-lflags += -lvbsp -lutask -lmppa_remote -lmppa_async -lmppa_request_engine -lmppapower -lmppanoc -lmpparouting -Wl,--defsym=_LIBNOC_DISABLE_FIFO_FULL_CHECK=0
endif
ifeq ($(cluster_system), nodeos)
cluster-lflags += -pthread -lmppa_remote -lmppa_async -lmppa_request_engine -lmppapower -lmppanoc -lmpparouting -Wl,--defsym=_LIBNOC_DISABLE_FIFO_FULL_CHECK=0
endif

io-bin := io_bin
io_bin-srcs := io_main.c $(SRC)
io_bin-cflags := -DNB_CLUSTER=$(nb_cluster) -std=gnu99 -g ${COMPILE_OPTI} -DTIME_DISPLAY=$(time_display) -DMPPA_TRACE_ENABLE -mhypervisor
io_bin-lflags := -lvbsp -lmppa_remote -lmppa_async -lmppa_request_engine -lpcie_queue -lutask  -lmppapower -lmppanoc -lmpparouting -mhypervisor -Wl,--defsym=_LIBNOC_DISABLE_FIFO_FULL_CHECK=0 ../matrix/output/lib/io/libmatrix_io.a -fopenmp

mppa-bin := multibin_bin
multibin_bin-objs = io_bin cluster_bin

host-bin := host_bin
host_bin-srcs := host_main.c
host-cflags := ${COMPILE_OPTI}
host-lflags := -lpthread -lrt -lmppa_remote -lpcie
host-bin    := host_bin

# clean hooks will be called with clean
clean-hooks := clean2

include $(K1_TOOLCHAIN_DIR)/share/make/Makefile.kalray

run_sim: all
	$(K1_TOOLCHAIN_DIR)/bin/k1-mppa --march=bostan -- ${O}/bin/multibin_bin.mpk

run_pcie_sim: all
	$(K1_TOOLCHAIN_DIR)/bin/k1-pciesim-runner ./${O}/bin/host_bin ./${O}/bin/multibin_bin.mpk io_bin


ifeq (${board}, emb01)
# When running on emb01, the following variable must be defined.
# k1-remote-runner will also need a password. We STRONGLY recommend
# to do not use --passwd option for that.
# Instead, please consider providing the proper user ssh public key
# from developer to the EMB01 (you may use ssh-copyid standard tool).
# It will allow connecting without providing a password.
EMB01_ADDRESS := $(shell echo $$EMB01_ADDRESS)
EMB01_USER := $(shell echo $$EMB01_USER)

run_jtag: all
	@[ "" != "${EMB01_ADDRESS}" ]  || ( echo "EMB01_ADDRESS not defined"  ; false)
	@[ "" != "${EMB01_USER}" ]     || ( echo "EMB01_USER not defined"     ; false)
	cd $(BIN_DIR);k1-remote-runner --remote ${EMB01_ADDRESS} --user ${EMB01_USER} --host_exec host_bin --multibinary multibin_bin.mpk -- ./host_bin multibin_bin.mpk io_bin

run_hw: all
	@[ "" != "${EMB01_ADDRESS}" ]  || ( echo "EMB01_ADDRESS not defined"  ; false)
	@[ "" != "${EMB01_USER}" ]     || ( echo "EMB01_USER not defined"     ; false)
	cd $(BIN_DIR) 
else

run_jtag: all
	$(K1_TOOLCHAIN_DIR)/bin/k1-jtag-runner --multibinary=./${O}/bin/multibin_bin.mpk --exec-multibin=IODDR0:io_bin

run_hw: all
	rm -f C*
	./${O}/bin/host_bin ./${O}/bin/multibin_bin.mpk io_bin

endif

clean2:
	rm -fr C*_out_data
