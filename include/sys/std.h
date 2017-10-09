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

/*!@file sys/std.h
 * @author uael
 */
#ifndef __SYS_STD_H
# define __SYS_STD_H

#include "plat.h"

#define VERSION_C89 198900L
#define VERSION_C90 199000L
#define VERSION_C94 199409L
#define VERSION_C99 199901L
#define VERSION_C11 201112L

#if defined __STDC_VERSION__
# if (__STDC_VERSION__ - 0 > 1)
#   define STD_C __STDC_VERSION__
# else
#   if (CC_SUNPRO - 0 >= 0x420)
#     define STD_C VERSION_C94
#   else
#     define STD_C VERSION_C90
#   endif
# endif
#else
# if defined __STDC__ \
  || defined CC_MSVC \
  || defined CC_BORLAND
#   define STD_C VERSION_C89
# endif
#endif

#if (STD_C - 0 >= VERSION_C89)
# define STD_C89
#endif
#if (STD_C - 0 >= VERSION_C90)
# define STD_C90
#endif
#if (STD_C - 0 >= VERSION_C94)
# define STD_C94
#endif
#if (STD_C - 0 >= VERSION_C99)
# define STD_C99
#endif
#if (STD_C - 0 >= VERSION_C11)
# define STD_C11
#endif

#define VERSION_CXX98 199711L

#if defined __cplusplus
# if __cplusplus - 0 == 1
#   define STD_CXX VERSION_CXX98
# else
#   define STD_CXX __cplusplus
# endif
#endif

#if (STD_CXX - 0 >= VERSION_CXX98)
# define STD_CXX98
#endif

#if defined OS_UNIX
# include <unistd.h>
#endif

#define VERSION_POSIX_1990 199009L
#define VERSION_POSIX_1992 199209L
#define VERSION_POSIX_1993 199309L
#define VERSION_POSIX_1996 199506L
#define VERSION_POSIX_2001 200112L

#if _POSIX_VERSION - 0 >= VERSION_POSIX_1990
# define STD_POSIX_1990
#endif
#if _POSIX_VERSION - 0 >= VERSION_POSIX_1992
# define STD_POSIX_1992
#endif
#if _POSIX_VERSION - 0 >= VERSION_POSIX_1993
# define STD_POSIX_1993
#endif
#if _POSIX_VERSION - 0 >= VERSION_POSIX_1996
# define STD_POSIX_1996
#endif
#if _POSIX_VERSION - 0 >= VERSION_POSIX_2001
# define STD_POSIX_2001
#endif
#if defined _POSIX_VERSION
# define STD_POSIX _POSIX_VERSION
#endif

#define VERSION_XOPEN_1989 300
#define VERSION_XOPEN_1992 400
#define VERSION_XOPEN_1995 450
#define VERSION_XOPEN_1998 500
#define VERSION_XOPEN_2003 600

#if (_XOPEN_VERSION - 0 >= 3) || defined _XOPEN_XPG3
# define STD_XOPEN_1989
#endif
#if (_XOPEN_VERSION - 0 >= 4) || defined _XOPEN_XPG4
# define STD_XOPEN_1992
#endif
#if (_XOPEN_VERSION - 0 > 4) || \
    (defined _XOPEN_UNIX && (_XOPEN_VERSION - 0 == 4))
# define STD_XOPEN_1995
#endif
#if (_XOPEN_VERSION - 0 >= VERSION_XOPEN_1998)
# define STD_XOPEN_1998
#endif
#if (_XOPEN_VERSION - 0 >= VERSION_XOPEN_2003)
# define STD_XOPEN_2003
#endif

#if defined STD_XOPEN_1998
# define STD_XOPEN _XOPEN_VERSION
#else
# if defined STD_XOPEN_1995
#   define STD_XOPEN VERSION_XOPEN_1995
# else
#   if defined STD_XOPEN_1992
#     define STD_XOPEN VERSION_XOPEN_1992
#   else
#     if defined STD_XOPEN_1989
#       define STD_XOPEN VERSION_XOPEN_1989
#     endif
#   endif
# endif
#endif

#endif /* !__SYS_STD_H */
