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

/*!@file compat/preprocessor.h
 * @author uael
 *
 * @addtogroup compat @{
 */
#ifndef __COMPAT_PREPROCESSOR_H
# define __COMPAT_PREPROCESSOR_H

#include "compat/conf.h"

#ifndef STRINGIFY
# define STRINGIFY_EX(x) #x
# define STRINGIFY(x) STRINGIFY_EX(x)
#endif

#ifndef COUNT_OF
# define COUNT_OF(x) (sizeof(x)/sizeof(*(x)))
#endif

#ifndef containerof
# define containerof(ptr, type, member) \
	((type *) ((char *) (ptr) - offsetof(type, member)))
#endif

#define PP_VA_ARGS(...) __VA_ARGS__
#define PP_EVAL(...) PP_VA_ARGS(PP_VA_ARGS(PP_VA_ARGS(__VA_ARGS__)))

#define PP_PRIMITIVE_JOIN__(x, y) x##y
#define PP_PRIMITIVE_JOIN(x, y) PP_PRIMITIVE_JOIN__(x, y)
#define PP_JOIN(x, y) PP_PRIMITIVE_JOIN(x, y)

#define PP_LPAR (
#define PP_RPAR )
#define PP_VA_PASS(...) PP_LPAR __VA_ARGS__ PP_RPAR
#ifdef _MSC_VER
# define PP_MCALL(macro, ...) \
	PP_EVAL(PP_JOIN(PP_EVAL(macro), PP_VA_PASS(__VA_ARGS__)))
#else
# define PP_MCALL(macro, ...) PP_EVAL(PP_EVAL(macro) PP_VA_PASS(__VA_ARGS__))
#endif

#define PP_VA_NARGS_PEEK(_1, _2, _3, _4, _5, _6, _7, _8, _9, N, ...) N
#define PP_VA_NARGS_RSEQ 9, 8, 7, 6, 5, 4, 3, 2, 1
#define PP_VA_NARGS(...) \
	PP_MCALL(PP_VA_NARGS_PEEK, __VA_ARGS__, PP_VA_NARGS_RSEQ)

#endif /* !__COMPAT_PREPROCESSOR_H */
/*!@} */
