dnl* shmem_get_x.m4
dnl*
dnl* Copyright (c) 2016 U.S. Army Research Laboratory.  All rights reserved.
dnl* Copyright (c) 2019 Brown Deer Technology, LLC. All rights reserved.
dnl*
dnl* This file is part of the ARL OpenSHMEM Reference Implementation software
dnl* package. For license information, see the LICENSE file in the top level
dnl* directory of the distribution.
dnl*

define(decl_shmem_get_x,
`void shmem_get$1( void *dest, const void *source, size_t nelems, int pe);'
)

define(func_shmem_get_x,
`void shmem_get$1( void *dest, const void *source, size_t nelems, int pe)
{
	shmemx_memcpy((void*)dest, shmem_ptr((void*)source,pe), nelems*sizeof($2) );
}'
)
