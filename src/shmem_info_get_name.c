/* shmem_info_get_name.c
 *
 * Copyright (c) 2016 U.S. Army Research Laboratory.  All rights reserved.
 * Copyright (c) 2019 Brown Deer Technology, LLC. All rights reserved.
 *
 * This file is part of the ARL OpenSHMEM Reference Implementation software
 * package. For license information, see the LICENSE file in the top level
 * directory of the distribution. 
 *
 */

#include "shmem.h"
#include "internals.h"

SHMEM_SCOPE
void shmem_info_get_name(char *name)
{
	const char* vendor_string = SHMEM_VENDOR_STRING;
	char* tmp = (char*)vendor_string;
	while ((*tmp)) (*name++) = (*tmp++);
	*name = '\0'; // null terminate
}

