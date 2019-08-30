/* shmem_ptr.c
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
void* shmem_ptr(const void* dest, int pe)
{

	if (pe == __shmem.my_pe) return (void*)dest;

	void* remote;

	if ((intptr_t)dest < __shmem.heap_shm_base) {

		remote = (void*)((intptr_t)dest 
			+ (__shmem.data_shm_base - __shmem.data_base) + pe*__shmem.data_size);

	} else {

		remote = (void*)((intptr_t)dest + (pe - __shmem.my_pe)*__shmem.heap_size);

	}

	return remote;
}

