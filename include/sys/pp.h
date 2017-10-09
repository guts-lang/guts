/*
 * MIT License
 *
 * Copyright (c) 2016-2017 Abel Lucas <www.github.com/uael>
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

/*!@file sys/pp.h
 * @author uael
 */
#ifndef __SYS_PP_H
# define __SYS_PP_H

/*!@def STRINGIFY(x)
 * A macro which stringifies its argument.
 * @param[in] x All the tokens which have to be stringified.
 * @return An expression of type pointer to char.
 */
#ifndef STRINGIFY
# define STRINGIFY_EX(x) #x
# define STRINGIFY(x) STRINGIFY_EX(x)
#endif

#define COUNT_OF(x) (sizeof(x)/sizeof(*(x)))

#define init(ptr, T) memset(ptr, 0, sizeof(T))

#endif /* !__SYS_PP_H */
