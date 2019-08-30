/* shmemx.h
 *
 * Copyright (c) 2016 U.S. Army Research Laboratory.  All rights reserved.
 * Copyright (c) 2019 Brown Deer Technology, LLC. All rights reserved.
 *
 * This file is part of the ARL OpenSHMEM Reference Implementation software
 * package. For license information, see the LICENSE file in the top level
 * directory of the distribution. 
 *
 */

#ifndef _shmemx_h
#define _shmemx_h

#include "internals.h"

#ifdef __cplusplus
extern "C" {
#endif

void shmemx_set_lock_pe(volatile long* lock, int pe);
int shmemx_test_lock_pe(volatile long* lock, int pe);
void shmemx_clear_lock_pe(volatile long* lock, int pe);

void shmemx_memcpy(void* dst, const void* src, size_t nbytes);

#ifdef __cplusplus
}
#endif

#endif
