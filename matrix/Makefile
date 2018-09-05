#
# Copyright (C) 2015-2017 Kalray SA. All rights reserved.
#
ARCH ?= k1b
nb_cluster ?= 1
cluster_system ?= bare

arch := $(ARCH)
board := developer

system-name := bare
COMPILE_OPTI := -O3

cluster-cflags := -fopenmp
cluster-lflags := -fopenmp
cflags-io := -fopenmp
io-lflags := -fopenmp

matrix_io-srcs := matrixcd.c matrixd.c matrixcf.c matrixf.c complex_f.c complex_d.c
matrix_io-name := matrix_io
io-lib := matrix_io
matrix_c-srcs := matrixcd.c matrixd.c matrixcf.c matrixf.c complex_f.c complex_d.c
matrix_c-name := matrix_c
cluster-lib := matrix_c


include $(K1_TOOLCHAIN_DIR)/share/make/Makefile.kalray



