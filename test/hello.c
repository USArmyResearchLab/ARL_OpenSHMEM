/* hello.c
 *
 * Copyright (c) 2016 U.S. Army Research Laboratory.  All rights reserved.
 * Copyright (c) 2019 Brown Deer Technology, LLC. All rights reserved.
 *
 * This file is part of the ARL OpenSHMEM Reference Implementation software
 * package. For license information, see the LICENSE file in the top level
 * directory of the distribution.
 */

/*
 * "Hello, World" of OpenSHMEM
 */

#include <stdio.h>
#include <string.h>
#include <shmem.h>

int main(int argc, char* argv[])
{
	int err = 0;
	int major, minor;
	char name[SHMEM_MAX_NAME_LEN];

	// Starts/Initializes SHMEM/OpenSHMEM
	// Some implementations use the deprecated start_pes(0)
	shmem_init();

	// Fetch the number or processes
	// Some implementations use the deprecated num_pes()
	int n_pes = shmem_n_pes();

	// Assign my process ID to me
	int me = shmem_my_pe();

	// Query 
	shmem_info_get_name(name);
	shmem_info_get_version(&major, &minor);

	if (me == 0) {
		printf("# API   : %s (%d.%d)\n", name, major, minor);
		printf("# HEADER: %s (%d.%d)\n", SHMEM_VENDOR_STRING,
			SHMEM_MAJOR_VERSION, SHMEM_MINOR_VERSION);
	}

	// Global barrier
	shmem_barrier_all();

	// Comparing shmem.h header values and API values
	if (strncmp(name, SHMEM_VENDOR_STRING, SHMEM_MAX_NAME_LEN)) {
		printf("# %d: ERROR: vendor name mismatch\n", me);
		err++;
	}
	if (major != SHMEM_MAJOR_VERSION) {
		printf("# %d: ERROR: major version mismatch\n", me);
		err++;
	}
	if (minor != SHMEM_MINOR_VERSION) {
		printf("# %d: ERROR: minor version mismatch\n", me);
		err++;
	}

	printf("Hello, World from %d of %d\n", me, n_pes);

	return(err);
}
