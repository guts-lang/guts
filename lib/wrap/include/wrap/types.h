/*
 * MIT License
 *
 * Copyright (c) 2016-2018 Abel Lucas <www.github.com/uael>
 * Copyright (c) 2016-2018 Guts <www.github.com/guts-lang>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/*!@file wrap/types.h
 * @author uael
 *
 * @addtogroup wrap @{
 */
#ifndef __WRAP_TYPES_H
# define __WRAP_TYPES_H

#include "wrap/defs.h"

#ifndef NULL
# ifdef __cplusplus
#   define NULL nullptr
# else
#   define NULL ((void *)0)
# endif
#endif

typedef float f32_t;
typedef double f64_t;

#ifdef HAS_INTTYPES_H
# include <inttypes.h>
#endif
#ifdef HAS_SYS_TYPES_H
# include <sys/types.h>
#endif

#ifdef HAS_STDINT_H
# if defined(__cplusplus)
#   include <cstdint>
typedef std::int8_t i8_t;
typedef std::int16_t i16_t;
typedef std::int32_t i32_t;
typedef std::int64_t i64_t;
typedef std::uint8_t u8_t;
typedef std::uint16_t u16_t;
typedef std::uint32_t u32_t;
typedef std::uint64_t u64_t;
# else
#   include <stdint.h>
typedef int8_t i8_t;
typedef int16_t i16_t;
typedef int32_t i32_t;
typedef int64_t i64_t;
typedef uint8_t u8_t;
typedef uint16_t u16_t;
typedef uint32_t u32_t;
typedef uint64_t u64_t;
# endif
#else
typedef signed char i8_t;
typedef signed short i16_t;
typedef signed int i32_t;
typedef signed long long i64_t;
typedef unsigned char u8_t;
typedef unsigned short u16_t;
typedef unsigned int u32_t;
typedef unsigned long long u64_t;
#endif

#if defined(OS_WIN) || !defined(HAS_STDINT_H)
# if CPU_64 && !defined(WIN32)
typedef i64_t isize_t;
#   if !defined(_SIZE_T_DEFINED)
typedef u64_t usize_t;
#   else
typedef size_t usize_t;
#   endif
#   if !defined(SSIZE_MAX)
#     define ISIZE_MAX I64_MAX
#   else
#     define ISIZE_MAX SSIZE_MAX
#   endif
#   if !defined(SIZE_MAX)
#     define USIZE_MAX U64_MAX
#   else
#     define USIZE_MAX SIZE_MAX
#   endif
# else
typedef i32_t isize_t;
#   if !defined(_SIZE_T_DEFINED)
typedef u32_t usize_t;
#   else
typedef size_t usize_t;
#   endif
#   if !defined(SSIZE_MAX)
#     define ISIZE_MAX I32_MAX
#   else
#     define ISIZE_MAX SSIZE_MAX
#   endif
#   if !defined(SIZE_MAX)
#     define USIZE_MAX U32_MAX
#   else
#     define USIZE_MAX SIZE_MAX
#   endif
# endif
#else
typedef ssize_t isize_t;
typedef size_t usize_t;
# define ISIZE_MAX SSIZE_MAX
# define USIZE_MAX SIZE_MAX
#endif

#ifdef HAS_STDBOOL_H
# if defined(__cplusplus) && __cplusplus >= 201103L
#   include <cstdbool>
# else
#   include <stdbool.h>
# endif
#else
# if !defined(__cplusplus) && !defined(__bool_true_false_are_defined)
#   define bool unsigned char
#   define true 1
#   define false 0
#   define __bool_true_false_are_defined 1
# endif
#endif

#include <limits.h>

#ifdef INT8_MIN
# define I8_MIN INT8_MIN
#else
# define I8_MIN (-(I8_MAX) - 1)
#endif

#ifdef INT16_MIN
# define I16_MIN INT16_MIN
#else
# define I16_MIN (-(I16_MAX) - 1)
#endif

#ifdef INT32_MIN
# define I32_MIN INT32_MIN
#else
# define I32_MIN (-(I32_MAX) - 1)
#endif

#ifdef INT64_MIN
# define I64_MIN INT64_MIN
#else
# define I64_MIN (-(I64_MAX) - 1)
#endif

#ifdef INT8_MAX
# define I8_MAX INT8_MAX
#else
# define I8_MAX 127
#endif

#ifdef INT16_MAX
# define I16_MAX INT16_MAX
#else
# define I16_MAX 32767
#endif

#ifdef INT32_MAX
# define I32_MAX INT32_MAX
#else
# define I32_MAX 2147483647
#endif

#ifdef INT64_MAX
# define I64_MAX INT64_MAX
#else
# define I64_MAX 9223372036854775807LL
#endif

#ifdef UINT8_MAX
# define U8_MAX UINT8_MAX
#else
# define U8_MAX 0xffU
#endif

#ifdef UINT16_MAX
# define U16_MAX UINT16_MAX
#else
# define U16_MAX 0xffffU
#endif

#ifdef UINT32_MAX
# define U32_MAX UINT32_MAX
#else
# define U32_MAX 0xffffffffUL
#endif

#ifdef UINT64_MAX
# define U64_MAX UINT64_MAX
#else
# define U64_MAX 0xffffffffffffffffULL
#endif

#endif /* !__WRAP_TYPES_H */
/*!@} */
