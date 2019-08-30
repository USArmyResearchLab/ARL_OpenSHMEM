dnl* shmem_x_wait.m4
dnl*
dnl* Copyright (c) 2016 U.S. Army Research Laboratory.  All rights reserved.
dnl* Copyright (c) 2019 Brown Deer Technology, LLC. All rights reserved.
dnl*
dnl* This file is part of the ARL OpenSHMEM Reference Implementation software
dnl* package. For license information, see the LICENSE file in the top level
dnl* directory of the distribution.
dnl*

define(decl_shmem_x_wait,
`void shmem_$1_wait( $2* ivar, $2 cmp_val);'
)

define(func_shmem_x_wait,
`void shmem_$1_wait( $2* ivar, $2 cmp_val)
{ 
	volatile $2* p = (volatile $2*)ivar;
	while (*p == cmp_val);
}'
)
