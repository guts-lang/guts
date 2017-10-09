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

/*!@file sys/feature.h
 * @author uael
 */
#ifndef __SYS_FEATURE_H
# define __SYS_FEATURE_H

#include "libs.h"

#ifndef __has_attribute
# define __has_attribute(x) 0
#endif

#ifndef __has_builtin
# define __has_builtin(x) 0
#endif

#ifndef __has_declspec_attribute
# define __has_declspec_attribute(x) 0
#endif

#ifndef __has_feature
# define __has_feature(x) 0
#endif

#ifndef __has_include
# define __has_include(x) 0
#endif

#ifndef __has_extension
# define __has_extension(x) __has_feature(x)
#endif

#if !defined __has_attribute__
# define __has_attribute__(x) (__has_attribute(x) || __HAS_ATTRIBUTE_##x)
# define __HAS_ATTRIBUTE_alias ( \
    (defined CC_GCC && VERNO_GE(CC_GCC, 2, 95)) \
    || (defined CC_CRAY) \
  )
# define __HAS_ATTRIBUTE_aligned ( \
    (defined CC_GCC && VERNO_GE(CC_GCC, 2, 95)) \
    || (defined CC_INTEL) \
    || (defined CC_CRAY) \
    || (defined CC_SUNPRO) \
    || (defined CC_DIAB) \
  )
# define __HAS_ATTRIBUTE_always_inline ( \
    (defined CC_GCC && VERNO_GE(CC_GCC, 3, 1)) \
  )
# define __HAS_ATTRIBUTE_cdecl ( \
    (defined CC_GCC && VERNO_GE(CC_GCC, 2, 95)) \
  )
# define __HAS_ATTRIBUTE_cleanup ( \
    (defined CC_GCC && VERNO_GE(CC_GCC, 3, 3)) \
  )
# define __HAS_ATTRIBUTE_common ( \
    (defined CC_GCC && VERNO_GE(CC_GCC, 3, 3)) \
  )
# define __HAS_ATTRIBUTE_const ( \
    (defined CC_GCC && VERNO_GE(CC_GCC, 2, 95)) \
    || (defined CC_INTEL) \
  )
# define __HAS_ATTRIBUTE_constructor ( \
    (defined CC_GCC && VERNO_GE(CC_GCC, 2, 95)) \
  )
# define __HAS_ATTRIBUTE_deprecated ( \
    (defined CC_GCC && VERNO_GE(CC_GCC, 3, 1)) \
    || (defined CC_CRAY) \
  )
# define __HAS_ATTRIBUTE_deprecated_message ( \
    (defined CC_GCC && VERNO_GE(CC_GCC, 4, 5)) \
    || (defined CC_CRAY) \
  )
# define __HAS_ATTRIBUTE_destructor ( \
    (defined CC_GCC && VERNO_GE(CC_GCC, 2, 95)) \
  )
# define __HAS_ATTRIBUTE_dllexport ( \
    (defined CC_GCC && VERNO_GE(CC_GCC, 2, 95)) \
  )
# define __HAS_ATTRIBUTE_dllimport ( \
    (defined CC_GCC && VERNO_GE(CC_GCC, 2, 95)) \
  )
# define __HAS_ATTRIBUTE_eightbit_data ( \
    (defined CC_GCC && VERNO_GE(CC_GCC, 2, 95)) \
  )
# define __HAS_ATTRIBUTE_exception  ( \
    (defined CC_GCC && VERNO_GE(CC_GCC, 2, 95)) \
  )
# define __HAS_ATTRIBUTE_far  ( \
    (defined CC_GCC && VERNO_GE(CC_GCC, 3, 3)) \
  )
# define __HAS_ATTRIBUTE_fastcall  ( \
    (defined CC_GCC && VERNO_GE(CC_GCC, 3, 4)) \
  )
# define __HAS_ATTRIBUTE_format ( \
    (defined CC_GCC && VERNO_GE(CC_GCC, 2, 95)) \
    || (defined CC_CRAY) \
  )
# define __HAS_ATTRIBUTE_format_arg ( \
    (defined CC_GCC && VERNO_GE(CC_GCC, 2, 95)) \
    || (defined CC_CRAY) \
  )
# define __HAS_ATTRIBUTE_function_vector ( \
    (defined CC_GCC && VERNO_GE(CC_GCC, 2, 95)) \
  )
# define __HAS_ATTRIBUTE_interrupt ( \
    (defined CC_GCC && VERNO_GE(CC_GCC, 2, 95)) \
  )
# define __HAS_ATTRIBUTE_interrupt_handler ( \
    (defined CC_GCC && VERNO_GE(CC_GCC, 2, 95)) \
  )
# define __HAS_ATTRIBUTE_long_call ( \
    (defined CC_GCC && VERNO_GE(CC_GCC, 3, 0)) \
  )
# define __HAS_ATTRIBUTE_longcall ( \
    (defined CC_GCC && VERNO_GE(CC_GCC, 2, 95)) \
  )
# define __HAS_ATTRIBUTE_malloc ( \
    (defined CC_GCC && VERNO_GE(CC_GCC, 3, 0)) \
  )
# define __HAS_ATTRIBUTE_may_alias ( \
    (defined CC_GCC && VERNO_GE(CC_GCC, 3, 3)) \
  )
# define __HAS_ATTRIBUTE_mode ( \
    (defined CC_GCC && VERNO_GE(CC_GCC, 2, 95)) \
  )
# define __HAS_ATTRIBUTE_model ( \
    (defined CC_GCC && VERNO_GE(CC_GCC, 2, 95)) \
  )
# define __HAS_ATTRIBUTE_naked ( \
    (defined CC_GCC && VERNO_GE(CC_GCC, 3, 0)) \
  )
# define __HAS_ATTRIBUTE_near ( \
    (defined CC_GCC && VERNO_GE(CC_GCC, 3, 3)) \
  )
# define __HAS_ATTRIBUTE_no_check_memory_usage ( \
    (defined CC_GCC && VERNO_GE(CC_GCC, 2, 95)) \
  )
# define __HAS_ATTRIBUTE_no_instrument_function ( \
    (defined CC_GCC && VERNO_GE(CC_GCC, 2, 95)) \
  )
# define __HAS_ATTRIBUTE_nocommon ( \
    (defined CC_GCC && VERNO_GE(CC_GCC, 2, 95)) \
  )
# define __HAS_ATTRIBUTE_noinline ( \
    (defined CC_GCC && VERNO_GE(CC_GCC, 3, 1)) \
  )
# define __HAS_ATTRIBUTE_nonnull ( \
    (defined CC_GCC && VERNO_GE(CC_GCC, 3, 3)) \
  )
# define __HAS_ATTRIBUTE_noreturn ( \
    (defined CC_GCC && VERNO_GE(CC_GCC, 2, 95)) \
  )
# define __HAS_ATTRIBUTE_nothrow ( \
    (defined CC_GCC && VERNO_GE(CC_GCC, 3, 3)) \
    || (defined CC_CRAY) \
  )
# define __HAS_ATTRIBUTE_packed ( \
    (defined CC_GCC && VERNO_GE(CC_GCC, 2, 95)) \
    || defined CC_DIAB \
    || (defined CC_INTEL) \
  )
# define __HAS_ATTRIBUTE_pure ( \
    (defined CC_GCC && VERNO_GE(CC_GCC, 3, 0)) \
  )
# define __HAS_ATTRIBUTE_regparm ( \
    (defined CC_GCC && VERNO_GE(CC_GCC, 2, 95)) \
  )
# define __HAS_ATTRIBUTE_saveall ( \
    (defined CC_GCC && VERNO_GE(CC_GCC, 3, 4)) \
  )
# define __HAS_ATTRIBUTE_section ( \
    (defined CC_GCC && VERNO_GE(CC_GCC, 2, 95)) \
    || (defined CC_INTEL) \
  )
# define __HAS_ATTRIBUTE_shared ( \
    (defined CC_GCC && VERNO_GE(CC_GCC, 3, 0)) \
  )
# define __HAS_ATTRIBUTE_signal ( \
    (defined CC_GCC && VERNO_GE(CC_GCC, 3, 0)) \
  )
# define __HAS_ATTRIBUTE_short_call ( \
    (defined CC_GCC && VERNO_GE(CC_GCC, 3, 0)) \
  )
# define __HAS_ATTRIBUTE_shortcall ( \
    (defined CC_GCC && VERNO_GE(CC_GCC, 2, 95)) \
  )
# define __HAS_ATTRIBUTE_sp_switch ( \
    (defined CC_GCC && VERNO_GE(CC_GCC, 3, 0)) \
  )
# define __HAS_ATTRIBUTE_stdcall ( \
    (defined CC_GCC && VERNO_GE(CC_GCC, 2, 95)) \
  )
# define __HAS_ATTRIBUTE_tiny_data ( \
    (defined CC_GCC && VERNO_GE(CC_GCC, 2, 95)) \
  )
# define __HAS_ATTRIBUTE_transparent_union ( \
    (defined CC_GCC && VERNO_GE(CC_GCC, 2, 95)) \
    || (defined CC_CRAY) \
  )
# define __HAS_ATTRIBUTE_trap_exit ( \
    (defined CC_GCC && VERNO_GE(CC_GCC, 3, 0)) \
  )
# define __HAS_ATTRIBUTE_unused ( \
    (defined CC_GCC && VERNO_GE(CC_GCC, 2, 95)) \
    || (defined CC_CRAY) \
    || (defined CC_INTEL) \
  )
# define __HAS_ATTRIBUTE_used ( \
    (defined CC_GCC && VERNO_GE(CC_GCC, 3, 1)) \
    || (defined CC_CRAY) \
    || (defined CC_INTEL) \
  )
# define __HAS_ATTRIBUTE_vector_size ( \
    (defined CC_GCC && VERNO_GE(CC_GCC, 3, 1)) \
  )
# define __HAS_ATTRIBUTE_visibility ( \
    (defined CC_GCC && VERNO_GE(CC_GCC, 3, 3)) \
    || (defined CC_SYSC && VERNO_GE(CC_SYSC, 1, 9)) \
    || (defined CC_HPCC && VERNO_GE(CC_HPCC, 6, 26)) \
  )
# define __HAS_ATTRIBUTE_warn_unused_result ( \
    (defined CC_GCC && VERNO_GE(CC_GCC, 3, 3)) \
    || (defined CC_CRAY) \
    || (defined CC_HPCC && VERNO_GE(CC_HPCC, 6, 26)) \
  )
# define __HAS_ATTRIBUTE_weak ( \
    (defined CC_GCC && VERNO_GE(CC_GCC, 2, 95)) \
    || (defined CC_CRAY) \
  )
#endif

#if !defined __has_builtin__
# define __has_builtin__(x) (__has_builtin(x) || __HAS_BUILTIN_##x)
#endif

#if !defined __has_declspec_attribute__
# define __has_declspec_attribute__(x) \
  (__has_declspec_attribute(x) || __HAS_DECLSPEC_ATTRIBUTE_##x)
# define __HAS_DECLSPEC_ATTRIBUTE_align ( \
    defined CC_MSVC && VERNO_GE(CC_MSVC, 13, 0) \
  )
# define __HAS_DECLSPEC_ATTRIBUTE_deprecated ( \
    defined CC_MSVC && VERNO_GE(CC_MSVC, 13, 0) \
  )
# define __HAS_DECLSPEC_ATTRIBUTE_dllexport ( \
    defined CC_COMEAU \
    || (defined CC_DMC) \
    || (defined CC_MSVC && VERNO_GE(CC_MSVC, 12, 0)) \
  )
# define __HAS_DECLSPEC_ATTRIBUTE_dllimport ( \
    defined CC_COMEAU \
    || (defined CC_DMC) \
    || ( \
      defined CC_MSVC && ( \
        defined(_M_IX86) \
        || defined(_M_IA64) \
        || defined(_M_AMD64) \
        || defined(_M_ARM) \
        || defined(_M_ARM64) \
      ) \
    ) \
  )
# define __HAS_DECLSPEC_ATTRIBUTE_naked ( \
    defined CC_COMEAU \
    || (defined CC_DMC) \
    || (defined CC_MSVC && VERNO_GE(CC_MSVC, 12, 0)) \
  )
# define __HAS_DECLSPEC_ATTRIBUTE_noinline ( \
    defined CC_MSVC && VERNO_GE(CC_MSVC, 13, 0) \
  )
# define __HAS_DECLSPEC_ATTRIBUTE_noreturn ( \
    defined CC_MSVC && VERNO_GE(CC_MSVC, 12, 0) \
  )
# define __HAS_DECLSPEC_ATTRIBUTE_nothrow ( \
    defined CC_MSVC && VERNO_GE(CC_MSVC, 12, 0) \
  )
# define __HAS_DECLSPEC_ATTRIBUTE_safebuffers ( \
    defined CC_MSVC && VERNO_GE(CC_MSVC, 16, 0) \
  )
# define __HAS_DECLSPEC_ATTRIBUTE_selectany ( \
    defined CC_MSVC && VERNO_GE(CC_MSVC, 11, 0) \
  )
# define __HAS_DECLSPEC_ATTRIBUTE_thread ( \
    defined CC_COMEAU \
    || (defined CC_DMC) \
    || (defined CC_INTEL) \
    || (defined CC_BORLAND) \
    || (defined CC_MSVC && VERNO_GE(CC_MSVC, 12, 0)) \
  )
#endif

#if !defined __has_feature__
# define __has_feature__(x) (__has_feature(x) || __HAS_FEATURE_##x)
# define __HAS_FEATURE_c_alignas ( \
    defined STD_C11 \
    || __has_include(<stdalign.h>) \
    || (defined CC_GCC && VERNO_GE(CC_GCC, 4, 7)) \
  )
# define __HAS_FEATURE_c_alignof ( \
    defined STD_C11 \
    || __has_include(<stdalign.h>) \
    || (defined CC_GCC && VERNO_GE(CC_GCC, 4, 7)) \
  )
# define __HAS_FEATURE_c_atomic ( \
    !defined __STDC_NO_ATOMICS && !defined CC_INTEL \
    && ( \
      defined STD_C11 \
      || __has_include(<stdatomic.h>) \
      || (defined CC_GCC && VERNO_GE(CC_GCC, 4, 9)) \
    ) \
  )
# define __HAS_FEATURE_c_bool ( \
    defined STD_C99 \
    || __has_include(<stdbool.h>) \
    || (defined CC_GCC && VERNO_GE(CC_GCC, 4, 9)) \
    || (defined CC_MSVC && VERNO_GE(CC_MSVC, 12, 0)) \
  )
# define __HAS_FEATURE_c_complex ( \
    defined STD_C99 \
    || __has_include(<complex.h>) \
    || (defined CC_GCC && defined STD_C90) \
  )
# define __HAS_FEATURE_c_generic_selections ( \
    defined STD_C11 \
    || (defined CC_GCC && VERNO_GE(CC_GCC, 4, 9)) \
  )
# define __HAS_FEATURE_c_noreturn ( \
    defined STD_C11 \
    || defined _Noreturn \
    || __has_include(<stdnoreturn.h>) \
    || (defined CC_CLANG && VERNO_GT(CC_CLANG, 3, 2)) \
  )
# define __HAS_FEATURE_c_static_assert ( \
    defined STD_C11 \
    || (defined CC_GCC && VERNO_GE(CC_GCC, 4, 6)) \
  )
# define __HAS_FEATURE_c_thread_local ( \
    defined STD_C11 \
    || (defined CC_GCC && VERNO_GE(CC_GCC, 4, 9)) \
  )
# if defined CC_MSVC
#   include <ctype.h>
#   if defined __STDC_SECURE_LIB__ && __STDC_SECURE_LIB__ >= 200411L
#     define __HAS_FEATURE_complex_h 1
#     define __HAS_FEATURE_fcntl_h 1
#     define __HAS_FEATURE_fenv_h 1
#     define __HAS_FEATURE_inttypes_h 1
#     define __HAS_FEATURE_io_h 1
#     define __HAS_FEATURE_stdint_h 1
#     define __HAS_FEATURE_stdbool_h 1
#     define __HAS_FEATURE_sys_stat_h 1
#     define __HAS_FEATURE_sys_types_h 1
#     define __HAS_FEATURE_uchar_h 1
#     define __HAS_FEATURE_wchar_h 1
#     define __HAS_FEATURE_wctype_h 1
#   else
#     error "Download and install the lastest ucrt from Microsoft."
#   endif
# endif
#endif

#if !defined __has_extension__
# define __has_extension__(x) (__has_extension(x) || __HAS_BUILTIN_##x)
#endif

#endif /* !__SYS_FEATURE_H */
