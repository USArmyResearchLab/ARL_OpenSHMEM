dnl* shmem_x_atomic_fetch.m4
dnl*
dnl* Copyright (c) 2016 U.S. Army Research Laboratory.  All rights reserved.
dnl* Copyright (c) 2019 Brown Deer Technology, LLC. All rights reserved.
dnl*
dnl* This file is part of the ARL OpenSHMEM Reference Implementation software
dnl* package. For license information, see the LICENSE file in the top level
dnl* directory of the distribution.
dnl*

define(decl_shmem_x_atomic_fetch,
`$2 shmem_$1_atomic_fetch( const $2* ptr, int pe);'
)

define(func_shmem_x_atomic_fetch,
`$2 shmem_$1_atomic_fetch( const $2* ptr, int pe)
{ 
	$2* remote_ptr = ($2*)shmem_ptr((void*)ptr, pe );
	return *(const volatile $2*)remote_ptr; 
}'
)
