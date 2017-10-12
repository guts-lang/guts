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

/*!@file nt/defs.h
 * @author uael
 */
#ifndef __NT_DEFS_H
# define __NT_DEFS_H

#include "exts.h"

#if defined __STDC__ || defined __cplusplus || defined CC_MSVC
# if !defined __const
#   define __const const
# endif
# if !defined __signed
#   define __signed signed
# endif
# if !defined CC_MSVC
#   if !defined __volatile
#     define __volatile volatile
#   endif
#   if !defined CC_GCC
#     if !defined __restrict
#       define __restrict
#     endif
#     define __restrict
#   else
#     if !defined __restrict
#       define __restrict __restrict__
#     endif
#   endif
# endif
# if defined __cplusplus
#   if !defined __inline
#     define __inline inline
#   endif
#   if !defined __register
#     define __register
#   endif
# else
#   if !defined __register
#     define __register register
#   endif
# endif
#else
# if !defined CC_GCC && !defined CC_MSVC
#   if !defined __const
#     define __const
#   endif
#   if !defined __inline
#     define __inline
#   endif
#   if !defined __signed
#     define __signed
#   endif
#   if !defined __volatile
#     define __volatile
#   endif
#   if !defined __register
#     define __register
#   endif
#   if !defined __restrict
#     define __restrict
#   endif
# endif
#endif

#if !defined CC_MSVC && !defined __cdecl
# define __cdecl
#endif

#if !defined __extern_c__
# ifdef __cplusplus
#   define __extern_c__ extern "C"
# else
#   define __extern_c__
# endif
#endif

#if !defined __api__
# if defined _MT && defined _DLL
#   if __has_declspec_attribute__(dllexport)
#     define __export_link__ __declspec(dllexport)
#   elif defined __export
#     define __export_link__ __export
#   else
#     define __export_link__
#   endif
#   if __has_declspec_attribute__(dllimport)
#     define __import_link__ __declspec(dllimport)
#   else
#     define __import_link__
#   endif
# else
#   if __has_attribute__(visibility)
#     define __export_link__ __attribute__((visibility("default")))
#   else
#     define __export_link__
#   endif
#   define __import_link__
# endif
# if defined API_COMPILE
#   ifdef __cplusplus
#     define __ext__ extern "C" __import_link__
#     define __api__ extern "C" __export_link__
#   else
#     define __ext__ extern __import_link__
#     define __api__ extern __export_link__
#   endif
# else
#   ifdef __cplusplus
#     define __ext__ extern "C" __import_link__
#     define __api__ extern "C" __import_link__
#   else
#     define __ext__ extern __import_link__
#     define __api__ extern __import_link__
#   endif
# endif
#endif

#if !defined __file__
# if defined __FILE__
#   define __file__ __FILE__
# else
#   define __file__ nil
# endif
#endif

#if !defined __func__
# if defined __FUNCTION__
#   define __func__ __FUNCTION__
# elif defined __FUNC__
#   define __func__ __FUNC__
# else
#   define __func__ NULL
# endif
#endif

#if !defined __line__
# if defined __LINE__
#   define __line__ __LINE__
# else
#   define __line__ (-1)
# endif
#endif

#if !defined __pretty_func__
# if defined __PRETTY_FUNCTION__
#   define __pretty_func__ __PRETTY_FUNCTION__
# elif defined __PRETTY_FUNC__
#   define __pretty_func__ __PRETTY_FUNC__
# else
#   define __pretty_func__ __func__
# endif
#endif

#if !defined __aligned
# if defined aligned
#   define __aligned(x) aligned(x)
# elif __has_declspec_attribute__(align)
#   define __aligned(x) __declspec(align(x))
# elif __has_attribute__(aligned)
#   define __aligned(x) __attribute__((aligned(x)))
# else
#   define __aligned(x)
# endif
#endif

#if !defined __noreturn
# if defined __dead2
#   define __noreturn __dead2
# elif defined _Noreturn
#   define __noreturn _Noreturn
# elif __has_feature__(stdnoreturn_h) || __has_feature__(c_noreturn)
#   define __noreturn _Noreturn
# elif __has_attribute__(noreturn)
#   define __noreturn __attribute__((__noreturn__))
# elif __has_declspec_attribute__(noreturn)
#   define __noreturn __declspec(noreturn)
# else
#   define __noreturn
# endif
#endif

#if !defined __thread_local
# if defined _Thread_local || __has_feature__(c_thread_local)
#   define __thread_local _Thread_local
# elif defined __thread || defined CC_GCC
#   define __thread_local __thread
# elif __has_declspec_attribute__(thread)
#   define __thread_local __declspec(thread)
# endif
#endif

#if !defined DEPRECATED
# if __has_declspec_attribute__(deprecated)
#   define DEPRECATED __declspec(deprecated)
# elif __has_attribute__(deprecated)
#   define DEPRECATED __attribute__((deprecated))
# else
#   define DEPRECATED
# endif
#endif

#if !defined NOSANITIZE
# if __has_feature__(address_sanitizer) || defined __SANITIZE_ADDRESS__ || \
     __has_attribute__(no_sanitize_address)
#   define NOSANITIZE __attribute__((no_sanitize_address))
# else
#   define NOSANITIZE
# endif
#endif

#if !defined FORCEINLINE
# if defined DEBUG
#   define FORCEINLINE __inline
# elif defined __forceinline || CC_MSVC
#   define FORCEINLINE __forceinline
# elif __has_attribute__(always_inline)
#   define FORCEINLINE __attribute__((__always_inline__)) __inline
# else
#   define FORCEINLINE __inline
# endif
#endif

#if !defined NORETURN
# if defined U_DEBUG
#   define NORETURN
# else
#   define NORETURN __noreturn
# endif
#endif

#if !defined PURE
# if __has_attribute__(pure)
#   define PURE __attribute__((__pure__))
# elif defined __pure
#   define PURE __pure
# else
#   define PURE
# endif
#endif

#if !defined CONST
# if __has_attribute__(__const)
#   define CONST __attribute__((__const__))
# elif defined __pure2
#   define CONST __pure2
# else
#   define CONST
# endif
#endif

#if !defined UNUSED
# if __has_attribute__(unused)
#   define UNUSED __attribute__((unused))
# elif defined __LCLINT__
#   define UNUSED /*@unused@*/
# elif defined __unused
#   define UNUSED __unused
# else
#   define UNUSED
# endif
#endif

#if !defined MALLOC
# if __has_attribute__(malloc)
#   define MALLOC __attribute__((malloc))
# else
#   define MALLOC
# endif
#endif

#if !defined RESTRICT
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

#endif /* !__NT_DEFS_H */
