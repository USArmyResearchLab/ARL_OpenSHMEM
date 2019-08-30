/* barrier.c
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
 * Performance test for shmem_barrier
 */

#include <stdio.h>
#include <shmem.h>
#include "ctimer.h"

#define NLOOP 100

int main (void)
{
	int i, npe;
	static long pSyncA[SHMEM_BARRIER_SYNC_SIZE];
	static long pSyncB[SHMEM_BARRIER_SYNC_SIZE];
	for (i = 0; i < SHMEM_BARRIER_SYNC_SIZE; i++) {
		pSyncA[i] = SHMEM_SYNC_VALUE;
		pSyncB[i] = SHMEM_SYNC_VALUE;
	}

	shmem_init();
	int me = shmem_my_pe();
	int npes = shmem_n_pes();

	if (me == 0) {
		printf("# SHMEM Barrier times for variable NPES\n" \
			"# NPES\tLatency (nanoseconds)\n");
	}

	for (npe = 1; npe <= npes; npe++)
	{
		shmem_barrier_all();
		ctimer_start();

		unsigned int t = ctimer();
		if (me < npe) {
			for (i = 0; i < NLOOP; i += 2) {
				shmem_barrier(0, 0, npe, pSyncA);
				shmem_barrier(0, 0, npe, pSyncB);
			}
		}
		t -= ctimer();

		if (me == 0) {
			unsigned int nsec = ctimer_nsec(t / NLOOP);
			printf("%5d %7u\n", npe, nsec);
		}
	}
	shmem_finalize();

	return 0;
}
