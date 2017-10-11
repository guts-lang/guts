/*
 * MIT License
 *
 * Copyright (c) 2016-2017 Abel Lucas <www.github.com/uael>
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

/*!@file sys/tys.h
 * @author uael
 */
#ifndef __SYS_TYS_H
# define __SYS_TYS_H

#include "incs.h"

#ifdef NULL
# define nil NULL
#else
# define nil ((void *) 0)
#endif

typedef char char_t;
typedef float f32_t;
typedef double f64_t;

#if __has_feature__(stdint_h)
typedef int8_t i8_t;
typedef int16_t i16_t;
typedef int32_t i32_t;
typedef int64_t i64_t;
typedef uint8_t u8_t;
typedef uint16_t u16_t;
typedef uint32_t u32_t;
typedef uint64_t u64_t;
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

#if defined OS_WIN || !__has_feature__(stdint_h)
# if CPU_64 && !defined WIN32
typedef i64_t isize_t;
typedef u64_t usize_t;
#   define ISIZE_MAX I64_MAX
#   define USIZE_MAX U64_MAX
# else
typedef i32_t isize_t;
typedef u32_t usize_t;
#   define ISIZE_MAX I32_MAX
#   define USIZE_MAX U32_MAX
# endif
#else
typedef ssize_t isize_t;
typedef size_t usize_t;
# define ISIZE_MAX SSIZE_MAX
# define USIZE_MAX SIZE_MAX
#endif

#if !defined __cplusplus && !defined __bool_true_false_are_defined
# define bool unsigned char
# define true 1
# define false 0
# define __bool_true_false_are_defined 1
#endif

typedef bool bool_t;

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

#define i8eq(a, b) ((bool_t)((i8_t)(a) == (i8_t)(b)))
#define u8eq(a, b) ((bool_t)((u8_t)(a) == (u8_t)(b)))
#define i16eq(a, b) ((bool_t)((i16_t)(a) == (i16_t)(b)))
#define u16eq(a, b) ((bool_t)((u16_t)(a) == (u16_t)(b)))
#define i32eq(a, b) ((bool_t)((i32_t)(a) == (i32_t)(b)))
#define u32eq(a, b) ((bool_t)((u32_t)(a) == (u32_t)(b)))
#define i64eq(a, b) ((bool_t)((i64_t)(a) == (i64_t)(b)))
#define u64eq(a, b) ((bool_t)((u64_t)(a) == (u64_t)(b)))
#define isizeeq(a, b) ((bool_t)((isize_t)(a) == (isize_t)(b)))
#define usizeeq(a, b) ((bool_t)((usize_t)(a) == (usize_t)(b)))
#define f32eq(a, b) ((bool_t)((f32_t)(a) == (f32_t)(b)))
#define f64eq(a, b) ((bool_t)((f64_t)(a) == (f64_t)(b)))
#define streq(a, b) ((bool_t)(strcmp(a, b) == 0))
#define addreq(a, b) ((bool_t)(&(a) == &(b)))

#define i8cmp(a, b) ((i8_t)(((a) > (b)) - ((a) < (b))))
#define u8cmp(a, b) ((i8_t)(((a) > (b)) - ((a) < (b))))
#define i16cmp(a, b) ((i8_t)(((a) > (b)) - ((a) < (b))))
#define u16cmp(a, b) ((i8_t)(((a) > (b)) - ((a) < (b))))
#define i32cmp(a, b) ((i8_t)(((a) > (b)) - ((a) < (b))))
#define u32cmp(a, b) ((i8_t)(((a) > (b)) - ((a) < (b))))
#define i64cmp(a, b) ((i8_t)(((a) > (b)) - ((a) < (b))))
#define u64cmp(a, b) ((i8_t)(((a) > (b)) - ((a) < (b))))
#define isizecmp(a, b) ((i8_t)(((a) > (b)) - ((a) < (b))))
#define usizecmp(a, b) ((i8_t)(((a) > (b)) - ((a) < (b))))
#define f32cmp(a, b) ((i8_t)(((a) > (b)) - ((a) < (b))))
#define f64cmp(a, b) ((i8_t)(((a) > (b)) - ((a) < (b))))
#define addrcmp(a, b) ((i8_t)((&(a) > &(b)) - (&(a) < &(b))))

#endif /* !__SYS_TYS_H */
