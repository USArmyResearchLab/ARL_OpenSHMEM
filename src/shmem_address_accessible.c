/* shmem_address_accessible.c
 *
 * Copyright (c) 2016 U.S. Army Research Laboratory.  All rights reserved.
 * Copyright (c) 2019 Brown Deer Technology, LLC. All rights reserved.
 *
 * This file is part of the ARL OpenSHMEM Reference Implementation software
 * package. For license information, see the LICENSE file in the top level
 * directory of the distribution. 
 *
 */

#include "internals.h"
#include "shmem.h"

#include <stdio.h>
#include <stdlib.h>

SHMEM_SCOPE
int shmem_addr_accessible(const void* addr, int pe)
{
	fprintf(stderr,"shmem_addr_accessible unimplemented\n");
	exit(-1);
}

