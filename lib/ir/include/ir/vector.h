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

/*!@file ir/vector.h
 * @author uael
 *
 * @addtogroup ir.ds @{
 */
#ifndef __IR_VECTOR_H
# define __IR_VECTOR_H

#include <compat.h>

#include "ir/conf.h"

#define CAP_MIN (32)

#if __has_builtin__(__builtin_popcount)
# define ISPOW2(n) (__builtin_popcount(n) == 1)
#else
# define ISPOW2(n) (((n) != 0) && (((n) & (~(n) + 1)) == (n)))
#endif

FORCEINLINE PURE
static usize_t __roundup(usize_t n)
{
	usize_t i;
	usize_t j;

	if (n == SIZE_MAX || ISPOW2(n))
		return n;
	i = n;
	if (!(j = i & 0xFFFF0000))
		j = i;
	if (!(i = j & 0xFF00FF00))
		i = j;
	if (!(j = i & 0xF0F0F0F0))
		j = i;
	if (!(i = j & 0xCCCCCCCC))
		i = j;
	if (!(j = i & 0xAAAAAAAA))
		j = i;
	i = j << 1;
	return (i < n) ? (usize_t)SIZE_MAX : i;
}

#define vecof(T) T *

#define __veclen(v) (((usize_t *)(v))[-1])
#define __veccap(v) (((usize_t *)(v))[-2])

#define vecdtor(v) \
	((v) ? (free((usize_t *)(v) - 2), (void)((v) = NULL)) : (void)0)

#define veclen(v) \
	((v) ? __veclen(v) : (usize_t)0)

#define veccap(v) \
	((v) ? __veccap(v) : (usize_t)0)

#define vecempty(v) \
	((v) ? (bool)(__veclen(v) == 0): (bool)true)

#define vecbeg(v) \
	(v)

#define vecend(v) \
	((v) ? (v) + __veclen(v) : NULL)

#define vecback(v) \
	(veclen(v) ? (v) + __veclen(v) - 1: (v))

#define vecat(v, i) \
	((v) ? (v) + (i) : NULL)

#define vecgrow(v, n) do { \
	usize_t __n = (n) + 1; \
	if (!(v)) { \
		usize_t *__p; \
		if (__n < CAP_MIN) __n = CAP_MIN; \
		else __n = __roundup(__n); \
		__p = malloc(__n * sizeof(*(v)) + (sizeof(usize_t) * 2)); \
		*__p++ = __n; \
		*__p++ = 0; \
		(v) = (void *)(__p); \
	} else if (__n > ((usize_t *)(v))[-2]) { \
		usize_t __len, *__p; \
		__n = __roundup(__n); \
		__p = realloc((usize_t *)(v) - 2, \
			(__n * sizeof(*(v)) + (sizeof(usize_t) * 2))); \
		*__p++ = __n; \
		__len = *__p++; \
		(v) = (void *)__p; \
		bzero((char *)__p + (__len * sizeof(*(v))), \
			(__n - __len) * sizeof(*(v))); \
	} \
} while(false)

#define vecnpush(v, items, n) do { \
	usize_t __len1; \
	usize_t __n1 = (n); \
	vecgrow(v, __n1); \
	__len1 = __veclen(v); \
	memcpy((v) + __len1, items, (__n1) * sizeof(*(v))); \
	__veclen(v) = __len1 + __n1; \
} while(false)

#define vecpush(v, item) do { \
	usize_t __len1; \
	vecgrow(v, 1); \
	__len1 = __veclen(v); \
	*((v) + __len1) = (item); \
	__veclen(v) = __len1 + 1; \
} while(false)

#define vecnshift(v, items, n) do { \
	usize_t __len1; \
	usize_t __n1 = (n); \
	vecgrow(v, __n1); \
	if ((__len1 = __veclen(v))) \
		memmove((v) + __n1, (v), (__len1 + 1) * sizeof(*(v))); \
	memcpy((v), items, (__n1) * sizeof(*(v))); \
	__veclen(v) = __len1 + __n1; \
} while(false)

#define vecshift(v, item) do { \
	usize_t __len1; \
	usize_t __n1 = 1; \
	vecgrow(v, __n1); \
	if ((__len1 = __veclen(v))) \
		memmove((v) + __n1, (v), (__len1 + 1) * sizeof(*(v))); \
	*(v) = (item); \
	__veclen(v) = __len1 + __n1; \
} while(false)

#endif /* !__IR_VECTOR_H */
/*!@} */
