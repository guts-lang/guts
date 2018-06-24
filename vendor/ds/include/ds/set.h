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

/*!@file ds/set.h
 * @author uael
 *
 * @addtogroup ds @{
 */
#ifndef __DS_SET_H
# define __DS_SET_H

#include "ds/htable.h"

#define setof(TItem) htableof(TItem)

#define setinit(M, EQ, HASH) htable_init( \
	(htable_t *)(M), sizeof(*(M)->entries), sizeof(*(M)->entries), \
	EQ, HASH \
)
#define setput(M, K) htable_put((htable_t *)(M), (u8_t *)(K))
#define sethas(M, K) htable_has((htable_t *)(M), (u8_t *)(K))
#define setdel(M, K) htable_del((htable_t *)(M), (u8_t *)(K))

#endif /* !__DS_SET_H */
/*!@} */
