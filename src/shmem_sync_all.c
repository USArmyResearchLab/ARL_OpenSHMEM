/* shmem_sync_all.c
 *
 * Copyright (c) 2016 U.S. Army Research Laboratory.  All rights reserved.
 * Copyright (c) 2019 Brown Deer Technology, LLC. All rights reserved.
 *
 * This file is part of the ARL OpenSHMEM Reference Implementation software
 * package. For license information, see the LICENSE file in the top level
 * directory of the distribution. 
 *
 */

#include <stdio.h>

#include "internals.h"
#include "shmem.h"

SHMEM_SCOPE
void shmem_sync_all(void)
{
	int i;

	debug("shmem_sync_all begin");

	volatile int* pcount
		= (volatile int*)shmem_ptr((void*)&__shmem.giant_barrier_count,0);

	sem_t* pgate1 = (sem_t*)shmem_ptr((void*)&__shmem.giant_barrier_gate1,0);
	sem_t* pgate2 = (sem_t*)shmem_ptr((void*)&__shmem.giant_barrier_gate2,0);

	__giant_lock();
	if (++(*pcount) == __shmem.n_pes) {
		debug("shmem_sync_all: before sem_post on pgate1");
		for(i=0;i<__shmem.n_pes;i++) sem_post(pgate1);
		debug("shmem_sync_all: after sem_post on pgate1");
	}
	__giant_unlock();

	debug("shmem_sync_all: before sem_wait on pgate1");
	sem_wait(pgate1);
	debug("shmem_sync_all: after sem_wait on pgate1");

	__giant_lock();
	if (--(*pcount) == 0) {
		debug("shmem_sync_all: before sem_post on pgate2");
		for(i=0;i<__shmem.n_pes;i++) sem_post(pgate2);
		debug("shmem_sync_all: after sem_post on pgate2");
	}
	__giant_unlock();

	sem_wait(pgate2);

	debug("shmem_sync_all end");
}

