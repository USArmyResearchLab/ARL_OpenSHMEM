dnl* shmem_x_atomic_fetch_inc.m4
dnl*
dnl* Copyright (c) 2016 U.S. Army Research Laboratory.  All rights reserved.
dnl* Copyright (c) 2019 Brown Deer Technology, LLC. All rights reserved.
dnl*
dnl* This file is part of the ARL OpenSHMEM Reference Implementation software
dnl* package. For license information, see the LICENSE file in the top level
dnl* directory of the distribution.
dnl*

define(decl_shmem_x_atomic_fetch_inc,
`$2 shmem_$1_atomic_fetch_inc( $2* ptr, int pe);'
)

define(func_shmem_x_atomic_fetch_inc,
`$2 shmem_$1_atomic_fetch_inc( $2* ptr, int pe)
{
	$2* remote_ptr = ($2*)shmem_ptr((void*)ptr, pe ); 
	long* x = (long*)shmem_ptr((void*)&__shmem.lock_atomic_$1, pe);
	__set_spin_lock(x);
	$2 r = *remote_ptr;
	*remote_ptr = r + 1;
	__clear_spin_lock(x);
	return r;
}'
)
