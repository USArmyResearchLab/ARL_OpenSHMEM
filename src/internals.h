/* internals.h
 *
 * Copyright (c) 2016 U.S. Army Research Laboratory.  All rights reserved.
 * Copyright (c) 2019 Brown Deer Technology, LLC. All rights reserved.
 *
 * This file is part of the ARL OpenSHMEM Reference Implementation software
 * package. For license information, see the LICENSE file in the top level
 * directory of the distribution. 
 *
 */

#ifndef _x86_64_internal_h
#define _x86_64_internal_h

#include <stdint.h>
#include <sys/types.h>
#include <semaphore.h>

#include "spin_locks.h"
#include "xmalloc.h"
#include "shmemx.h"

#ifdef ENABLE_DEBUG
#include <stdio.h>
#define debug(msg,...) do{printf("%d# " msg "\n",__shmem_my_pe,##__VA_ARGS__); fflush(stdout);}while(0)
#else
#define debug(msg,...)
#endif

//#include "shmem_mman.h"
#define SHMEM_LOW_PRIORITY 
//#define shmemx_brk(ptr) __shmemx_brk(ptr)
//#define shmemx_sbrk(size) __shmemx_sbrk(size)

#define SHMEM_INLINE       inline __attribute__((__always_inline__))
#define SHMEM_SCOPE 

/*
#define SHMEM_MAX_PES      65536
#define SHMEM_MAX_PES_LOG2 16

#define __INTERNAL_F2C_SCALE        ( sizeof (long) / sizeof (int) )
#define __BCAST_SYNC_SIZE           ( SHMEM_MAX_PES_LOG2 / __INTERNAL_F2C_SCALE )
#define __BARRIER_SYNC_SIZE         ( SHMEM_MAX_PES_LOG2 / __INTERNAL_F2C_SCALE )

//#define __REDUCE_SYNC_SIZE          ( SHMEM_MAX_PES_LOG2 / __INTERNAL_F2C_SCALE + 2)
#define __REDUCE_SYNC_SIZE          (2*sizeof(sem_t))

#define __REDUCE_MIN_WRKDATA_SIZE   ( 16 / __INTERNAL_F2C_SCALE )
#define __SYNC_VALUE                ( 0 )
#define __COLLECT_SYNC_SIZE         ( SHMEM_MAX_PES_LOG2 / __INTERNAL_F2C_SCALE )
#define __ALLTOALL_SYNC_SIZE        ( SHMEM_MAX_PES_LOG2 / __INTERNAL_F2C_SCALE )
#define __ALLTOALLS_SYNC_SIZE       ( SHMEM_MAX_PES_LOG2 / __INTERNAL_F2C_SCALE )
#define __SYNC_SIZE                 ( __REDUCE_SYNC_SIZE ) // maximum of SYNC_SIZEs

#define  __THREAD_SINGLE     0
#define  __THREAD_FUNNELED   1
#define  __THREAD_SERIALIZED 2
#define  __THREAD_MULTIPLE   3

#define  __CTX_DEFAULT    0
#define  __CTX_SERIALIZED 1
#define  __CTX_PRIVATE    2
#define  __CTX_NOSTORE    4
*/

#include "shmem_defs.h"

#define __NUM_OF_ATOMIC_LOCKS 15

#if __cplusplus
extern "C" {
#endif

typedef struct {
	int run_id;
	int my_pe;
	int n_pes;
	int n_pes_log2;
	union {
		volatile long lock_atomic[__NUM_OF_ATOMIC_LOCKS];
		struct {
			volatile long lock_atomic_int;
			volatile long lock_atomic_long;
			volatile long lock_atomic_longlong;
			volatile long lock_atomic_uint;
			volatile long lock_atomic_ulong;
			volatile long lock_atomic_ulonglong;
			volatile long lock_atomic_int32;
			volatile long lock_atomic_int64;
			volatile long lock_atomic_uint32;
			volatile long lock_atomic_uint64;
			volatile long lock_atomic_size;
			volatile long lock_atomic_ptrdiff;
			volatile long lock_atomic_float;
			volatile long lock_atomic_double;
			volatile long lock_receive_finished;
		};
	};
	volatile unsigned char csrc0;
	volatile unsigned char csrc1;
	unsigned char volatile * volatile cdst0;
	unsigned char volatile * volatile cdst1;

	sem_t giant_mutex;
	sem_t* p_giant_mutex;

	int giant_barrier_count;
	sem_t giant_barrier_gate1;
	sem_t giant_barrier_gate2;

	sem_t sync_mutex;

	int data_fd;
	size_t data_size;
	intptr_t data_base;
	intptr_t data_shm_base;

	size_t heap_size;
	intptr_t heap_shm_base;
	intptr_t local_mem_base;
	intptr_t free_mem;

	volatile long barrier_sync[__BARRIER_SYNC_SIZE];
	long* barrier_psync[__BARRIER_SYNC_SIZE];

	int msg;

	long long pad_alignment_wrk[__REDUCE_MIN_WRKDATA_SIZE];
	long pad_alignment_sync[__REDUCE_SYNC_SIZE];
	long long pad_alignment;

	xmem_t* xmem0;

} shmem_internals_t;

extern shmem_internals_t __shmem;
extern int __shmem_my_pe;
extern int __shmem_n_pes;

SHMEM_SCOPE long* __shmem_lock_ptr (const long* p);
SHMEM_SCOPE void __shmem_clear_lock (volatile long* x);
SHMEM_SCOPE void __shmem_set_lock (volatile long* x);
SHMEM_SCOPE int __shmem_test_lock (volatile long* x);

#if __cplusplus
}
#endif

#define __giant_lock() do { \
	debug("%d giant_lock: atempt lock",__LINE__); \
	sem_wait(__shmem.p_giant_mutex); \
	debug("%d giant_lock: lock acquired",__LINE__); \
} while(0)

#define __giant_unlock() do { \
	debug("%d giant_unlock: release lock\n",__LINE__); \
	sem_post(__shmem.p_giant_mutex); \
	debug("%d giant_unlock: lock released\n",__LINE__); \
} while(0)

#define __lock_mutex(mtx) do { \
	debug("%d lock_mutex: atempt lock",__LINE__); \
	sem_wait(mtx); \
	debug("%d lock_mutex: lock acquired",__LINE__); \
} while(0)

#define __unlock_mutex(mtx) do { \
	debug("%d unlock_mutex: release lock\n",__LINE__); \
	sem_post(mtx); \
	debug("%d unlock_mutex: lock released\n",__LINE__); \
} while(0)

#define __and(a,b) ((a)&(b))
#define __or(a,b) ((a)|(b))
#define __xor(a,b) ((a)^(b))
#define __sum(a,b) ((a)+(b))
#define __prod(a,b) ((a)*(b))
#define __max(a,b) (((a)<(b))?(b):(a))
#define __min(a,b) (((a)<(b))?(a):(b))

#endif
