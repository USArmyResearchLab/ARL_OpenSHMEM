dnl* shmem_api_top.m4
dnl*
dnl* Copyright (c) 2016 U.S. Army Research Laboratory.  All rights reserved.
dnl* Copyright (c) 2019 Brown Deer Technology, LLC. All rights reserved.
dnl*
dnl* This file is part of the ARL OpenSHMEM Reference Implementation software
dnl* package. For license information, see the LICENSE file in the top level
dnl* directory of the distribution.
dnl*

/*
 * Copyright (c) 2016 U.S. Army Research laboratory. All rights reserved.
 * Copyright (c) 2019 Brown Deer Technology, LLC. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 * 
 * This software was developed by Brown Deer Technology, LLC. with Copyright
 * assigned to the US Army Research laboratory as required by contract.
 */

#ifndef _shmem_h
#define _shmem_h

#include <stddef.h>
#include <stdint.h>

#include "shmem_defs.h"

#define SHMEM_VENDOR_STRING \
	"ARL OpenSHMEM Reference Implementation version 1.4"

#define SHMEM_MAJOR_VERSION             1
#define SHMEM_MINOR_VERSION             4
#define SHMEM_MAX_NAME_LEN              64

#define SHMEM_INTERNAL_F2C_SCALE        __INTERNAL_F2C_SCALE
#define SHMEM_BCAST_SYNC_SIZE           __BCAST_SYNC_SIZE
#define SHMEM_BARRIER_SYNC_SIZE         __BARRIER_SYNC_SIZE
#define SHMEM_REDUCE_SYNC_SIZE          __REDUCE_SYNC_SIZE
#define SHMEM_REDUCE_MIN_WRKDATA_SIZE   __REDUCE_MIN_WRKDATA_SIZE
#define SHMEM_SYNC_VALUE                __SYNC_VALUE
#define SHMEM_COLLECT_SYNC_SIZE         __COLLECT_SYNC_SIZE
#define SHMEM_ALLTOALL_SYNC_SIZE        __ALLTOALL_SYNC_SIZE
#define SHMEM_ALLTOALLS_SYNC_SIZE       __ALLTOALLS_SYNC_SIZE
#define SHMEM_SYNC_SIZE                 __SYNC_SIZE
#define SHMEM_THREAD_SINGLE             __THREAD_SINGLE
#define SHMEM_THREAD_FUNNELED           __THREAD_FUNNELED
#define SHMEM_THREAD_SERIALIZED         __THREAD_SERIALIZED
#define SHMEM_THREAD_MULTIPLE           __THREAD_MULTIPLE
#define SHMEM_CTX_SERIALIZED            __CTX_SERIALIZED
#define SHMEM_CTX_PRIVATE               __CTX_PRIVATE
#define SHMEM_CTX_NOSTORE               __CTX_NOSTORE
#define _SHMEM_MAJOR_VERSION            SHMEM_MAJOR_VERSION
#define _SHMEM_MINOR_VERSION            SHMEM_MINOR_VERSION
#define _SHMEM_MAX_NAME_LEN             SHMEM_MAX_NAME_LEN
#define _SHMEM_VENDOR_STRING            SHMEM_VENDOR_STRING
#define _SHMEM_INTERNAL_F2C_SCALE       SHMEM_INTERNAL_F2C_SCALE
#define _SHMEM_BCAST_SYNC_SIZE          SHMEM_BCAST_SYNC_SIZE
#define _SHMEM_BARRIER_SYNC_SIZE        SHMEM_BARRIER_SYNC_SIZE
#define _SHMEM_REDUCE_SYNC_SIZE         SHMEM_REDUCE_SYNC_SIZE
#define _SHMEM_REDUCE_MIN_WRKDATA_SIZE  SHMEM_REDUCE_MIN_WRKDATA_SIZE
#define _SHMEM_SYNC_VALUE               SHMEM_SYNC_VALUE
#define _SHMEM_COLLECT_SYNC_SIZE        SHMEM_COLLECT_SYNC_SIZE
#define _SHMEM_CMP_EQ                   SHMEM_CMP_EQ
#define _SHMEM_CMP_NE                   SHMEM_CMP_NE
#define _SHMEM_CMP_GT                   SHMEM_CMP_GT
#define _SHMEM_CMP_GE                   SHMEM_CMP_GE
#define _SHMEM_CMP_LT                   SHMEM_CMP_LT
#define _SHMEM_CMP_LE                   SHMEM_CMP_LE
#define _my_pe(...)                     shmem_my_pe(__VA_ARGS__)
#define _num_pes(...)                   shmem_n_pes(__VA_ARGS__)
#define shmalloc(...)                   shmem_malloc(__VA_ARGS__)
#define shfree(...)                     shmem_free(__VA_ARGS__)
#define shrealloc(...)                  shmem_realloc(__VA_ARGS__)
#define shmemalign(...)                 shmem_align(__VA_ARGS__)
#define start_pes(...)                  shmem_init()
#define shmem_clear_cache_inv(...)      do{}while(0)
#define shmem_set_cache_inv(...)        do{}while(0)
#define shmem_clear_cache_line_inv(...) do{}while(0)
#define shmem_set_cache_line_inv(...)   do{}while(0)
#define shmem_udcflush(...)             do{}while(0)
#define shmem_udcflush_line(...)        do{}while(0)

#define shmem_fence(...) shmem_quiet(__VA_ARGS__)
#define shmem_ctx_fence(ctx) shmem_ctx_quiet(ctx)

#define shmem_putmem_nbi(...) shmem_put8_nbi(__VA_ARGS__)
#define shmem_getmem_nbi(...) shmem_get8_nbi(__VA_ARGS__)
#define shmem_putmem(...) shmem_put8(__VA_ARGS__)
#define shmem_getmem(...) shmem_get8(__VA_ARGS__)
#define shmem_ctx_putmem_nbi(ctx,...) shmem_put8_nbi(__VA_ARGS__)
#define shmem_ctx_getmem_nbi(...) shmem_get8_nbi(__VA_ARGS__)
#define shmem_ctx_putmem(ctx,...) shmem_put8(__VA_ARGS__)
#define shmem_ctx_getmem(ctx,...) shmem_get8(__VA_ARGS__)

#ifdef __cplusplus
extern "C" {
#endif

enum shmem_cmp_constants
{
	SHMEM_CMP_EQ = 0,
	SHMEM_CMP_NE,
	SHMEM_CMP_GT,
	SHMEM_CMP_GE,
	SHMEM_CMP_LT,
	SHMEM_CMP_LE
};

typedef long shmem_ctx_t;

extern int __shmem_my_pe;
extern int __shmem_n_pes;
extern const shmem_ctx_t SHMEM_CTX_DEFAULT;

static inline __attribute__((always_inline)) int shmem_my_pe(void)
{ return __shmem_my_pe; }

static inline __attribute__((always_inline)) int shmem_n_pes(void)
{ return __shmem_n_pes; }

static inline __attribute__((always_inline)) int shmem_pe_accessible(int pe)
{ return ((pe >= 0 && pe < __shmem_n_pes) ? 1 : 0); }

//static int shmem_addr_accessible(const void* addr, int pe)
//{
//	return (
//		(shmem_pe_accessible(pe)
//			&& (intptr_t)addr < (intptr_t)shmemx_sbrk(0)
//		) ? 1 : 0
//	);
//}

void shmem_init(void);
void shmem_finalize(void);
int shmem_init_thread(int requested, int *provided);
void shmem_query_thread(int* provided);
void shmem_info_get_name(char* name);
void shmem_info_get_version(int* major, int* minor);
void* shmem_malloc(size_t size);
void* shmem_calloc(size_t count, size_t size);
void* shmem_realloc(void* ptr, size_t size);
void* shmem_align(size_t alignment, size_t size);
void shmem_free(void* ptr);
void* shmem_ptr(const void* dest, int pe);
int shmem_addr_accessible(const void* addr, int pe);
void shmem_barrier(int PE_start, int logPE_stride, int PE_size, long *pSync);
void shmem_barrier_all(void);
void shmem_quiet(void);
void shmem_sync(int pe_start, int log_pe_stride, int pe_size, long *psync);
void shmem_sync_all(void);
void shmem_clear_lock(long* lock);
void shmem_set_lock(long* lock);
int shmem_test_lock(long* lock);
int shmem_ctx_create(long options, shmem_ctx_t *ctx);
void shmem_ctx_destroy(shmem_ctx_t *ctx);
void shmem_ctx_quiet(shmem_ctx_t ctx);

void shmem_global_exit(int status);


