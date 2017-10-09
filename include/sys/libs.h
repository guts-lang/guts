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

/*!@file sys/libs.h
 * @author uael
 */
#ifndef __SYS_LIBS_H
# define __SYS_LIBS_H

#include "std.h"

#if defined __ANDROID__
# include <sys/cdefs.h>
#elif defined __APPLE__
# include <TargetConditionals.h>
#elif defined __linux__
# include <features.h>
#endif

#if defined (_MSC_VER)
# define LIBC_MSVCRT 1
#elif defined __BIONIC__
# define LIBC_BIONIC 1
# define LIBC_BSD 1
#elif defined __UCLIBC__
# define LIBC_UCLIBC 1
#elif defined __KLIBC__
# define LIBC_KLIBC 1
# define LIBC_BSD 1
#elif defined __CRTL_VER
# define LIBC_VMS 1
# define LIBC_BSD 1
#elif defined __LIBREL__
# define LIBC_ZOS 1
# define LIBC_BSD 1
#elif defined __GLIBC__ || defined __GNU_LIBRARY__
# define LIBC_GLIBC 1
#else
# define LIBC_BSD 1
#endif

#endif /* !__SYS_LIBS_H */
