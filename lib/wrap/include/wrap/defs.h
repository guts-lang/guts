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

/*!@file wrap/defs.h
 * @author uael
 *
 * @addtogroup wrap @{
 */
#ifndef __WRAP_DEFS_H
# define __WRAP_DEFS_H

#include "wrap/feature.h"

#ifdef __cplusplus
# include <cstdlib>
# include <cstddef>
# include <cstdarg>
# include <cerrno>
#else
# include <stdlib.h>
# include <stddef.h>
# include <stdarg.h>
# include <errno.h>
#endif

#if defined(OS_WIN) && defined(CC_MSVC)
# define _WIN32_WINNT 0x0502
# define WIN32_LEAN_AND_MEAN
# include <windows.h>
# include <malloc.h>
#endif

#if defined(__STDC__) || defined(__cplusplus) || defined(CC_MSVC)
# if !defined(__const)
#   define __const const
# endif
# if !defined(__signed)
#   define __signed signed
# endif
# if !defined(CC_MSVC)
#   if !defined(__volatile)
#     define __volatile volatile
#   endif
#   if !defined(CC_GCC)
#     if !defined(__restrict)
#       define __restrict
#     endif
#     define __restrict
#   else
#     if !defined(__restrict)
#       define __restrict __restrict__
#     endif
#   endif
# endif
# if defined(__cplusplus)
#   if !defined(__inline)
#     define __inline inline
#   endif
#   if !defined(__register)
#     define __register
#   endif
# else
#   if !defined(__register)
#     define __register register
#   endif
# endif
#else
# if !defined(CC_GCC) && !defined(CC_MSVC)
#   if !defined(__const)
#     define __const
#   endif
#   if !defined(__inline)
#     define __inline
#   endif
#   if !defined(__signed)
#     define __signed
#   endif
#   if !defined(__volatile)
#     define __volatile
#   endif
#   if !defined(__register)
#     define __register
#   endif
#   if !defined(__restrict)
#     define __restrict
#   endif
# endif
#endif

#if !defined(CC_MSVC) && !defined(__cdecl)
# define __cdecl
#endif

#if !defined(__extern_c)
# ifdef __cplusplus
#   define __extern_c extern "C"
# else
#   define __extern_c
# endif
#endif

#if !defined(__api)
# if defined(_MD) && defined(_DLL)
#   if __has_declspec_attribute__(dllexport)
#     define __export_link __declspec(dllexport)
#   elif defined(__export)
#     define __export_link __export
#   else
#     define __export_link
#   endif
#   if __has_declspec_attribute__(dllimport)
#     define __import_link __declspec(dllimport)
#   else
#     define __import_link
#   endif
# else
#   if __has_attribute__(visibility)
#     define __export_link __attribute__((visibility("default")))
#   else
#     define __export_link
#   endif
#   define __import_link
# endif
# if defined(API_COMPILE)
#   ifdef __cplusplus
#     define __ext extern "C" __import_link
#     define __api extern "C" __export_link
#   else
#     define __ext extern __import_link
#     define __api extern __export_link
#   endif
# else
#   ifdef __cplusplus
#     define __ext extern "C" __import_link
#     define __api extern "C" __import_link
#   else
#     define __ext extern __import_link
#     define __api extern __import_link
#   endif
# endif
#endif

#if !defined(__file__)
# if defined(__FILE__)
#   define __file__ __FILE__
# else
#   define __file__ NULL
# endif
#endif

#if defined(__FUNCTION__)
# define __func__ __FUNCTION__
#endif

#if !defined(__line__)
# if defined(__LINE__)
#   define __line__ __LINE__
# else
#   define __line__ (-1)
# endif
#endif

#if !defined(__pretty_func__)
# if defined(__PRETTY_FUNCTION__)
#   define __pretty_func__ __PRETTY_FUNCTION__
# elif defined(__PRETTY_FUNC__)
#   define __pretty_func__ __PRETTY_FUNC__
# else
#   define __pretty_func__ __func__
# endif
#endif

#if !defined(ALIGNED)
# if defined(__aligned)
#   define ALIGNED(x) __aligned(x)
# elif defined(aligned)
#   define ALIGNED(x) aligned(x)
# elif __has_declspec_attribute__(align)
#   define ALIGNED(x) __declspec(align(x))
# elif __has_attribute__(aligned)
#   define ALIGNED(x) __attribute__((aligned(x)))
# else
#   define ALIGNED(x)
# endif
#endif

#if !defined(NORETURN)
# if defined(__noreturn)
#   define NORETURN __noreturn
# elif defined(__dead2)
#   define NORETURN __dead2
# elif defined(_Noreturn)
#   define NORETURN _Noreturn
# elif __has_attribute__(noreturn)
#   define NORETURN __attribute__((__noreturn__))
# elif __has_declspec_attribute__(noreturn)
#   define NORETURN __declspec(noreturn)
# elif __has_feature__(stdnoreturn_h) || __has_feature__(c_noreturn)
#   define NORETURN _Noreturn
# else
#   define NORETURN
# endif
#endif

#if !defined(THREAD_LOCAL)
# if defined(__thread_local)
#   define THREAD_LOCAL __thread_local
# elif defined(_Thread_local) || __has_feature__(c_thread_local)
#   define THREAD_LOCAL _Thread_local
# elif defined(__thread) || defined(CC_GCC)
#   define THREAD_LOCAL __thread
# elif __has_declspec_attribute__(thread)
#   define THREAD_LOCAL __declspec(thread)
# else
#   define THREAD_LOCAL
# endif
#endif

#if !defined(DEPRECATED)
# if __has_declspec_attribute__(deprecated)
#   define DEPRECATED __declspec(deprecated)
# elif __has_attribute__(deprecated)
#   define DEPRECATED __attribute__((deprecated))
# else
#   define DEPRECATED
# endif
#endif

#if !defined(FORCEINLINE)
# if defined(DEBUG)
#   define FORCEINLINE __inline
# elif defined(__forceinline) || defined(CC_MSVC)
#   define FORCEINLINE __forceinline
# elif __has_attribute__(always_inline)
#   define FORCEINLINE __attribute__((__always_inline__)) __inline
# else
#   define FORCEINLINE __inline
# endif
#endif

#if !defined(PURE)
# if __has_attribute__(pure)
#   define PURE __attribute__((__pure__))
# elif defined(__pure)
#   define PURE __pure
# else
#   define PURE
# endif
#endif

#if !defined(CONST)
# if __has_attribute__(const)
#   define CONST __attribute__((__const__))
# elif defined(__pure2)
#   define CONST __pure2
# else
#   define CONST
# endif
#endif

#if !defined(UNUSED)
# if __has_attribute__(unused)
#   define UNUSED __attribute__((unused))
# elif defined(__LCLINT__)
#   define UNUSED /*@unused@*/
# elif defined(__unused)
#   define UNUSED __unused
# else
#   define UNUSED
# endif
#endif

#if !defined(MALLOC)
# if __has_attribute__(malloc)
#   define MALLOC __attribute__((malloc))
# else
#   define MALLOC
# endif
#endif

#if !defined(NOINLINE)
# if __has_attribute__(noinline)
#   define NOINLINE __attribute__((noinline))
# elif __has_declspec_attribute__(noinline)
#   define NOINLINE __declspec(noinline)
# else
#   define NOINLINE
# endif
#endif

#if !defined(REGPARAM)
# if __has_attribute__(regparam)
#   define REGPARAM(N) __attribute__((regparam(N)))
# else
#   define REGPARAM(N)
# endif
#endif

#if !defined(OPTNONE)
# if __has_attribute__(optnone)
#   define OPTNONE __attribute__((optnone))
# elif __has_attribute__(optimize)
#   define OPTNONE __attribute__((optimize("O0")))
# else
#   define OPTNONE
# endif
#endif

#if !defined(RESTRICT)
# if __has_declspec_attribute__(restrict)
#   define RESTRICT __declspec(restrict)
# else
#   define RESTRICT
# endif
#endif

#if __has_builtin__(expect)
# define LIKELY(x) __builtin_expect(!!(x), 1)
# define UNLIKELY(x) __builtin_expect(!!(x), 0)
#else
# define LIKELY(x) (x)
# define UNLIKELY(x) (x)
#endif

#endif /* !__WRAP_DEFS_H */
/*!@} */
