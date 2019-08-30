/* shmemx_memcpy.c
 *
 * Copyright (c) 2016 U.S. Army Research Laboratory.  All rights reserved.
 * Copyright (c) 2019 Brown Deer Technology, LLC. All rights reserved.
 *
 * This file is part of the ARL OpenSHMEM Reference Implementation software
 * package. For license information, see the LICENSE file in the top level
 * directory of the distribution. 
 *
 */

#include <string.h>
#include <sys/mman.h>

#include "internals.h"
#include "shmem.h"

void shmemx_memcpy(void* dst, const void* src, size_t nbytes)
{ 
	memcpy(dst,src,nbytes);
	msync(dst,nbytes,MS_SYNC|MS_INVALIDATE);
}

