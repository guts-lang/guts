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

/*!@file ucc/plat.h
 * @author uael
 */
#ifndef __UCC_PLAT_H
# define __UCC_PLAT_H

#include "arch.h"
#include "cpu.h"
#include "os.h"

#if defined OS_IOS
# include <TargetConditionals.h>
# define PLAT_IOS
# if defined TARGET_IPHONE_SIMULATOR && TARGET_IPHONE_SIMULATOR == 1
#   define PLAT_IOS_SIMULATOR 0
# else
#   define PLAT_IOS_DEVICE 0
# endif
#endif

#if defined(__MINGW32__) || defined(__MINGW64__)
# include <_mingw.h>
# if defined(__MINGW64_VERSION_MAJOR) && defined(__MINGW64_VERSION_MINOR)
#   define PLAT_MINGW VERNO(__MINGW64_VERSION_MAJOR, __MINGW64_VERSION_MINOR, 0)
# elif defined(__MINGW32_VERSION_MAJOR) && defined(__MINGW32_VERSION_MINOR)
#   define PLAT_MINGW VERNO(__MINGW32_MAJOR_VERSION, __MINGW32_MINOR_VERSION, 0)
# else
#   define PLAT_MINGW 0
# endif
#endif

#if defined OS_WIN && \
    defined(WINAPI_FAMILY) && (WINAPI_FAMILY == WINAPI_FAMILY_DESKTOP_APP)
# define PLAT_WIN_DESKTOP 0
#endif

#if defined OS_WIN && \
    defined(WINAPI_FAMILY) && WINAPI_FAMILY == WINAPI_FAMILY_PHONE_APP
# define PLAT_WIN_PHONE 0
#endif

#if defined OS_WIN && \
    defined(WINAPI_FAMILY) && WINAPI_FAMILY == WINAPI_FAMILY_APP
# define PLAT_WIN_STORE 0
#endif

#if defined PLAT_WIN_PHONE || defined PLAT_WIN_STORE
# define PLAT_WIN_RT 0
#endif

#endif /* !__UCC_PLAT_H */
