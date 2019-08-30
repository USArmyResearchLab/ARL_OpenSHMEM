/* shmem_global_exit.c
 *
 * Copyright (c) 2016 U.S. Army Research Laboratory.  All rights reserved.
 * Copyright (c) 2019 Brown Deer Technology, LLC. All rights reserved.
 *
 * This file is part of the ARL OpenSHMEM Reference Implementation software
 * package. For license information, see the LICENSE file in the top level
 * directory of the distribution. 
 *
 */

#include <stdlib.h>
#include "shmem.h"
#include "internals.h"

// C11 _Noreturn function specifier to shmem_global_exit
#if (defined(__STDC_VERSION__) && __STDC_VERSION__ >= 20112L)
_Noreturn
#endif
SHMEM_SCOPE
void shmem_global_exit(int status)
{
	exit(status);
}

