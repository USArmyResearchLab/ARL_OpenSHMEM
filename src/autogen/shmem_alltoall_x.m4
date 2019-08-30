dnl* shmem_alltoall_x.m4
dnl*
dnl* Copyright (c) 2016 U.S. Army Research Laboratory.  All rights reserved.
dnl* Copyright (c) 2019 Brown Deer Technology, LLC. All rights reserved.
dnl*
dnl* This file is part of the ARL OpenSHMEM Reference Implementation software
dnl* package. For license information, see the LICENSE file in the top level
dnl* directory of the distribution.
dnl*

define(decl_shmem_alltoall_x,
`void shmem_alltoall$1( void* dest, const void* source, size_t nelems,
	int pe_start, int log_pe_stride, int pe_size, long* psync);'
)

define(func_shmem_alltoall_x,
`void shmem_alltoall$1( void* dest, const void* source, size_t nelems,
	int pe_start, int log_pe_stride, int pe_size, long* psync)
{
	debug("[%d] shmem_alltoall_x begin\n",__shmem.my_pe);
	int j;
	int dst_offset = ((__shmem.my_pe - pe_start) >> log_pe_stride)*nelems;
	int pe_size_stride = pe_size << log_pe_stride;
	int step = 1 << log_pe_stride;
	int pe_end = pe_size_stride + pe_start;
	$2* psrc = ($2*)source;
	$2* pdsto = ($2*)dest + dst_offset;
	shmemx_memcpy((void*)pdsto, (void*)psrc, nelems*sizeof($2));
	for (j = 1; j < pe_size; j++) {
		int pe_to = __shmem.my_pe + j*step;
		if (pe_to >= pe_end) pe_to -= pe_size_stride;
		$2* pdst = ($2*)shmem_ptr(pdsto, pe_to);
		shmemx_memcpy((void*)pdst, (void*)psrc, nelems*sizeof($2));
	}
	debug("[%d] shmem_alltoall_x before barrier\n",__shmem.my_pe);
	shmem_barrier(pe_start, log_pe_stride, pe_size, psync);
	debug("[%d] shmem_alltoall_x end\n",__shmem.my_pe);
}'
)
