/* alltoall64.c
 *
 * Copyright (c) 2016 U.S. Army Research Laboratory.  All rights reserved.
 * Copyright (c) 2019 Brown Deer Technology, LLC. All rights reserved.
 *
 * This file is part of the ARL OpenSHMEM Reference Implementation software
 * package. For license information, see the LICENSE file in the top level
 * directory of the distribution.
 *
 */

/*
 * Performance test for shmem_alltoall64
 *
 */

#include <stdio.h>
#include <shmem.h>
#include "ctimer.h"

#define NELEMENT 64
#define NLOOP 10

int main (void)
{
	int i, j, nelement;

	shmem_init();
	int me = shmem_my_pe();
	int npes = shmem_n_pes();

	static long pSyncA[SHMEM_ALLTOALL_SYNC_SIZE];
	static long pSyncB[SHMEM_ALLTOALL_SYNC_SIZE];
	for (i = 0; i < SHMEM_ALLTOALL_SYNC_SIZE; i++) {
		pSyncA[i] = SHMEM_SYNC_VALUE;
		pSyncB[i] = SHMEM_SYNC_VALUE;
	}

	long long* source 
		= (long long*)shmem_malloc(NELEMENT * npes * sizeof (*source));

	long long* target 
		= (long long*)shmem_malloc(NELEMENT * npes * sizeof (*target));

	for (i = 0; i < NELEMENT * npes; i++) {
		source[i] = me;
		target[i] = -90;
	}

	if (me == 0) {
		printf("# SHMEM AlltoAll64 times for NPES = %d\n" 
			"# Bytes\tLatency (nanoseconds)\n", npes); fflush(stdout);
	}

	for (nelement = 1; nelement <= NELEMENT; nelement <<= 1)
	{
		shmem_barrier_all();
		ctimer_start();

		unsigned int t = ctimer();
		for (i = 0; i < NLOOP; i += 2) {
			shmem_alltoall64 (target, source, nelement, 0, 0, npes, pSyncA);
			shmem_alltoall64 (target, source, nelement, 0, 0, npes, pSyncB);
		}
		t -= ctimer();

		if (me == 0) {
			unsigned int bytes = nelement * sizeof(*source);
			unsigned int nsec = ctimer_nsec(t / NLOOP);
			printf("%5d %7u\n", bytes, nsec);
		}
		unsigned int err = 0;
		for (j = 0; j < npes; j++) {
			for (i = 0; i < nelement; i++) {
				if (target[j*nelement + i] != j) err++;
			}
		}
		if (err) printf("# %d: %d ERRORS\n", me, err);
	}

	shmem_free(target);
	shmem_free(source);

	shmem_finalize();

	return 0;
}
