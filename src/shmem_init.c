/* shmem_init.c
 *
 * Copyright (c) 2016 U.S. Army Research Laboratory.  All rights reserved.
 * Copyright (c) 2019 Brown Deer Technology, LLC. All rights reserved.
 *
 * This file is part of the ARL OpenSHMEM Reference Implementation software
 * package. For license information, see the LICENSE file in the top level
 * directory of the distribution. 
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/time.h>
#include <semaphore.h>

#include "shmem.h"
#include "internals.h"

int __shmem_my_pe = -1;
int __shmem_n_pes = -1;

shmem_internals_t __shmem = { 0 };

const shmem_ctx_t SHMEM_CTX_DEFAULT = __CTX_DEFAULT;

//SHMEM_SCOPE void SHMEM_INLINE
//__shmem_dissemination_barrier_init(void)
//{
//	int c, r;
//	for (c = 0, r = 1; r < __shmem.n_pes; c++, r <<= 1)
//	{
//		int to = __shmem.my_pe + r;
//		if (to >= __shmem.n_pes) to -= __shmem.n_pes;
//		long* lock = (long*)(__shmem.barrier_sync + c);
//		long* remote_lock = (long*)shmem_ptr((void*)lock, to);
//		__shmem.barrier_psync[c] = remote_lock;
//	}
//	debug("pe %d barrier_psync[0] %p",__shmem.my_pe,__shmem.barrier_psync[0]);
//}


static void find_data_pages(void** ret_map_start, size_t* ret_map_size);
static int remap_pages(int fd, off_t offset, void* map_start, size_t map_size);
static void* shm_alloc(size_t size, unsigned int gkey, int id, int* ret_fd);


SHMEM_SCOPE
void shmem_init(void)
{
	int run_id;
	int my_pe;
	int n_pes;
	int n_pes_log2;
	size_t heap_size;

	debug("shmem_init begin");

	bzero(&__shmem,sizeof(shmem_internals_t));

	const char* env_var_run_id = getenv("SHMEM_RUN_ID");
	const char* env_var_my_pe = getenv("SHMEM_MY_PE");
	const char* env_var_n_pes = getenv("SHMEM_N_PES");
	const char* env_var_heap_size = getenv("SHMEM_SYMMETRIC_HEAP_SIZE");

	if ( env_var_run_id && env_var_my_pe && env_var_n_pes ) {
		run_id = atoi(env_var_run_id);
		my_pe = atoi(env_var_my_pe);
		n_pes = atoi(env_var_n_pes);
	} else {
		run_id = getpid();
		my_pe = 0;
		n_pes = 1;
		fprintf(stderr,"shmem warning: SHMEM_RUN_ID, SHMEM_MY_PE, or"
			" SHMEM_N_PES not set. Defaulting to serial execution\n");
	}

	if (env_var_heap_size) {
		size_t len = strnlen(env_var_heap_size,1024);
		unsigned int x = 1;
		if (env_var_heap_size[len-1]=='K') {
			x = 1024;
		} else if (env_var_heap_size[len-1]=='M') {
			x = 1024*1024;
		}
		heap_size = x * atoi(env_var_heap_size);
	} else {
		heap_size = 128*1024*1024;
		if (my_pe == 0) {
			fprintf(stderr,"shmem warning: SHMEM_SYMMETRIC_HEAP_SIZE not set."
				" Defaulting to %ld bytes\n", heap_size);
		}
	}

	debug("run_id=%d\nmy_pe=%d\nn_pes=%d\nheap_size=%ld\n",
		run_id, my_pe, n_pes, heap_size);

	if ( n_pes < 0 || n_pes > SHMEM_MAX_PES ) {
		fprintf(stderr,"shmem error: n_pes out of range: %d\n",n_pes);
		exit(-1);
	}

	if ( my_pe < 0 || my_pe >= n_pes ) {
		fprintf(stderr,"shmem error: my_pe out of range: %d\n",my_pe);
		exit(-1);
	}


	// log2_ceil of n_pes precalculated once
	n_pes_log2 = 0;
	unsigned int x = n_pes - 1;
	while (x > 0) {
		n_pes_log2++;
		x >>= 1;
	}

	__shmem.run_id = run_id;
	__shmem_my_pe = __shmem.my_pe = my_pe;
	__shmem_n_pes = __shmem.n_pes = n_pes;
	__shmem.n_pes_log2 = n_pes_log2;
	__shmem.heap_size = heap_size;


	__shmem.giant_barrier_count = 0;
	sem_init(&__shmem.giant_mutex,1,0);
	sem_init(&__shmem.giant_barrier_gate1,1,0);
	sem_init(&__shmem.giant_barrier_gate2,1,0);

	sem_init(&__shmem.sync_mutex,1,0);


	/* First we need to remap .data and .bss into a shm allocation. */

	int data_fd;
	void* data_start;
	size_t data_size;

	find_data_pages(&data_start,&data_size);

	intptr_t data_shm_base 
		= (intptr_t)shm_alloc(n_pes*data_size,run_id,0,&data_fd);
	
	munmap((void*)(data_shm_base+my_pe*data_size), data_size);

	remap_pages(data_fd, my_pe*data_size, data_start, data_size);

	__shmem.data_fd = data_fd;
	__shmem.data_base = (intptr_t)data_start;
	__shmem.data_size = data_size;
	__shmem.data_shm_base = data_shm_base;

	debug("symmetric data:\n\tdata_fd=%d\n\tdata_base=%p\n\tdata_size=%ld\n"
		"\tdata_shm_base=%p\n",data_fd,data_start,data_size,(void*)data_shm_base);


	/* we now have enough information to setup the symmetric heap -DAR */

	debug("shmem_init setup shared memory region");

	intptr_t heap_shm_base = (intptr_t)shm_alloc(n_pes*heap_size,run_id,1,0);

	debug("shmem_init shm_alloc returned %p",(void*)heap_shm_base);

	__shmem.heap_shm_base = heap_shm_base;

	debug("__shmem.heap_shm_base %p\n",(void*)__shmem.heap_shm_base);


	int c, r;
	for (c = 0, r = 1; r < __shmem.n_pes; c++, r <<= 1)
	{
		int to = __shmem.my_pe + r;
		if (to >= __shmem.n_pes) to -= __shmem.n_pes;
		long* lock = (long*)(__shmem.barrier_sync + c);
		long* remote_lock = (long*)shmem_ptr((void*)lock, to);
		__shmem.barrier_psync[c] = remote_lock;
	}
	debug("pe %d barrier_psync[0] %p",__shmem.my_pe,__shmem.barrier_psync[0]);


	__shmem.local_mem_base 
		= __shmem.heap_shm_base + my_pe * heap_size + getpagesize();

	__shmem.free_mem = __shmem.local_mem_base;

	__shmem.xmem0 = xmalloc_init( 
		(void*)__shmem.local_mem_base, heap_size - getpagesize(), 0 );

	debug("[%d] before barrier\n",my_pe);

	__shmem.p_giant_mutex = (sem_t*)shmem_ptr((void*)&__shmem.giant_mutex,0);

	if (my_pe == 0) sem_post(&__shmem.giant_mutex);
	sem_post(&__shmem.sync_mutex);


	shmem_sync_all();


	debug("[%d] after barrier\n",my_pe);


	if (__shmem.n_pes == 1) return;

	if (!__shmem.my_pe) {
		debug("master set psync to 1 @ %p",__shmem.barrier_psync[0]);
		*__shmem.barrier_psync[0] = 1;
	}

	debug("spin on sync at %p",__shmem.barrier_sync);
	while(*__shmem.barrier_sync == SHMEM_SYNC_VALUE);

	if (__shmem.my_pe) {
		debug("slave set psync to 1 @ %p",__shmem.barrier_psync[0]);
		*__shmem.barrier_psync[0] = 1;
	}

	debug("set sync to SHMEM_SYNC_VALUE @ %p",__shmem.barrier_sync);
	*__shmem.barrier_sync = SHMEM_SYNC_VALUE;

	debug("shmem_init done");

	
}


SHMEM_SCOPE
void shmem_finalize(void)
{
   debug("[%d] shmem_finalize complete\n",__shmem.my_pe);
}


#define SHM_ALLOC_SHMOBJ_TAG "shmem_init"
#define SHM_ALLOC_TIMEOUT 10

static void* shm_alloc(size_t size, unsigned int gkey, int id, int* ret_fd)
{
	if (gkey == 0) return(0);

	int fd;
	void* p0;

	struct timeval t0,t1;
	int timeout;

	char shmobj0[64];

	snprintf(shmobj0,64,"/" SHM_ALLOC_SHMOBJ_TAG ".%d.%d",gkey,id);

	if (__shmem.my_pe == 0) {

		fd = shm_open( shmobj0,O_RDWR|O_CREAT|O_EXCL,0);

		if (fd < 0) {
			fprintf(stderr,"shmem error: master shm_open failed\n");
			exit(-1);
		}

		ftruncate(fd,size);

		p0 = mmap( 0,size,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);

		if (!p0) {
			close(fd);
			if (ret_fd) *ret_fd = -1;
			return(0);
		}

		fchmod(fd,S_IRUSR|S_IWUSR);

	} else {

		timeout = 0;
		gettimeofday(&t0,0);
		t0.tv_sec += SHM_ALLOC_TIMEOUT;

		do {
			fd = shm_open( shmobj0, O_RDWR,0);
			gettimeofday(&t1,0);
			timeout = (t1.tv_sec > t0.tv_sec && t1.tv_usec > t0.tv_usec)? 1:0;
		} while( (fd < 0) && !timeout );
	
		if (timeout) {
			errno = ENOKEY;
			if(ret_fd) *ret_fd = -1;
			return(0);
		}

		ftruncate(fd,size);

		p0 = mmap( 0,size,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);

		if (!p0) {
			close(fd);
			if (ret_fd) *ret_fd = -1;
			return(0);
		}

	}

	if (ret_fd) *ret_fd = fd;

	return(p0);	

}

extern void* end;
extern void* __data_start;

static void find_data_pages( void** ret_map_start, size_t* ret_map_size )
{

	long page_size = getpagesize();
	long page_mask = ~(page_size-1);
	intptr_t map_start = ((intptr_t)&__data_start) & page_mask;
	size_t map_size = ((intptr_t)&end) - map_start;

	if ((map_size & page_mask) != map_size) 
		map_size = (map_size & page_mask) + page_size;

	if (ret_map_start) *ret_map_start = (void*)map_start;
	if (ret_map_size) *ret_map_size = map_size;

}


static int remap_pages(int fd, off_t offset, void* map_start, size_t map_size)
{

	int i;

	// allocate memory to backup pages containing .data and .bss sections
	void* buf = mmap(0,map_size,PROT_READ|PROT_WRITE,
		MAP_PRIVATE|MAP_ANONYMOUS,-1,0);

	if (!buf) return(-1);

	// copy pages and DO NOT use memcpy
	long long* q = (long long*)map_start;
	long long* p = (long long*)buf;
	for(i=0;i<map_size/sizeof(long long);i++,p++,q++) *p = *q;

	// create a new mapping to hold the data and bss sections
	void* map_ptr = mmap(map_start,map_size,
		PROT_READ|PROT_WRITE,MAP_SHARED|MAP_FIXED,fd,offset);

	if (!map_ptr) return(-1);

	// XXX This is where things are a bit scary, the data sections of entire
	// XXX program have been cleared and must be restored from our backup.
	// XXX At this point we should probably have disabled interrupts and
	// XXX done just about everything else we can do to create a critical
	// XXX section.  -DAR

	// copy pages back into place and DO NOT use memcpy
	q = (long long*)buf;
	p = (long long*)map_ptr;
	for(i=0;i<map_size/sizeof(long long);i++,p++,q++) *p = *q;	

	return(0);
}

