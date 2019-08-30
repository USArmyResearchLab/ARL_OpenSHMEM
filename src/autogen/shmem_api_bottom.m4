dnl* shmem_api_bottom.m4
dnl*
dnl* Copyright (c) 2016 U.S. Army Research Laboratory.  All rights reserved.
dnl* Copyright (c) 2019 Brown Deer Technology, LLC. All rights reserved.
dnl*
dnl* This file is part of the ARL OpenSHMEM Reference Implementation software
dnl* package. For license information, see the LICENSE file in the top level
dnl* directory of the distribution.
dnl*

#ifdef __cplusplus
}
#endif

#define DECL_SHMEM_TYPE_RMA(TYPE,TYPENAME,SIZE) \
static void shmem_##TYPENAME##_put_nbi (TYPE *dest, const TYPE *source, size_t nelems, int pe) \
{ shmem_put##SIZE##_nbi (dest, source, nelems, pe); } \
static void shmem_ctx_##TYPENAME##_put_nbi (shmem_ctx_t ctx, TYPE *dest, const TYPE *source, size_t nelems, int pe) \
{ shmem_put##SIZE##_nbi (dest, source, nelems, pe); } \
static void shmem_##TYPENAME##_get_nbi (TYPE *dest, const TYPE *source, size_t nelems, int pe) \
{ shmem_get##SIZE##_nbi (dest, source, nelems, pe); } \
static void shmem_ctx_##TYPENAME##_get_nbi (shmem_ctx_t ctx, TYPE *dest, const TYPE *source, size_t nelems, int pe) \
{ shmem_get##SIZE##_nbi (dest, source, nelems, pe); } \
static void shmem_##TYPENAME##_put (TYPE *dest, const TYPE *source, size_t nelems, int pe) \
{ shmem_put##SIZE (dest, source, nelems, pe); } \
static void shmem_ctx_##TYPENAME##_put (shmem_ctx_t ctx, TYPE *dest, const TYPE *source, size_t nelems, int pe) \
{ shmem_put##SIZE (dest, source, nelems, pe); } \
static void shmem_##TYPENAME##_get (TYPE *dest, const TYPE *source, size_t nelems, int pe) \
{ shmem_get##SIZE (dest, source, nelems, pe); } \
static void shmem_ctx_##TYPENAME##_get (shmem_ctx_t ctx, TYPE *dest, const TYPE *source, size_t nelems, int pe) \
{ shmem_get##SIZE (dest, source, nelems, pe); } \
static void shmem_##TYPENAME##_p (TYPE *dest, TYPE value, int pe) \
{ TYPE* ptr = (TYPE*)shmem_ptr((void*)dest, pe); *ptr = value; } \
static void shmem_ctx_##TYPENAME##_p (shmem_ctx_t ctx, TYPE *dest, TYPE value, int pe) \
{ shmem_##TYPENAME##_p (dest, value, pe); } \
static TYPE shmem_##TYPENAME##_g (TYPE *source, int pe) \
{ return *((TYPE*)shmem_ptr((void*)source, pe)); } \
static TYPE shmem_ctx_##TYPENAME##_g (TYPE *source, int pe) \
{ return shmem_##TYPENAME##_g (source, pe); } \
static void shmem_##TYPENAME##_iput (TYPE *dest, const TYPE *source, ptrdiff_t dst, ptrdiff_t sst, size_t nelems, int pe) \
{ shmem_iput##SIZE (dest, source, dst, sst, nelems, pe); } \
static void shmem_ctx_##TYPENAME##_iput (shmem_ctx_t ctx, TYPE *dest, const TYPE *source, ptrdiff_t dst, ptrdiff_t sst, size_t nelems, int pe) \
{ shmem_iput##SIZE (dest, source, dst, sst, nelems, pe); } \
static void shmem_##TYPENAME##_iget (TYPE *dest, const TYPE *source, ptrdiff_t dst, ptrdiff_t sst, size_t nelems, int pe) \
{ shmem_iget##SIZE (dest, source, dst, sst, nelems, pe); } \
static void shmem_ctx_##TYPENAME##_iget (shmem_ctx_t ctx, TYPE *dest, const TYPE *source, ptrdiff_t dst, ptrdiff_t sst, size_t nelems, int pe) \
{ shmem_iget##SIZE (dest, source, dst, sst, nelems, pe); }

DECL_SHMEM_TYPE_RMA(float,              float,       32)
DECL_SHMEM_TYPE_RMA(double,             double,      64)
DECL_SHMEM_TYPE_RMA(long double,        longdouble, 128)
DECL_SHMEM_TYPE_RMA(char,               char,         8)
DECL_SHMEM_TYPE_RMA(signed char,        schar,        8)
DECL_SHMEM_TYPE_RMA(short,              short,       16)
DECL_SHMEM_TYPE_RMA(int,                int,         32)
DECL_SHMEM_TYPE_RMA(long long,          longlong,    64)
DECL_SHMEM_TYPE_RMA(unsigned char,      uchar,        8)
DECL_SHMEM_TYPE_RMA(unsigned short,     ushort,      16)
DECL_SHMEM_TYPE_RMA(unsigned int,       uint,        32)
DECL_SHMEM_TYPE_RMA(unsigned long long, ulonglong,   64)
DECL_SHMEM_TYPE_RMA(int8_t,             int8,         8)
DECL_SHMEM_TYPE_RMA(int16_t,            int16,       16)
DECL_SHMEM_TYPE_RMA(int32_t,            int32,       32)
DECL_SHMEM_TYPE_RMA(int64_t,            int64,       64)
DECL_SHMEM_TYPE_RMA(uint8_t,            uint8,        8)
DECL_SHMEM_TYPE_RMA(uint16_t,           uint16,      16)
DECL_SHMEM_TYPE_RMA(uint32_t,           uint32,      32)
DECL_SHMEM_TYPE_RMA(uint64_t,           uint64,      64)
#if __LP64__
DECL_SHMEM_TYPE_RMA(long,               long,        64)
DECL_SHMEM_TYPE_RMA(unsigned long,      ulong,       64)
DECL_SHMEM_TYPE_RMA(size_t,             size,        64)
DECL_SHMEM_TYPE_RMA(ptrdiff_t,          ptrdiff,     64)
#else
DECL_SHMEM_TYPE_RMA(long,               long,        32)
DECL_SHMEM_TYPE_RMA(unsigned long,      ulong,       32)
DECL_SHMEM_TYPE_RMA(size_t,             size,        32)
DECL_SHMEM_TYPE_RMA(ptrdiff_t,          ptrdiff,     32)
#endif

#endif
