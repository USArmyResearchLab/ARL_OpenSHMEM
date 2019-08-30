dnl* shmem_fcollect_x.m4
dnl*
dnl* Copyright (c) 2016 U.S. Army Research Laboratory.  All rights reserved.
dnl* Copyright (c) 2019 Brown Deer Technology, LLC. All rights reserved.
dnl*
dnl* This file is part of the ARL OpenSHMEM Reference Implementation software
dnl* package. For license information, see the LICENSE file in the top level
dnl* directory of the distribution.
dnl*

define(decl_shmem_fcollect_x,
`void shmem_fcollect$1(void *dest, const void *source, size_t nelems, 
	int pe_start, int log_pe_stride, int pe_size, long* psync);'
)

define(func_shmem_fcollect_x,
`void shmem_fcollect$1(void *dest, const void *source, size_t nelems, 
	int pe_start, int log_pe_stride, int pe_size, long* psync)
{
	const int step = 1 << log_pe_stride;
	const int pe_shift = pe_size << log_pe_stride;
	const int pe_end = pe_start + pe_shift;
	const int my_pe = __shmem.my_pe;

	const int nbytes = nelems << ifelse($1,32,2,ifelse($1,64,3,?$1?));

	const ptrdiff_t offset = nbytes * ((my_pe - pe_start) >> log_pe_stride);
	const void* target = (void*)((intptr_t)dest + offset);

	int pe = my_pe;

	do {
		shmemx_memcpy(shmem_ptr(target,pe), source, nelems*sizeof($2));
		pe += step;
	} while (pe < pe_end);

	pe -= pe_shift;
	while (pe < my_pe) {
		shmemx_memcpy(shmem_ptr(target,pe), source, nelems*sizeof($2));
		pe += step;
	}
	shmem_barrier(pe_start, log_pe_stride, pe_size, psync);

}'
)
