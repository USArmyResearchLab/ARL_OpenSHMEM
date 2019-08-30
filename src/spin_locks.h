/* spin_locks.h
 *
 * Copyright (c) 2016 U.S. Army Research Laboratory.  All rights reserved.
 * Copyright (c) 2019 Brown Deer Technology, LLC. All rights reserved.
 *
 * This file is part of the ARL OpenSHMEM Reference Implementation software
 * package. For license information, see the LICENSE file in the top level
 * directory of the distribution. 
 *
 */

#ifndef _spin_locks_h
#define _spin_locks_h

void __clear_spin_lock(volatile long* x);

void __set_spin_lock (volatile long* x);

int __test_spin_lock(volatile long* x);

#endif

