dnl* shmem_alltoalls_x.m4
dnl*
dnl* Copyright (c) 2016 U.S. Army Research Laboratory.  All rights reserved.
dnl* Copyright (c) 2019 Brown Deer Technology, LLC. All rights reserved.
dnl*
dnl* This file is part of the ARL OpenSHMEM Reference Implementation software
dnl* package. For license information, see the LICENSE file in the top level
dnl* directory of the distribution.
dnl*

define(decl_shmem_alltoalls_x,
`void shmem_alltoalls$1( void* dest, const void* source,
	ptrdiff_t dst, ptrdiff_t sst, size_t nelems,
	int PE_start, int logPE_stride, int PE_size, long *pSync);'
)

define(func_shmem_alltoalls_x,
`void shmem_alltoalls$1( void* dest, const void* source,
	ptrdiff_t dst, ptrdiff_t sst, size_t nelems,
	int PE_start, int logPE_stride, int PE_size, long *pSync)
{
	const int PE_step = 1 << logPE_stride;

	$2* pdst = ($2*)((intptr_t)dest + sizeof($2) * nelems * dst * __shmem.my_pe);

	int i, pe;

	for (i = 0, pe = PE_start; i < PE_size; i++, pe += PE_step) {

		shmem_iput$1(pdst,
			($2*)((intptr_t)source + sizeof($2) * nelems * sst * i),
			dst, sst, nelems, pe);

	}

	shmem_barrier(PE_start, logPE_stride, PE_size, pSync);

}'
)
