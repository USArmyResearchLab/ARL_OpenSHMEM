/* broadcast64.c
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
 * Performance test for shmem_broadcast64
 */

#include <stdio.h>
#include <shmem.h>
#include "ctimer.h"

#define NELEMENT 1024
#define NLOOP 10

int main (void)
{
	int i, elements;
	static long pSyncA[SHMEM_BCAST_SYNC_SIZE];
	static long pSyncB[SHMEM_BCAST_SYNC_SIZE];
	for (i = 0; i < SHMEM_BCAST_SYNC_SIZE; i++) {
		pSyncA[i] = SHMEM_SYNC_VALUE;
		pSyncB[i] = SHMEM_SYNC_VALUE;
	}

	shmem_init();
	int me = shmem_my_pe();
	int npes = shmem_n_pes();

	long long* source = (long long*)shmem_malloc(NELEMENT * sizeof (*source));
	long long* target = (long long*)shmem_malloc(NELEMENT * sizeof (*target));

	if (me == 0) {
		printf("# SHMEM Broadcast64 times for NPES = %d\n" \
			"# Bytes\tLatency (nanoseconds)\n", npes);
	}

	for (elements = 1; elements <= NELEMENT; elements <<= 1)
	{
		// reset values for each iteration
		for (i = 0; i < NELEMENT; i++) {
			source[i] = i + 1;
			target[i] = -90;
		}
		shmem_barrier_all();
		ctimer_start();

		unsigned int t = ctimer();
		for (i = 0; i < NLOOP; i += 2) {
			shmem_broadcast64 (target, source, elements, 0, 0, 0, npes, pSyncA);
			shmem_broadcast64 (target, source, elements, 0, 0, 0, npes, pSyncB);
		}
		t -= ctimer();

		if (me == 0) {
			unsigned int bytes = elements * sizeof(*source);
			unsigned int nsec = ctimer_nsec(t / NLOOP);
			printf("%5d %7u\n", bytes, nsec);
		}
		else {
			int err = 0;
			for (i = 0; i < elements; i++) if (target[i] != source[i]) err++;
			for (i = elements; i < NELEMENT; i++) if (target[i] != -90) err++;
			if (err) printf("# %d: ERROR: %d incorrect value(s) copied\n", me, err);
		}
	}

	shmem_free(target);
	shmem_free(source);

	shmem_finalize();

	return 0;
}
