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

/*!@file ds/stack.h
 * @author uael
 *
 * @addtogroup ds @{
 */
#ifndef __DS_STACK_H
# define __DS_STACK_H

#include "ds/sequence.h"

#define __STACK_GUARD SEQGUARD
#define __STACK_TSZ u32_t

#define stackof(T) seqof(T)

#define stackdtor(s) seqdtor(s, __STACK_GUARD, __STACK_TSZ)
#define stacklen(s) seqlen(s, __STACK_TSZ)
#define stackcap(s) seqcap(s, __STACK_TSZ)
#define stackempty(s) seqempty(s, __STACK_TSZ)
#define stackbeg(s) seqbeg(s)
#define stackend(s) seqend(s, __STACK_TSZ)
#define stackback(s) seqback(s, __STACK_TSZ)
#define stackat(s, i) seqat(s, i)
#define stackgrow(s, n) seqgrow(s, n, __STACK_GUARD, __STACK_TSZ)
#define stackpush(s, item) seqpush(s, item, __STACK_GUARD, __STACK_TSZ)
#define stackpop(s) (stacklen(s) ? ((s) + __seqlen(s, __STACK_TSZ)--) : NULL)

#endif /* !__DS_STACK_H */
/*!@} */
