dnl* shmem_collect_x.m4
dnl*
dnl* Copyright (c) 2016 U.S. Army Research Laboratory.  All rights reserved.
dnl* Copyright (c) 2019 Brown Deer Technology, LLC. All rights reserved.
dnl*
dnl* This file is part of the ARL OpenSHMEM Reference Implementation software
dnl* package. For license information, see the LICENSE file in the top level
dnl* directory of the distribution.
dnl*

define(decl_shmem_collect_x,
`void shmem_collect$1(void *dest, const void *source, size_t nelems, 
	int pe_start, int log_pe_stride, int pe_size, long* psync);'
)

define(func_shmem_collect_x,
`void shmem_collect$1(void *dest, const void *source, size_t nelems, 
	int pe_start, int log_pe_stride, int pe_size, long* psync)
{
	int pe = __shmem.my_pe;
	int pe_step = 0x1 << log_pe_stride;
	int pe_end = pe_start + pe_step * (pe_size - 1);
	int my_offset = 0;
	int inelems = (int)nelems;

	/* We know the value here is 0 and we can use it and reset it before
	 * it is needed in the barrier */

	volatile long* offset = psync + SHMEM_COLLECT_SYNC_SIZE - 1;

	int* neighbor_offset = (int*)shmem_ptr((void*)offset, pe + pe_step);

	if (__builtin_expect((pe == pe_start),0)) {

		*neighbor_offset = my_offset + inelems;

	} else {

		/// spin until neighbor sets offset 
		while (!(my_offset = *offset));

		*offset = SHMEM_SYNC_VALUE;

		if (__builtin_expect((pe != pe_end),1))
			*neighbor_offset = my_offset + inelems;
	}

	int i, j;
	for (i = pe_start; i <= pe_end; i += pe_step) {
		$2* dst = ($2*)shmem_ptr((void*)dest, i) + my_offset;
		for (j = 0; j < inelems; j++) dst[j] = (($2*)source)[j];
	}

	shmem_barrier(pe_start, log_pe_stride, pe_size, psync);
}'
)
