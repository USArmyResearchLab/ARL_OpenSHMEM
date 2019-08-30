/* shmemx_test_lock_pe.c
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

#include "spin_locks.h"

SHMEM_SCOPE
int shmemx_test_lock_pe(volatile long* lock, int pe)
{
	long* x = (long*)shmem_ptr((const void*)lock, pe);
	return __test_spin_lock(x);
}
