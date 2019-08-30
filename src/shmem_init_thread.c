/* shmem_init_thread.c
 *
 * Copyright (c) 2016 U.S. Army Research Laboratory.  All rights reserved.
 * Copyright (c) 2019 Brown Deer Technology, LLC. All rights reserved.
 *
 * This file is part of the ARL OpenSHMEM Reference Implementation software
 * package. For license information, see the LICENSE file in the top level
 * directory of the distribution. 
 *
 */

#include "shmem.h"
#include "internals.h"

SHMEM_SCOPE
int shmem_init_thread(int requested, int* provided)
{
	shmem_query_thread(provided);
	shmem_init();
	return 0;
}

