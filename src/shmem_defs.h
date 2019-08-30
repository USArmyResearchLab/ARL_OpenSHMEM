/* shmem_defs.h
 *
 * Copyright (c) 2016 U.S. Army Research Laboratory.  All rights reserved.
 * Copyright (c) 2019 Brown Deer Technology, LLC. All rights reserved.
 *
 * This file is part of the ARL OpenSHMEM Reference Implementation software
 * package. For license information, see the LICENSE file in the top level
 * directory of the distribution. 
 *
 */

#ifndef _shmem_defs_h
#define _shmem_defs_h

#define SHMEM_LOW_PRIORITY 
#define SHMEM_MAX_PES      65536
#define SHMEM_MAX_PES_LOG2 16
#define __INTERNAL_F2C_SCALE ( sizeof (long) / sizeof (int) )
#define __BCAST_SYNC_SIZE ( SHMEM_MAX_PES_LOG2 / __INTERNAL_F2C_SCALE )
#define __BARRIER_SYNC_SIZE ( SHMEM_MAX_PES_LOG2 / __INTERNAL_F2C_SCALE )
#define __REDUCE_SYNC_SIZE ( SHMEM_MAX_PES_LOG2 / __INTERNAL_F2C_SCALE + 2)
#define __REDUCE_MIN_WRKDATA_SIZE ( 16 / __INTERNAL_F2C_SCALE )
#define __SYNC_VALUE ( 0 )
#define __COLLECT_SYNC_SIZE ( SHMEM_MAX_PES_LOG2 / __INTERNAL_F2C_SCALE )
#define __ALLTOALL_SYNC_SIZE ( SHMEM_MAX_PES_LOG2 / __INTERNAL_F2C_SCALE )
#define __ALLTOALLS_SYNC_SIZE ( SHMEM_MAX_PES_LOG2 / __INTERNAL_F2C_SCALE )
#define __SYNC_SIZE ( __REDUCE_SYNC_SIZE ) // maximum of SYNC_SIZEs

#define  __THREAD_SINGLE     0
#define  __THREAD_FUNNELED   1
#define  __THREAD_SERIALIZED 2
#define  __THREAD_MULTIPLE   3

#define  __CTX_DEFAULT    0
#define  __CTX_SERIALIZED 1
#define  __CTX_PRIVATE    2
#define  __CTX_NOSTORE    4

#endif

