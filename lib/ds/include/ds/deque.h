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

/*!@file ds/deque.h
 * @author uael
 *
 * @addtogroup ds @{
 */
#ifndef __DS_DEQUE_H
# define __DS_DEQUE_H

#include "ds/sequence.h"

#define __DEQ_CUR (SEQGUARD + 1)
#define __DEQ_GUARD __DEQ_CUR
#define __DEQ_TSZ u32_t

#define __deqcur(d) SEQPROP(d, __DEQ_CUR, __DEQ_TSZ)

#define deqof(T) seqof(T)

#define deqdtor(d) seqdtor(d, __DEQ_GUARD, __DEQ_TSZ)
#define deqcur(d) SEQPROP_SAFE(d, __DEQ_CUR, __DEQ_TSZ)
#define deqlen(d) ((d) ? __seqlen(d, __DEQ_TSZ) - __deqcur(d) : (__DEQ_TSZ)0)
#define deqcap(d) seqcap(d, __DEQ_TSZ)
#define deqempty(d) ((bool)(deqlen(d) == 0))
#define deqbeg(d) (seqbeg(d) + __deqcur(d))
#define deqend(d) seqend(d, __DEQ_TSZ)
#define deqback(d) seqback(d, __DEQ_TSZ)
#define deqat(d, i) seqat(d, (i) + __deqcur(d))
#define deqgrow(d, n) seqgrow(d, n, __DEQ_GUARD, __DEQ_TSZ)
#define deqnpush(d, items, n) seqnpush(d, items, n, __DEQ_GUARD, __DEQ_TSZ)
#define deqpush(d, item) seqpush(d, item, __DEQ_GUARD, __DEQ_TSZ)
#define dequsht(d) ((d) ? ((__deqcur(d) = __deqcur(d) + 1), deqat(d, -1)) : NULL)

#endif /* !__DS_DEQUE_H */
/*!@} */
