ARL OpenSHMEM
======

OpenSHMEM is a standardized interface to enable portable applications for
partitioned global address space (PGAS) architectures. This repository includes
the *ARL OpenSHMEM* implementation for optimized one-sided communication,
atomic, and collective operations for multicore and manycore processors (see
[./src](./src)) and test codes (see [./test](./test)).

The OpenSHMEM implementation is based on the [OpenSHMEM 1.4
Specification](http://openshmem.org/site/sites/default/site\_files/OpenSHMEM-1.4.pdf)

This software was developed by the US Army Research Laboratory, Computational
Sciences Division, Computing Architectures Branch, and Brown Deer Technology.

Prerequisites
-----

Presently, a POSIX-compliant development environment is needed, including GCC,
GNU Make, M4, Bash (Unix shell), and Perl tools.  No external support libraries
are required (i.e. no network interfaces) as the target platforms are multi-core
processors.

Using
-----

From the top level directory, type `make` to build, or `make check` to build
and check.

In [./test](./test), executing `make run` will execute most test codes with a
default number of PEs and `make check` will run all of the test codes with
various parameters while checking for correctness. There is no installation,
but you should link to the _libshmem.a_ static library and use the appropriate
include path for _shmem.h_ found within [./src](./src) or subdirectories.

Citation
-----

This software was originally developed for the Adapteva Epiphany Coprocessor
and described in the following paper:

*J. Ross and D. Richie, "An OpenSHMEM Implementation for the Adapteva Epiphany
Coprocessor," OpenSHMEM and Related Technologies. Enhancing OpenSHMEM for
Hybrid Environments, vol. 10007, pp. 146-159, Dec. 2016,
doi:10.1007/978-3-319-50995-2_10*
