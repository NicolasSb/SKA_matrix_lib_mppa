ARCH ?= k1b
nb_cluster ?= 1
cluster_system ?= bare

arch := $(ARCH)
board := developer

system-name := bare
COMPILE_OPTI := -O3
VPATH=source:include
SOURCE=matrixcd.c matrixd.c matrixcf.c matrixf.c matrixi.c complex_f.c complex_d.c


cflags := -fopenmp -Wall -Wextra -Winit-self -Wswitch-default -Wfloat-equal -Wundef -Wshadow -Werror -Wuninitialized
lflags := -fopenmp

matrix_io-srcs := $(SOURCE)
matrix_io-name := matrix_io
io-lib := matrix_io
matrix_c-srcs := $(SOURCE)
matrix_c-name := matrix_c
cluster-lib := matrix_c


include $(K1_TOOLCHAIN_DIR)/share/make/Makefile.kalray



