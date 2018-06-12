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

/*!@file compat/string.h
 * @author uael
 *
 * @addtogroup compat @{
 */
#ifndef __COMPAT_STRING_H
# define __COMPAT_STRING_H

#include "compat/types.h"

#ifdef HAS_STRING_H
# include <string.h>
#endif
#ifdef HAS_STRINGS_H
# include <strings.h>
#endif

#ifndef HAS_BZERO
void
bzero(void *ptr, usize_t n);
#endif

#ifndef HAS_STRLCPY
usize_t
strlcpy(char *__restrict dst, const char *__restrict src, usize_t dstsize);
#endif

#ifndef HAS_STRNCPY
char *
strncpy(char *dst, __const char *src, usize_t n);
#endif

#ifndef HAS_STRNLEN
usize_t
strnlen(const char *s, usize_t n);
#endif

#endif /* !__COMPAT_STRING_H */
/*!@} */
