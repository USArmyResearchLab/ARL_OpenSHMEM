/* shmem_sync.c
 *
 * Copyright (c) 2016 U.S. Army Research Laboratory.  All rights reserved.
 * Copyright (c) 2019 Brown Deer Technology, LLC. All rights reserved.
 *
 * This file is part of the ARL OpenSHMEM Reference Implementation software
 * package. For license information, see the LICENSE file in the top level
 * directory of the distribution. 
 *
 */

#include <unistd.h>

#include "internals.h"
#include "shmem.h"

SHMEM_SCOPE
void shmem_sync(int pe_start, int log_pe_stride, int pe_size, long *psync)
{
	debug("[%d] shmem_sync begin",__shmem.my_pe);

	int pe_size_stride = pe_size << log_pe_stride;
	int pe_end = pe_size_stride + pe_start;

	int c, r;
	for (c = 0, r = (1 << log_pe_stride); r < pe_size_stride; c++, r <<= 1)
	{
		int to = __shmem.my_pe + r;
		if (to >= pe_end) to -= pe_size_stride;

		volatile long* lock = (volatile long*)(psync + c);
		long * remote_lock = (long*)shmem_ptr((void*)lock, to);

		__set_spin_lock(remote_lock);

		__clear_spin_lock(lock);
	}

	debug("[%d] shmem_sync end",__shmem.my_pe);
}

