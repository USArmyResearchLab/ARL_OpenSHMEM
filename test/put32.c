/* put32.c
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
 * Performance test for shmem_put (latency and bandwidth)
 */

#include <stdio.h>
#include <shmem.h>
#include "ctimer.h"

#define NELEMENT 2048
#define NLOOP 10

int main (void)
{
	int i, nelement;
	static unsigned int t, tsum;
	static int pWrk[SHMEM_REDUCE_MIN_WRKDATA_SIZE];
	static long pSync[SHMEM_REDUCE_SYNC_SIZE];
	for (i = 0; i < SHMEM_REDUCE_SYNC_SIZE; i++) {
		pSync[i] = SHMEM_SYNC_VALUE;
	}

	shmem_init();
	int me = shmem_my_pe();
	int npes = shmem_n_pes();

	int nxtpe = me + 1;
	if (nxtpe >= npes) nxtpe -= npes;

	int* source = (int*)shmem_align(NELEMENT * sizeof(int), 0x2000);
	int* target = (int*)shmem_align(NELEMENT * sizeof(int), 0x2000);
	for (i = 0; i < NELEMENT; i++) {
		source[i] = i + 1;
	}

	if (me == 0) {
		printf("# SHMEM Put32 times for variable message size\n" \
			"# Bytes\tLatency (nanoseconds)\n");
	}

	/* For int put we take average of all the times realized by a pair of PEs,
	thus reducing effects of physical location of PEs */
	for (nelement = 1; nelement <= NELEMENT; nelement <<= 1)
	{
		// reset values for each iteration
		for (i = 0; i < NELEMENT; i++) {
			target[i] = -90;
		}
		shmem_barrier_all();
		ctimer_start();

		t = ctimer();

		for (i = 0; i < NLOOP; i++) {
			shmem_put32(target, source, nelement, nxtpe);
		}

		t -= ctimer();

		shmem_int_sum_to_all(&tsum, &t, 1, 0, 0, npes, pWrk, pSync);

		if (me == 0) {
			int bytes = nelement * sizeof(*source);
			unsigned int nsec = ctimer_nsec(tsum / (npes * NLOOP));
			printf("%6d %7u\n", bytes, nsec);
		}

		int err = 0;
		for (i = 0; i < nelement; i++) if (target[i] != source[i]) err++;
		for (i = nelement; i < NELEMENT; i++) if (target[i] != -90) err++;
		if (err) printf("# %d: ERROR: %d incorrect value(s) copied\n", me, err);
	}

	shmem_free(target);
	shmem_free(source);

	shmem_finalize();

	return 0;
}
