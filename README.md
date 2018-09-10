# SKA MATRIX LIB

## A complex and matrix calculation framework
These files are given without any waranty.
This framework is developped in c and aims to give a static library for INSA developers workiing for the SKA project.
The matrix functions are ALREADY multithreaded onto a single cluster of the MPPA.
Please, first transfer your data on a cluster and then call the profided functions.


### build and linking

####build
To build the lib simply copy the provided files on the MPPA in a new folder.
Open a terminal and go in this folder 
```
$ cd <Path>
$ make
```
the static library is compiled separately for the clusters and IOs (as the optimizations are different)
you can find the library in the newly created directory :

<folder>/output/lib/io/libmatrix_io.a 			for the io static lib
<folder>/output/lib/cluster/libmatrix_c.a		for the cluster static lib

####linking
##### linking on the clusters
To link the library on the cluster add <path_to_cluster_lib>/libmatrix_c.a to the "cluster-lflags" line of your makefile.

##### linking on the io
To link the library on the cluster add <path_to_io_lib>/libmatrix\_io.a to the "io\_bin-lflags" line of your makefile.


###### IMPORTANT 
The library is parallelized with openMP dont forget to add the -fopenmp option to the following lines of your makefile:

_cluster-cflags_
_cluster-lflags_
_io\_bin-cflags_
_io\_bin-lflags_

