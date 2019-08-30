/* xmalloc.h
 *
 * Copyright (c) 2019 Brown Deer Technology, LLC.  All Rights Reserved.
 * Copyright (c) 2012 Brown Deer Technology, LLC.  All Rights Reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

/* DAR */

#ifndef _xmalloc_h
#define _xmalloc_h

#include <sys/types.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef void* xmem_t;

xmem_t xmalloc_init( void* ptr, size_t len, int flags );

void* xmalloc( xmem_t xmem, size_t len );

void* xrealloc( xmem_t xmem, void* ptr, size_t len );

void xfree( xmem_t xmem, void* ptr );

void* xcalloc( xmem_t xmem, size_t num, size_t size );

int xposix_memalign(xmem_t xmem, void** memptr, size_t alignment, size_t size);

void xmem_final( xmem_t xmem );

#ifdef __cplusplus
}
#endif

#endif


