/* shmem_barrier.c
 *
 * Copyright (c) 2016 U.S. Army Research Laboratory.  All rights reserved.
 * Copyright (c) 2019 Brown Deer Technology, LLC. All rights reserved.
 *
 * This file is part of the ARL OpenSHMEM Reference Implementation software
 * package. For license information, see the LICENSE file in the top level
 * directory of the distribution. 
 *
 */

#include "internals.h"
#include "shmem.h"


SHMEM_SCOPE 
void shmem_barrier(int PE_start, int logPE_stride, int PE_size, long *pSync)
{
	shmem_quiet();
	shmem_sync(PE_start, logPE_stride, PE_size, pSync);
}

