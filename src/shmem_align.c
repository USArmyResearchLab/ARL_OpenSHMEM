/* shmem_align.c
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

void* __attribute__((malloc)) shmem_align(size_t alignment, size_t size)
{ 
	void* ptr = 0;
	xposix_memalign( __shmem.xmem0, &ptr, alignment, size );
	return ptr;
}

