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

/*!@file ds/map.h
 * @author uael
 *
 * @addtogroup ds @{
 */
#ifndef __DS_MAP_H
# define __DS_MAP_H

#include "ds/htable.h"

#define pairof(TKey, TVal) struct { \
	TKey key; \
	TVal val; \
}
#define mapof(TKey, TVal) htableof(pairof(TKey, TVal))

#define mapinit(M, HASH, EQ) htable_init( \
	(htable_t *)(M), sizeof((M)->entries->key), sizeof(*(M)->entries), \
	HASH, EQ \
)
#define mapput(M, K, V) do { \
	usize_t __idx = htable_put((htable_t *)(M), (u8_t *)(K)); \
	(M)->entries[__idx].val = (V); \
} while (false)
#define maphas(M, K) htable_has((htable_t *)(M), (u8_t *)(K))
#define mapget(M, K) htable_get((htable_t *)(M), (u8_t *)(K))
#define mapdel(M, K) htable_del((htable_t *)(M), (u8_t *)(K))

#endif /* !__DS_MAP_H */
/*!@} */
