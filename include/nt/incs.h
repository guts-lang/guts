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

/*!@file nt/libc.h
 * @author uael
 */
#ifndef __NT_LIBC_H
# define __NT_LIBC_H

#include "defs.h"

#ifdef __cplusplus
# include <cassert>
# include <cctype>
# include <cerrno>
# include <cfloat>
# include <climits>
# include <clocale>
# include <cmath>
# include <csetjmp>
# include <csignal>
# include <cstdarg>
# include <cstddef>
# include <cstdint>
# include <cstdio>
# include <cstdlib>
# include <cstring>
# include <ctime>
#elif defined __STDC__ || defined CC_MSVC
# include <assert.h>
# include <ctype.h>
# include <errno.h>
# include <float.h>
# include <limits.h>
# include <locale.h>
# include <math.h>
# include <setjmp.h>
# include <signal.h>
# include <stdarg.h>
# include <stddef.h>
# include <stdint.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <time.h>
#endif

#if __has_feature__(complex_h)
# if defined __cplusplus
#   include <ccomplex>
# else
#   include <complex.h>
# endif
#endif

#if __has_feature__(direct_h)
# include <direct.h>
#endif

#if __has_feature__(dirent_h)
# include <dirent.h>
#endif

#if __has_feature__(fcntl_h)
# include <fcntl.h>
#endif

#if __has_feature__(fenv_h)
# if defined __cplusplus
#   include <cfenv>
# else
#   include <fenv.h>
# endif
#endif

#if __has_feature__(inttypes_h)
# if defined __cplusplus
#   include <cinttypes>
# else
#   include <inttypes.h>
# endif
#endif

#if __has_feature__(io_h)
# include <io.h>
#endif

#if __has_feature__(iso646_h)
# include <iso646.h>
#endif

#if __has_feature__(malloc_h)
# include <malloc.h>
#endif

#if __has_feature__(stdalign_h)
# include <stdalign.h>
#endif

#if __has_feature__(stdatomic_h)
# include <stdatomic.h>
#endif

#if __has_feature__(stdbool_h)
# if defined __cplusplus
#   include <cstdbool>
# else
#   include <stdbool.h>
# endif
#endif

#if __has_feature__(stdint_h)
# if defined __cplusplus
#   include <cstdint>
# else
#   include <stdint.h>
# endif
#endif

#if __has_feature__(stdnoreturn_h)
# include <stdnoreturn.h>
#endif

#if __has_feature__(sys_stat_h)
# include <sys/stat.h>
#endif

#if __has_feature__(sys_types_h)
# include <sys/types.h>
#endif

#if __has_feature__(tgmath_h)
# include <tgmath.h>
#endif

#if __has_feature__(threads_h)
# include <threads.h>
#endif

#if __has_feature__(uchar_h)
# include <uchar.h>
#endif

#if __has_feature__(unistd_h)
# include <unistd.h>
#endif

#if __has_feature__(wchar_h)
# if defined __cplusplus
#   include <cwchar>
# else
#   include <wchar.h>
# endif
#endif

#if __has_feature__(wctype_h)
# if defined __cplusplus
#   include <cwctype>
# else
#   include <wctype.h>
# endif
#endif

#endif /* !__NT_LIBC_H */
