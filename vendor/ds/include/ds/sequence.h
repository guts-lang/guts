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

/*!@file ds/sequence.h
 * @author uael
 *
 * @addtogroup ds @{
 */
#ifndef __DS_SEQUENCE_H
# define __DS_SEQUENCE_H

#include <wrap.h>

#include "ds/conf.h"

#define CAP_MIN (32)

#if __has_builtin__(__builtin_popcount)
# define ISPOW2(n) (__builtin_popcount(n) == 1)
#else
# define ISPOW2(n) (((n) != 0) && (((n) & (~(n) + 1)) == (n)))
#endif

#define SEQLEN (1)
#define SEQCAP (2)
#define SEQGUARD SEQCAP
#define SEQPROP(s, idx, TSz) (((TSz *)(s))[-(idx)])
#define SEQPROP_SAFE(s, idx, TSz) ((s) ? (((TSz *)(s))[-(idx)]) : (TSz)0)

#define __seqlen(s, TSz) SEQPROP(s, SEQLEN, TSz)
#define __seqcap(s, TSz) SEQPROP(s, SEQCAP, TSz)

#define seqof(T) T *

#define seqdtor(s, guard, TSz) \
	((s) ? (free((TSz *)(s) - (guard)), (void)((s) = NULL)) : (void)0)

#define seqlen(s, TSz) \
	SEQPROP_SAFE(s, SEQLEN, TSz)

#define seqcap(s, TSz) \
	SEQPROP_SAFE(s, SEQCAP, TSz)

#define seqempty(v, TSz) \
	((s) ? (bool)(__seqlen(s, TSz) == 0): (bool)true)

#define seqbeg(s) \
	(s)

#define seqend(s, TSz) \
	((s) + __seqlen(s, TSz))

#define seqback(s, TSz) \
	((s) + __seqlen(s, TSz) - 1)

#define seqat(s, i) \
	((s) + (i))

#define seqgrow(s, guard, TSz) do { \
	if (!(s)) { \
		TSz *__p; \
		__p = calloc(1, CAP_MIN * sizeof(*(s)) + (sizeof(TSz) * (guard))); \
		(s) = (void *)(__p + (guard)); \
		__seqcap(s, TSz) = CAP_MIN; \
	} else if (__seqlen(s, TSz) + 1 >= __seqcap(s, TSz)) { \
		TSz __n, __len, *__p; \
		__n = __seqcap(s, TSz) * 2; \
		__p = realloc((TSz *)(s) - (guard), \
			(__n * sizeof(*(s)) + (sizeof(TSz) * (guard)))); \
		(s) = (void *)(__p + (guard)); \
		__seqcap(s, TSz) = __n; \
		__len = __seqlen(s, TSz); \
		bzero((char *)s + (__len * sizeof(*(s))), \
			(__n - __len) * sizeof(*(s))); \
	} \
} while(false)

#define seqpush(s, item, guard, TSz) do { \
	TSz __len1; \
	seqgrow(s, guard, TSz); \
	__len1 = __seqlen(s, TSz); \
	*((s) + __len1) = (item); \
	__seqlen(s, TSz) = __len1 + 1; \
} while(false)

#define seqshift(s, item, guard, TSz) do { \
	TSz __len1; \
	seqgrow(s, 1); \
	if ((__len1 = __seqlen(s, TSz))) \
		memmove((s) + 1, (s), (__len1 + 1) * sizeof(*(s))); \
	*(s) = (item); \
	__seqlen(s, TSz) = __len1 + 1; \
} while(false)

#define seqeach(s, key, val, TSz) for ( \
	key = 0, val = (s); \
	key < seqlen(s, TSz); \
	val = (s) + ++key \
)

#define seqeachk(s, key, TSz) for ( \
	key = 0; \
	key < seqlen(s, TSz); \
	++key \
)

#define seqapply(s, fn, TSz) do for ( \
	TSz __key = 0; \
	__key < seqlen(s, TSz); \
	++__key \
) fn((s) + __key); while (0)

#endif /* !__DS_SEQUENCE_H */
/*!@} */
