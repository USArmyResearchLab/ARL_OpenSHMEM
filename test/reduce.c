/* reduce.c
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
#include <shmem.h>
#include "ctimer.h"

#define NELEMENT 2048
//#define NLOOP 100
#define NLOOP 10

int main (void)
{
	int i, nelement;
	static unsigned int t, tsum;
	static long pSync[SHMEM_REDUCE_SYNC_SIZE];
	for (i = 0; i < SHMEM_REDUCE_SYNC_SIZE; i++) {
		pSync[i] = SHMEM_SYNC_VALUE;
	}

	shmem_init();
	int me = shmem_my_pe();
	int npes = shmem_n_pes();

	int pwrk_elems = NELEMENT/2 + 1;
	pwrk_elems = (pwrk_elems > SHMEM_REDUCE_MIN_WRKDATA_SIZE) ? 
		pwrk_elems : SHMEM_REDUCE_MIN_WRKDATA_SIZE;

	int* source = (int*)shmem_malloc(NELEMENT * sizeof (int) * 1000);
	int* target = (int*)shmem_malloc(NELEMENT * sizeof (int) * 1000);
	int* pwrk   = (int*)shmem_malloc(pwrk_elems * sizeof(int) * 1000);
	for (i = 0; i < NELEMENT; i++) {
		source[i] = i;
	}

	if (me == 0) {
		printf("# SHMEM Reduction times for NPES = %d\n" \
			"# Nelements \tLatency (nanoseconds)\n", npes);
	}

	for (nelement = 1; nelement <= NELEMENT; nelement <<= 1) {

		// reset values for each iteration
		for (i = 0; i < NELEMENT; i++) {
			target[i] = -90;
		}
		shmem_barrier_all();
		ctimer_start();

		t = ctimer();
		for (i = 0; i < NLOOP; i++) {
			shmem_int_sum_to_all(target,source,nelement,0,0,npes, pwrk, pSync);
		}
		t -= ctimer();

		shmem_int_sum_to_all(&tsum, &t, 1, 0, 0, npes, pwrk, pSync);

		if (me == 0) {
			unsigned int nsec = ctimer_nsec(tsum / (npes * NLOOP));
			printf("%5d %7u\n", nelement, nsec);
		}

		int err = 0;
		for (i = 0; i < nelement; i++) {
			if (target[i] != i*npes) {
				printf("%d# error %d %d %d\n",me,i,target[i],i*npes);
				err++;
			}
		}
		if (err) printf("# %d: ERRORS %d\n", me, err);
	}

	shmem_free(target);
	shmem_free(source);

	shmem_finalize();

	return 0;
}
