/* fcollect64.c
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
 * Performance test for shmem_fcollect64
 */

#include <stdio.h>
#include <shmem.h>
#include "ctimer.h"

#define NELEMENT 128
#define NLOOP 10

int main (void)
{
	int i, j, nelement;
	static long pSyncA[SHMEM_COLLECT_SYNC_SIZE];
	static long pSyncB[SHMEM_COLLECT_SYNC_SIZE];
	for (i = 0; i < SHMEM_COLLECT_SYNC_SIZE; i++) {
		pSyncA[i] = SHMEM_SYNC_VALUE;
		pSyncB[i] = SHMEM_SYNC_VALUE;
	}

	shmem_init();
	int me = shmem_my_pe();
	int npes = shmem_n_pes();

	long long* source = (long long*)shmem_malloc(NELEMENT * sizeof (*source));
	long long* target = (long long*)shmem_malloc(NELEMENT * sizeof (*target) * npes);

	for (i = 0; i < NELEMENT; i++) {
		source[i] = (i + 1) * 10 + me;
	}

	if (me == 0) {
		printf("# SHMEM Fcollect64 times for NPES = %d\n" \
			"# Bytes\tLatency (nanoseconds)\n", npes);
	}

	for (nelement = 1; nelement <= NELEMENT; nelement <<= 1)
	{
		for (i = 0; i < nelement * npes; i++) {
			target[i] = -90;
		}
		shmem_barrier_all();
		ctimer_start();

		unsigned int t = ctimer();
		for (i = 0; i < NLOOP; i += 2) {
			shmem_fcollect64(target, source, nelement, 0, 0, npes, pSyncA);
			shmem_fcollect64(target, source, nelement, 0, 0, npes, pSyncB);
		}
		t -= ctimer();

		if (me == 0) {
			int bytes = nelement * sizeof(*source);
			unsigned int nsec = ctimer_nsec(t / NLOOP);
			printf("%5d %7u\n", bytes, nsec);
		}
		int err = 0;
		for (j = 0; j < npes; j++) {
			for (i = 0; i < nelement; i++) {
				if (target[j*nelement + i] != ((i + 1) * 10 + j)) err++;
			}
		}
		if (err) printf("%d: %d ERRORS\n", me, err);
	}

	shmem_free(target);
	shmem_free(source);

	shmem_finalize();

	return 0;
}
