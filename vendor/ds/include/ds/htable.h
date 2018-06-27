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

/*!@file ds/htable.h
 * @author uael
 *
 * @addtogroup ds @{
 */
#ifndef __DS_HTABLE_H
# define __DS_HTABLE_H

#include "ds/hash.h"

typedef struct {
	u32_t flag: 2;
	u32_t hash: 30;
} htable_span_t;

#define htableof(TItem) struct { \
	TItem *entries; \
	htable_span_t *spans; \
	u32_t len, bit; \
	u32_t ksz, esz; \
	eq_fn_t *eq; \
	hash_fn_t *hash; \
}

typedef htableof(u8_t) htable_t;

/*!
 *
 * @param self
 * @param ksz
 * @param esz
 * @param hash
 * @param eq
 */
__api void htable_init(htable_t *self, usize_t ksz, usize_t esz,
					   eq_fn_t *eq, hash_fn_t *hash);

/*!
 *
 * @param self
 */
__api void htable_dtor(htable_t *self);

/*!
 *
 * @param self
 * @param key
 * @return
 */
__api u32_t htable_put(htable_t *self, u8_t const *key);

/*!
 *
 * @param self
 * @param key
 * @return
 */
__api u32_t htable_get(htable_t *self, u8_t const *key);

/*!
 *
 * @param self
 * @param key
 * @return
 */
__api bool htable_has(htable_t *self, u8_t const *key);

/*!
 *
 * @param self
 * @param key
 * @return
 */
__api bool htable_del(htable_t *self, u8_t const *key);

#define htableitof(TItem) struct { \
	htable_t *table; \
	TItem *item; \
	u32_t idx; \
}

typedef htableitof(u8_t) htable_iter_t;

/*!
 *
 * @param self
 * @param htable
 */
__api void htable_iter_init(htable_iter_t *self, htable_t *htable);

/*!
 *
 * @param self
 * @return
 */
__api bool htable_iter_hasnext(htable_iter_t *self);

/*!
 *
 * @param self
 */
__api void htable_iter_next(htable_iter_t *self);

#endif /* !__DS_HTABLE_H */
/*!@} */
