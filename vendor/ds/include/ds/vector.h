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

/*!@file ds/vector.h
 * @author uael
 *
 * @addtogroup ds @{
 */
#ifndef __DS_VECTOR_H
# define __DS_VECTOR_H

#include "ds/sequence.h"

#define __VEC_GUARD SEQGUARD
#define __VEC_TSZ u32_t

#define vecof(T) seqof(T)

#define vecdtor(v) seqdtor(v, __VEC_GUARD, __VEC_TSZ)
#define veclen(v) seqlen(v, __VEC_TSZ)
#define veccap(v) seqcap(v, __VEC_TSZ)
#define vecempty(v) seqempty(v, __VEC_TSZ)
#define vecbeg(v) seqbeg(v)
#define vecend(v) seqend(v, __VEC_TSZ)
#define vecback(v) seqback(v, __VEC_TSZ)
#define vecat(v, i) seqat(v, i)
#define vecpush(v, item) seqpush(v, item, __VEC_GUARD, __VEC_TSZ)
#define vecshift(v, item) seqshift(v, item, __VEC_GUARD, __VEC_TSZ)

#endif /* !__DS_VECTOR_H */
/*!@} */
