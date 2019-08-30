dnl* shmem_x_op_to_all.m4
dnl*
dnl* Copyright (c) 2016 U.S. Army Research Laboratory.  All rights reserved.
dnl* Copyright (c) 2019 Brown Deer Technology, LLC. All rights reserved.
dnl*
dnl* This file is part of the ARL OpenSHMEM Reference Implementation software
dnl* package. For license information, see the LICENSE file in the top level
dnl* directory of the distribution.
dnl*

define(decl_shmem_x_op_to_all,
`void shmem_$1_$3_to_all( $2* dst, const $2* src, int nreduce,
	int pe_start, int log_pe_stride, int pe_size, $2* pwork, long* psync);'
)

define(func_shmem_x_op_to_all,
`void shmem_$1_$3_to_all( $2* dst, const $2* src, int nreduce,
	int pe_start, int log_pe_stride, int pe_size, $2* pwork, long* psync)
{
	debug("shmem_x_op_to_all begin");

	int pe_size_stride = pe_size << log_pe_stride;
	int pe_step = 0x1 << log_pe_stride;
	int pe_end = pe_size_stride + pe_start;
	int nreduced2p1 = (nreduce >> 1) + 1;

	int nwrk = (nreduced2p1 > (int)SHMEM_REDUCE_MIN_WRKDATA_SIZE) ?
		 nreduced2p1 : (int)SHMEM_REDUCE_MIN_WRKDATA_SIZE;

	int i, j, r;

	for (i = 0; i < nreduce; i++) {
		dst[i] = src[i];
	}

	int to = __shmem.my_pe;

	for(r = 1; r < pe_size; r++) {

		to += pe_step;
		if (to >= pe_end) to -= pe_size_stride;

		$2* remote_work = ($2*)shmem_ptr((void*)pwork, to);

		for(i = 0; i < nreduce; i += nwrk) {

			int nrem = nreduce - i;
			nrem = (nrem > nwrk) ? nwrk : nrem;

			for(j = 0; j < nrem; j++) {
				remote_work[j] = src[i+j];
			}

			shmem_barrier(pe_start, log_pe_stride, pe_size, psync);

			for(j = 0; j < nrem; j++) {
				dst[i+j] = $4(dst[i+j],pwork[j]);
			}

			shmem_barrier(pe_start, log_pe_stride, pe_size, psync);

		}
	}

	debug("shmem_x_op_to_all end");
}'
)
