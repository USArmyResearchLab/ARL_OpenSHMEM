/* shmem_realloc.c
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

void* shmem_realloc(void* ptr, size_t size)
{
	return xrealloc( __shmem.xmem0, ptr, size );
}

