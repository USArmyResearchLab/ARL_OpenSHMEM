dnl* shmem_x_test.m4
dnl*
dnl* Copyright (c) 2016 U.S. Army Research Laboratory.  All rights reserved.
dnl* Copyright (c) 2019 Brown Deer Technology, LLC. All rights reserved.
dnl*
dnl* This file is part of the ARL OpenSHMEM Reference Implementation software
dnl* package. For license information, see the LICENSE file in the top level
dnl* directory of the distribution.
dnl*

define(decl_shmem_x_test,
`int shmem_$1_test( $2* ivar, int cmp, $2 cmp_val);'
)

define(func_shmem_x_test,
`
int shmem_$1_test( $2* ivar, int cmp, $2 cmp_val)
{
	volatile $2* p = (volatile $2*)ivar;
	switch(cmp) {
		case SHMEM_CMP_EQ:
			if (*p == cmp_val) return 1;
			break;
		case SHMEM_CMP_NE:
			if (*p != cmp_val) return 1;
			break;
		case SHMEM_CMP_GT:
			if (*p > cmp_val) return 1;
			break;
		case SHMEM_CMP_LE:
			if (*p <= cmp_val) return 1;
			break;
		case SHMEM_CMP_LT:
			if (*p < cmp_val) return 1;
			break;
		case SHMEM_CMP_GE:
			if (*p >= cmp_val) return 1;
			break;
	}
	return 0;
}
'
)
