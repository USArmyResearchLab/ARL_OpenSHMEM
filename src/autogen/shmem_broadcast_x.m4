dnl* shmem_broadcast_x.m4
dnl*
dnl* Copyright (c) 2016 U.S. Army Research Laboratory.  All rights reserved.
dnl* Copyright (c) 2019 Brown Deer Technology, LLC. All rights reserved.
dnl*
dnl* This file is part of the ARL OpenSHMEM Reference Implementation software
dnl* package. For license information, see the LICENSE file in the top level
dnl* directory of the distribution.
dnl*

define(decl_shmem_broadcast_x,
`void shmem_broadcast$1( void* dest, const void* source, size_t nelems, 
	int pe_root, int pe_start, int log_pe_stride, int pe_size, long* psync);'
)

define(func_shmem_broadcast_x,
`void shmem_broadcast$1( void* dest, const void* source, size_t nelems, 
	int pe_root, int pe_start, int log_pe_stride, int pe_size, long* psync)
{
	if (pe_size == 1) return;

	int pe = __shmem.my_pe;

	int pe_root_stride = pe_root << log_pe_stride;

	$2* psrc = ($2*)dest;

	int pex = pe - pe_start - pe_root_stride;

	int pe_size_stride = pe_size << log_pe_stride;

	if (pex < 0) pex += pe_size_stride;

	if (pex == 0) psrc = ($2*)source;

	int pe_end = pe_size_stride + pe_start;

	int maskx = pe_size_stride - 1;

	maskx |= (maskx >> 1);

	maskx |= (maskx >> 2); /* up to 16 PEs */
	maskx |= (maskx >> 4); /* up to 1024 PEs */
	/*maskx |= (maskx >> 8);*/ /* up to 65536 PEs */

	maskx += 1; /* the next largest power of 2 of the largest PE number */

	maskx >>= 1;

	int mask2 = pex & -pex; /* least significant 1 bit */

	if (pex==0) mask2 = maskx;

	int mask1 = (mask2 << 1) - 1;

	__shmem.lock_receive_finished = 0;

	debug("broadcast calling barrier");

	shmem_barrier(pe_start, log_pe_stride, pe_size, psync);

	do {
		if ((pex & mask1) == 0) {

			int pe_to = (pex | mask2) + pe_start + pe_root_stride;

			if (pe_to >= pe_end) pe_to -= pe_size_stride;

			long* remote_lock_receive_finished 
				= (long*)shmem_ptr((void*)&__shmem.lock_receive_finished, pe_to);

			$2* remote_dest = ($2*)shmem_ptr((void*)dest, pe_to);

			shmemx_memcpy((void*)remote_dest, (void*)psrc, nelems*sizeof($2));

			/// clear lock_receive_finished on receiving PE 
			*remote_lock_receive_finished = 1;
//			__set_spin_lock(remote_lock_receive_finished);

		} else {
			while (!__shmem.lock_receive_finished);
			__shmem.lock_receive_finished = 0;
//			__clear_spin_lock(&__shmem.lock_receive_finished);
		}
		mask1 >>= 1;
		mask2 >>= 1;

	} while (mask1 >> log_pe_stride);

}'
)
