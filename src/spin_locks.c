/* spin_locks.c
 *
 * Copyright (c) 2016 U.S. Army Research Laboratory.  All rights reserved.
 * Copyright (c) 2019 Brown Deer Technology, LLC. All rights reserved.
 *
 * This file is part of the ARL OpenSHMEM Reference Implementation software
 * package. For license information, see the LICENSE file in the top level
 * directory of the distribution. 
 *
 */

#include <sys/mman.h>

void __clear_spin_lock(volatile long* x)
{
	const long free_val = 0;
	const long set_val = 1;

	while ( ! __sync_bool_compare_and_swap (x, set_val, free_val) );
}

void __set_spin_lock (volatile long* x)
{
	const long free_val = 0;
	const long set_val = 1;

	while ( ! __sync_bool_compare_and_swap (x, free_val, set_val) );

}

int __test_spin_lock(volatile long* x)
{
   const long free_val = 0;
   const long set_val = 1;
//   return ((__atomic_compare_exchange(x, &free_val, &set_val, 1,
//      __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST))? 0 : 1);
	return __sync_val_compare_and_swap (x, free_val, set_val);
}

