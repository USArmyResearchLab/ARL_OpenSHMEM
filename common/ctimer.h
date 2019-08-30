/* ctimer.h
 *
 * Copyright (c) 2016 U.S. Army Research Laboratory.  All rights reserved.
 * Copyright (c) 2019 Brown Deer Technology, LLC. All rights reserved.
 *
 * This file is part of the ARL OpenSHMEM Reference Implementation software
 * package. For license information, see the LICENSE file in the top level
 * directory of the distribution.
 *
 */

#ifndef _ctimer_h
#define _ctimer_h

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#include <sys/time.h>

struct timeval t0; // initialized with ctimer_start()

static void
ctimer_start(void)
{
	gettimeofday(&t0, 0);
}

static uint64_t
ctimer_nsec(uint64_t t)
{
	return t;
}

static uint64_t inline __attribute__((__always_inline__)) 
ctimer(void)
{
	const static uint64_t r = 0xffffffffffffffff;
	uint64_t ns; // nanoseconds passed since calling ctimer_start()
	struct timeval t1;
	gettimeofday(&t1,0);
	ns = 1e9 * (t1.tv_sec - t0.tv_sec)
		+ 1e3 * (t1.tv_usec - t0.tv_usec);
	return (r - ns); // this counts down from r
}

#ifdef __cplusplus
}
#endif

#endif
