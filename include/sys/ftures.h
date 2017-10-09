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
# if (defined CC_GCC && VERNO_GE(CC_GCC, 2, 95)) || \
     (defined CC_CRAY)
#   define __HAS_ATTRIBUTE_alias 1
# else
#   define __HAS_ATTRIBUTE_alias 0
# endif
# if (defined CC_GCC && VERNO_GE(CC_GCC, 2, 95)) || \
     (defined CC_INTEL) || \
     (defined CC_CRAY) || \
     (defined CC_SUNPRO) || \
     (defined CC_DIAB)
#   define __HAS_ATTRIBUTE_aligned 1
# else
#   define __HAS_ATTRIBUTE_aligned 0
# endif
# if (defined CC_GCC && VERNO_GE(CC_GCC, 3, 1))
#   define __HAS_ATTRIBUTE_always_inline 1
# else
#   define __HAS_ATTRIBUTE_always_inline 0
# endif
# if (defined CC_GCC && VERNO_GE(CC_GCC, 2, 95))
#   define __HAS_ATTRIBUTE_cdecl 1
# else
#   define __HAS_ATTRIBUTE_cdecl 0
# endif
# if (defined CC_GCC && VERNO_GE(CC_GCC, 3, 3))
#   define __HAS_ATTRIBUTE_cleanup 1
# else
#   define __HAS_ATTRIBUTE_cleanup 0
# endif
# if (defined CC_GCC && VERNO_GE(CC_GCC, 3, 3))
#   define __HAS_ATTRIBUTE_common 1
# else
#   define __HAS_ATTRIBUTE_common 0
# endif
# if (defined CC_GCC && VERNO_GE(CC_GCC, 2, 95)) || \
     (defined CC_INTEL)
#   define __HAS_ATTRIBUTE_const 1
# else
#   define __HAS_ATTRIBUTE_const 0
# endif
# if (defined CC_GCC && VERNO_GE(CC_GCC, 2, 95))
#   define __HAS_ATTRIBUTE_constructor 1
# else
#   define __HAS_ATTRIBUTE_constructor 0
# endif
# if (defined CC_GCC && VERNO_GE(CC_GCC, 3, 1)) || \
     (defined CC_CRAY)
#   define __HAS_ATTRIBUTE_deprecated 1
# else
#   define __HAS_ATTRIBUTE_deprecated 0
# endif
# if (defined CC_GCC && VERNO_GE(CC_GCC, 4, 5)) || \
     (defined CC_CRAY)
#   define __HAS_ATTRIBUTE_deprecated_message 1
# else
#   define __HAS_ATTRIBUTE_deprecated_message 0
# endif
# if (defined CC_GCC && VERNO_GE(CC_GCC, 2, 95))
#   define __HAS_ATTRIBUTE_destructor 1
# else
#   define __HAS_ATTRIBUTE_destructor 0
# endif
# if (defined CC_GCC && VERNO_GE(CC_GCC, 2, 95))
#   define __HAS_ATTRIBUTE_dllexport 1
# else
#   define __HAS_ATTRIBUTE_dllexport 0
# endif
# if (defined CC_GCC && VERNO_GE(CC_GCC, 2, 95))
#   define __HAS_ATTRIBUTE_dllimport 1
# else
#   define __HAS_ATTRIBUTE_dllimport 0
# endif
# if (defined CC_GCC && VERNO_GE(CC_GCC, 2, 95))
#   define __HAS_ATTRIBUTE_eightbit_data 1
# else
#   define __HAS_ATTRIBUTE_eightbit_data 0
# endif
# if (defined CC_GCC && VERNO_GE(CC_GCC, 2, 95))
#   define __HAS_ATTRIBUTE_exception 1
# else
#   define __HAS_ATTRIBUTE_exception 0
# endif
# if (defined CC_GCC && VERNO_GE(CC_GCC, 3, 3))
#   define __HAS_ATTRIBUTE_far 1
# else
#   define __HAS_ATTRIBUTE_far 0
# endif
# if (defined CC_GCC && VERNO_GE(CC_GCC, 3, 4))
#   define __HAS_ATTRIBUTE_fastcall 1
# else
#   define __HAS_ATTRIBUTE_fastcall 0
# endif
# if (defined CC_GCC && VERNO_GE(CC_GCC, 2, 95)) || \
     (defined CC_CRAY)
#   define __HAS_ATTRIBUTE_format 1
# else
#   define __HAS_ATTRIBUTE_format 0
# endif
# if (defined CC_GCC && VERNO_GE(CC_GCC, 2, 95)) || \
     (defined CC_CRAY)
#   define __HAS_ATTRIBUTE_format_arg 1
# else
#   define __HAS_ATTRIBUTE_format_arg 0
# endif
# if (defined CC_GCC && VERNO_GE(CC_GCC, 2, 95))
#   define __HAS_ATTRIBUTE_function_vector 1
# else
#   define __HAS_ATTRIBUTE_function_vector 0
# endif
# if (defined CC_GCC && VERNO_GE(CC_GCC, 2, 95))
#   define __HAS_ATTRIBUTE_interrupt 1
# else
#   define __HAS_ATTRIBUTE_interrupt 0
# endif
# if (defined CC_GCC && VERNO_GE(CC_GCC, 2, 95))
#   define __HAS_ATTRIBUTE_interrupt_handler 1
# else
#   define __HAS_ATTRIBUTE_interrupt_handler 0
# endif
# if (defined CC_GCC && VERNO_GE(CC_GCC, 3, 0))
#   define __HAS_ATTRIBUTE_long_call 1
# else
#   define __HAS_ATTRIBUTE_long_call 0
# endif
# if (defined CC_GCC && VERNO_GE(CC_GCC, 2, 95))
#   define __HAS_ATTRIBUTE_longcall 1
# else
#   define __HAS_ATTRIBUTE_longcall 0
# endif
# if (defined CC_GCC && VERNO_GE(CC_GCC, 3, 0))
#   define __HAS_ATTRIBUTE_malloc 1
# else
#   define __HAS_ATTRIBUTE_malloc 0
# endif
# if (defined CC_GCC && VERNO_GE(CC_GCC, 3, 3))
#   define __HAS_ATTRIBUTE_may_alias 1
# else
#   define __HAS_ATTRIBUTE_may_alias 0
# endif
# if (defined CC_GCC && VERNO_GE(CC_GCC, 2, 95))
#   define __HAS_ATTRIBUTE_mode 1
# else
#   define __HAS_ATTRIBUTE_mode 0
# endif
# if (defined CC_GCC && VERNO_GE(CC_GCC, 2, 95))
#   define __HAS_ATTRIBUTE_model 1
# else
#   define __HAS_ATTRIBUTE_model 0
# endif
# if (defined CC_GCC && VERNO_GE(CC_GCC, 3, 0))
#   define __HAS_ATTRIBUTE_naked 1
# else
#   define __HAS_ATTRIBUTE_naked 0
# endif
# if (defined CC_GCC && VERNO_GE(CC_GCC, 3, 3))
#   define __HAS_ATTRIBUTE_near 1
# else
#   define __HAS_ATTRIBUTE_near 0
# endif
# if (defined CC_GCC && VERNO_GE(CC_GCC, 2, 95))
#   define __HAS_ATTRIBUTE_no_check_memory_usage 1
# else
#   define __HAS_ATTRIBUTE_no_check_memory_usage 0
# endif
# if (defined CC_GCC && VERNO_GE(CC_GCC, 2, 95))
#   define __HAS_ATTRIBUTE_no_instrument_function 1
# else
#   define __HAS_ATTRIBUTE_no_instrument_function 0
# endif
# if (defined CC_GCC && VERNO_GE(CC_GCC, 2, 95))
#   define __HAS_ATTRIBUTE_nocommon 1
# else
#   define __HAS_ATTRIBUTE_nocommon 0
# endif
# if (defined CC_GCC && VERNO_GE(CC_GCC, 3, 1))
#   define __HAS_ATTRIBUTE_noinline 1
# else
#   define __HAS_ATTRIBUTE_noinline 0
# endif
# if (defined CC_GCC && VERNO_GE(CC_GCC, 3, 3))
#   define __HAS_ATTRIBUTE_nonnull 1
# else
#   define __HAS_ATTRIBUTE_nonnull 0
# endif
# if (defined CC_GCC && VERNO_GE(CC_GCC, 2, 95))
#   define __HAS_ATTRIBUTE_noreturn 1
# else
#   define __HAS_ATTRIBUTE_noreturn 0
# endif
# if (defined CC_GCC && VERNO_GE(CC_GCC, 3, 3)) || \
     (defined CC_CRAY)
#   define __HAS_ATTRIBUTE_nothrow 1
# else
#   define __HAS_ATTRIBUTE_nothrow 0
# endif
# if (defined CC_GCC && VERNO_GE(CC_GCC, 2, 95)) || \
     defined CC_DIAB || \
     (defined CC_INTEL)
#   define __HAS_ATTRIBUTE_packed 1
# else
#   define __HAS_ATTRIBUTE_packed 0
# endif
# if (defined CC_GCC && VERNO_GE(CC_GCC, 3, 0))
#   define __HAS_ATTRIBUTE_pure 1
# else
#   define __HAS_ATTRIBUTE_pure 0
# endif
# if (defined CC_GCC && VERNO_GE(CC_GCC, 2, 95))
#   define __HAS_ATTRIBUTE_regparm 1
# else
#   define __HAS_ATTRIBUTE_regparm 0
# endif
# if (defined CC_GCC && VERNO_GE(CC_GCC, 3, 4))
#   define __HAS_ATTRIBUTE_saveall 1
# else
#   define __HAS_ATTRIBUTE_saveall 0
# endif
# if (defined CC_GCC && VERNO_GE(CC_GCC, 2, 95)) || \
     (defined CC_INTEL)
#   define __HAS_ATTRIBUTE_section 1
# else
#   define __HAS_ATTRIBUTE_section 0
# endif
# if (defined CC_GCC && VERNO_GE(CC_GCC, 3, 0))
#   define __HAS_ATTRIBUTE_shared 1
# else
#   define __HAS_ATTRIBUTE_shared 0
# endif
# if (defined CC_GCC && VERNO_GE(CC_GCC, 3, 0))
#   define __HAS_ATTRIBUTE_signal 1
# else
#   define __HAS_ATTRIBUTE_signal 0
# endif
# if (defined CC_GCC && VERNO_GE(CC_GCC, 3, 0))
#   define __HAS_ATTRIBUTE_short_call 1
# else
#   define __HAS_ATTRIBUTE_short_call 0
# endif
# if (defined CC_GCC && VERNO_GE(CC_GCC, 2, 95))
#   define __HAS_ATTRIBUTE_shortcall 1
# else
#   define __HAS_ATTRIBUTE_shortcall 0
# endif
# if (defined CC_GCC && VERNO_GE(CC_GCC, 3, 0))
#   define __HAS_ATTRIBUTE_sp_switch 1
# else
#   define __HAS_ATTRIBUTE_sp_switch 0
# endif
# if (defined CC_GCC && VERNO_GE(CC_GCC, 2, 95))
#   define __HAS_ATTRIBUTE_stdcall 1
# else
#   define __HAS_ATTRIBUTE_stdcall 0
# endif
# if (defined CC_GCC && VERNO_GE(CC_GCC, 2, 95))
#   define __HAS_ATTRIBUTE_tiny_data 1
# else
#   define __HAS_ATTRIBUTE_tiny_data 0
# endif
# if (defined CC_GCC && VERNO_GE(CC_GCC, 2, 95)) || \
     (defined CC_CRAY)
#   define __HAS_ATTRIBUTE_transparent_union 1
# else
#   define __HAS_ATTRIBUTE_transparent_union 0
# endif
# if (defined CC_GCC && VERNO_GE(CC_GCC, 3, 0))
#   define __HAS_ATTRIBUTE_trap_exit 1
# else
#   define __HAS_ATTRIBUTE_trap_exit 0
# endif
# if (defined CC_GCC && VERNO_GE(CC_GCC, 2, 95)) || \
     (defined CC_CRAY) || \
     (defined CC_INTEL)
#   define __HAS_ATTRIBUTE_unused 1
# else
#   define __HAS_ATTRIBUTE_unused 0
# endif
# if (defined CC_GCC && VERNO_GE(CC_GCC, 3, 1)) || \
     (defined CC_CRAY) || \
     (defined CC_INTEL)
#   define __HAS_ATTRIBUTE_used 1
# else
#   define __HAS_ATTRIBUTE_used 0
# endif
# if (defined CC_GCC && VERNO_GE(CC_GCC, 3, 1))
#   define __HAS_ATTRIBUTE_vector_size 1
# else
#   define __HAS_ATTRIBUTE_vector_size 0
# endif
# if (defined CC_GCC && VERNO_GE(CC_GCC, 3, 3)) || \
     (defined CC_SYSC && VERNO_GE(CC_SYSC, 1, 9)) || \
     (defined CC_HPCC && VERNO_GE(CC_HPCC, 6, 26))
#   define __HAS_ATTRIBUTE_visibility 1
# else
#   define __HAS_ATTRIBUTE_visibility 0
# endif
# if (defined CC_GCC && VERNO_GE(CC_GCC, 3, 3)) || \
     (defined CC_CRAY) || \
     (defined CC_HPCC && VERNO_GE(CC_HPCC, 6, 26))
#   define __HAS_ATTRIBUTE_warn_unused_result 1
# else
#   define __HAS_ATTRIBUTE_warn_unused_result 0
# endif
# if (defined CC_GCC && VERNO_GE(CC_GCC, 2, 95)) || \
     (defined CC_CRAY)
#   define __HAS_ATTRIBUTE_weak 1
# else
#   define __HAS_ATTRIBUTE_weak 0
# endif
#endif

#if !defined __has_builtin__
# define __has_builtin__(x) (__has_builtin(x) || __HAS_BUILTIN_##x)
#endif

#if !defined __has_declspec_attribute__
# define __has_declspec_attribute__(x) \
  (__has_declspec_attribute(x) || __HAS_DECLSPEC_ATTRIBUTE_##x)
# if defined CC_MSVC && VERNO_GE(CC_MSVC, 13, 0)
#   define __HAS_DECLSPEC_ATTRIBUTE_align 1
# else
#   define __HAS_DECLSPEC_ATTRIBUTE_align 0
# endif
# if defined CC_MSVC && VERNO_GE(CC_MSVC, 13, 0)
#   define __HAS_DECLSPEC_ATTRIBUTE_deprecated 1
# else
#   define __HAS_DECLSPEC_ATTRIBUTE_deprecated 0
# endif
# if defined CC_COMEAU || \
     (defined CC_DMC) || \
     (defined CC_MSVC && VERNO_GE(CC_MSVC, 12, 0))
#   define __HAS_DECLSPEC_ATTRIBUTE_dllexport 1
# else
#   define __HAS_DECLSPEC_ATTRIBUTE_dllexport 0
# endif
# if defined CC_COMEAU || \
     (defined CC_DMC) || \
     ( \
      defined CC_MSVC && ( \
        defined(_M_IX86) || \
        defined(_M_IA64) || \
        defined(_M_AMD64) || \
        defined(_M_ARM) || \
        defined(_M_ARM64) \
      ) \
    )
#   define __HAS_DECLSPEC_ATTRIBUTE_dllimport 1
# else
#   define __HAS_DECLSPEC_ATTRIBUTE_dllimport 0
# endif
# if defined CC_COMEAU || \
     (defined CC_DMC) || \
     (defined CC_MSVC && VERNO_GE(CC_MSVC, 12, 0))
#   define __HAS_DECLSPEC_ATTRIBUTE_naked 1
# else
#   define __HAS_DECLSPEC_ATTRIBUTE_naked 0
# endif
# if defined CC_MSVC && VERNO_GE(CC_MSVC, 13, 0)
#   define __HAS_DECLSPEC_ATTRIBUTE_noinline 1
# else
#   define __HAS_DECLSPEC_ATTRIBUTE_noinline 0
# endif
# if defined CC_MSVC && VERNO_GE(CC_MSVC, 12, 0)
#   define __HAS_DECLSPEC_ATTRIBUTE_noreturn 1
# else
#   define __HAS_DECLSPEC_ATTRIBUTE_noreturn 0
# endif
# if defined CC_MSVC && VERNO_GE(CC_MSVC, 12, 0)
#   define __HAS_DECLSPEC_ATTRIBUTE_nothrow 1
# else
#   define __HAS_DECLSPEC_ATTRIBUTE_nothrow 0
# endif
# if defined CC_MSVC && VERNO_GE(CC_MSVC, 16, 0)
#   define __HAS_DECLSPEC_ATTRIBUTE_safebuffers 1
# else
#   define __HAS_DECLSPEC_ATTRIBUTE_safebuffers 0
# endif
# if defined CC_MSVC && VERNO_GE(CC_MSVC, 11, 0)
#   define __HAS_DECLSPEC_ATTRIBUTE_selectany 1
# else
#   define __HAS_DECLSPEC_ATTRIBUTE_selectany 0
# endif
# if defined CC_COMEAU || \
     (defined CC_DMC) || \
     (defined CC_INTEL) || \
     (defined CC_BORLAND) || \
     (defined CC_MSVC && VERNO_GE(CC_MSVC, 12, 0))
#   define __HAS_DECLSPEC_ATTRIBUTE_thread 1
# else
#   define __HAS_DECLSPEC_ATTRIBUTE_thread 0
# endif
#endif

#if !defined __has_feature__
# define __has_feature__(x) (__has_feature(x) || __HAS_FEATURE_##x)
# if defined STD_C11 || \
     __has_include(<stdalign.h>) || \
     (defined CC_GCC && VERNO_GE(CC_GCC, 4, 7))
#   define __HAS_FEATURE_c_alignas 1
# else
#   define __HAS_FEATURE_c_alignas 0
# endif
# if defined STD_C11 || \
     __has_include(<stdalign.h>) || \
     (defined CC_GCC && VERNO_GE(CC_GCC, 4, 7))
#   define __HAS_FEATURE_c_alignof 1
# else
#   define __HAS_FEATURE_c_alignof 0
# endif
# if !defined __STDC_NO_ATOMICS && !defined CC_INTEL && ( \
     defined STD_C11 || \
     __has_include(<stdatomic.h>) || \
     (defined CC_GCC && VERNO_GE(CC_GCC, 4, 9)))
#   define __HAS_FEATURE_c_atomic 1
# else
#   define __HAS_FEATURE_c_atomic 0
# endif
# if defined STD_C99 || \
     __has_include(<stdbool.h>) || \
     (defined CC_GCC && VERNO_GE(CC_GCC, 4, 9)) || \
     (defined CC_MSVC && VERNO_GE(CC_MSVC, 12, 0))
#   define __HAS_FEATURE_c_bool 1
# else
#   define __HAS_FEATURE_c_bool 0
# endif
# if defined STD_C99 || \
     __has_include(<complex.h>) || \
     (defined CC_GCC && defined STD_C90)
#   define __HAS_FEATURE_c_complex 1
# else
#   define __HAS_FEATURE_c_complex 0
# endif
# if defined STD_C11 || \
     (defined CC_GCC && VERNO_GE(CC_GCC, 4, 9))
#   define __HAS_FEATURE_c_generic_selections 1
# else
#   define __HAS_FEATURE_c_generic_selections 0
# endif
# if defined STD_C11 || \
     defined _Noreturn || \
     __has_include(<stdnoreturn.h>) || \
     (defined CC_CLANG && VERNO_GT(CC_CLANG, 3, 2))
#   define __HAS_FEATURE_c_noreturn 1
# else
#   define __HAS_FEATURE_c_noreturn 0
# endif
# if defined STD_C11 || \
     (defined CC_GCC && VERNO_GE(CC_GCC, 4, 6))
#   define __HAS_FEATURE_c_static_assert 1
# else
#   define __HAS_FEATURE_c_static_assert 0
# endif
# if defined STD_C11 || \
     (defined CC_GCC && VERNO_GE(CC_GCC, 4, 9))
#   define __HAS_FEATURE_c_thread_local 1
# else
#   define __HAS_FEATURE_c_thread_local 0
# endif
# if __has_include(<complex.h>) || \
     defined STD_C99 || \
     __has_feature__(c_complex) || \
     (defined CC_MSVC && VERNO_GE(CC_MSVC, 18, 0))
#   define __HAS_FEATURE_complex_h 1
# else
#   define __HAS_FEATURE_complex_h 0
# endif
# if __has_include(<direct.h>) || \
     (defined CC_MSVC && VERNO_GE(CC_MSVC, 15, 0))
#   define __HAS_FEATURE_direct_h 1
# else
#   define __HAS_FEATURE_direct_h 0
# endif
# if __has_include(<dirent.h>) || \
     defined STD_POSIX || \
     defined STD_XOPEN
#   define __HAS_FEATURE_dirent_h 1
# else
#   define __HAS_FEATURE_dirent_h 0
# endif
# if __has_include(<fcntl.h>) || \
     (defined CC_MSVC && VERNO_GE(CC_MSVC, 15, 0))
#   define __HAS_FEATURE_fcntl_h 1
# else
#   define __HAS_FEATURE_fcntl_h 0
# endif
# if __has_include(<fenv.h>) || \
     defined STD_C99 || \
     (defined CC_MSVC && VERNO_GE(CC_MSVC, 18, 0))
#   define __HAS_FEATURE_fenv_h 1
# else
#   define __HAS_FEATURE_fenv_h 0
# endif
# if __has_include(<fenv.h>) || \
     defined STD_C99 || \
     (defined CC_MSVC && VERNO_GE(CC_MSVC, 18, 0))
#   define __HAS_FEATURE_inttypes_h 1
# else
#   define __HAS_FEATURE_inttypes_h 0
# endif
# if defined CC_MSVC && VERNO_GE(CC_MSVC, 15, 0)
#   define __HAS_FEATURE_io_h 1
# else
#   define __HAS_FEATURE_io_h 0
# endif
# if __has_include(<iso646.h>) || \
     defined STD_POSIX || \
     defined STD_XOPEN
#   define __HAS_FEATURE_iso646_h 1
# else
#   define __HAS_FEATURE_iso646_h 0
# endif
# if defined CC_MSVC && VERNO_GE(CC_MSVC, 13, 0)
#   define __HAS_FEATURE_malloc_h 1
# else
#   define __HAS_FEATURE_malloc_h 0
# endif
# if __has_include(<stdalign.h>) || \
     defined STD_C11 || \
     __has_feature__(c_alignas) || \
     __has_feature__(c_alignof)
#   define __HAS_FEATURE_stdalign_h 1
# else
#   define __HAS_FEATURE_stdalign_h 0
# endif
# if __has_include(<stdatomic.h>) || \
     defined STD_C11 || \
     __has_feature__(c_atomic)
#   define __HAS_FEATURE_stdatomic_h 1
# else
#   define __HAS_FEATURE_stdatomic_h 0
# endif
# if __has_include(<stdbool.h>) || \
     defined STD_C99 || \
     __has_feature__(c_bool) || \
     (defined CC_MSVC && VERNO_GE(CC_MSVC, 18, 0))
#   define __HAS_FEATURE_stdbool_h 1
# else
#   define __HAS_FEATURE_stdbool_h 0
# endif
# if __has_include(<stdint.h>) || \
     defined STD_C99 || \
     (defined CC_MSVC && VERNO_GE(CC_MSVC, 17, 0))
#   define __HAS_FEATURE_stdint_h 1
# else
#   define __HAS_FEATURE_stdint_h 0
# endif
# if __has_include(<stdnoreturn.h>) || \
     defined STD_C11 || \
     __has_feature__(c_noreturn)
#   define __HAS_FEATURE_stdnoreturn_h 1
# else
#   define __HAS_FEATURE_stdnoreturn_h 0
# endif
# if __has_include(<sys/stat.h>) || \
     defined STD_POSIX || \
     defined STD_XOPEN || \
     (defined CC_MSVC && VERNO_GE(CC_MSVC, 13, 0))
#   define __HAS_FEATURE_sys_stat_h 1
# else
#   define __HAS_FEATURE_sys_stat_h 0
# endif
# if __has_include(<sys/types.h>) || \
     defined STD_POSIX || \
     defined STD_XOPEN || \
     (defined CC_MSVC && VERNO_GE(CC_MSVC, 13, 0))
#   define __HAS_FEATURE_sys_types_h 1
# else
#   define __HAS_FEATURE_sys_types_h 0
# endif
# if __has_include(<tgmath.h>) || \
     defined STD_C99
#   define __HAS_FEATURE_tgmath_h 1
# else
#   define __HAS_FEATURE_tgmath_h 0
# endif
# if (!defined OS_CYGWIN && __has_include(<threads.h>)) || \
     defined STD_C11
#   define __HAS_FEATURE_threads_h 1
# else
#   define __HAS_FEATURE_threads_h 0
# endif
# if __has_include(<uchar.h>) || \
     defined STD_C11 || \
     (defined CC_MSVC && VERNO_GE(CC_MSVC, 18, 0))
#   define __HAS_FEATURE_uchar_h 1
# else
#   define __HAS_FEATURE_uchar_h 0
# endif
# if __has_include(<unistd.h>) || \
     defined STD_POSIX || \
     defined STD_XOPEN
#   define __HAS_FEATURE_unistd_h 1
# else
#   define __HAS_FEATURE_unistd_h 0
# endif
# if __has_include(<wchar.h>) || \
     defined STD_C90 || \
     (defined CC_MSVC && VERNO_GE(CC_MSVC, 15, 0))
#   define __HAS_FEATURE_wchar_h 1
# else
#   define __HAS_FEATURE_wchar_h 0
# endif
# if __has_include(<wctype.h>) || \
     defined STD_C90 || \
     (defined CC_MSVC && VERNO_GE(CC_MSVC, 15, 0))
#   define __HAS_FEATURE_wctype_h 1
# else
#   define __HAS_FEATURE_wctype_h 0
# endif
#endif

#if !defined __has_extension__
# define __has_extension__(x) (__has_feature__(x) || __has_extension(x) || \
  __HAS_BUILTIN_##x)
#endif

#endif /* !__SYS_FEATURE_H */
