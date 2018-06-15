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

#include "compat/string.h"

#ifndef HAS_BZERO
FORCEINLINE void
bzero(void *ptr, usize_t n)
{
	memset(ptr, 0, n);
}
#endif /* !HAS_BZERO */

#ifndef HAS_STRLCPY
usize_t
strlcpy(char *__restrict dst, __const char *__restrict src, usize_t siz)
{
	char *d = dst;
	__const char *s = src;
	usize_t n = siz;

	if (n) {
		while (--n != 0) {
			if ((*d++ = *s++) == '\0')
				break;
		}
	}
	if (n == 0) {
		if (siz != 0)
			*d = '\0';
		while (*s++);
	}

	return s - src - 1;
}
#endif /* !HAS_STRLCPY */

#ifndef HAS_STRNCPY
FORCEINLINE char *
strncpy(char *s1, __const char *s2, usize_t n)
{
	usize_t size = strnlen(s2, n);
	if (size != n)
		memset(s1 + size, '\0', n - size);
	return memcpy(s1, s2, size);
}

#endif /* !HAS_STRNCPY */

#ifndef HAS_STRNLEN
FORCEINLINE usize_t
strnlen(__const char *s, usize_t n)
{
	__const char *p;

	p = s;
	while (*p && n--)
		p++;
	return (usize_t) (p - s);
}
#endif /* !HAS_STRNLEN */

