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

#ifndef __COMPAT_TEST_H
# define __COMPAT_TEST_H

#include "wrap/conf.h"
#include "wrap/string.h"

#include <assert.h>
#include <stdio.h>

#ifndef ASSERT_F
# define ASSERT_F(...) "%s:%d: ‘%s’\n", __FILE__, __LINE__, #__VA_ARGS__
#endif

#define ASSERT(cond) do if(!(cond))exit(printf(ASSERT_F(cond))>0);while(0)
#define ASSERT_EQ(a, b) ASSERT((a) == (b))
#define ASSERT_GE(a, b) ASSERT((a) >= (b))
#define ASSERT_LE(a, b) ASSERT((a) <= (b))
#define ASSERT_NEQ(a, b) ASSERT((a) != (b))
#define ASSERT_TRUE(a) ASSERT_EQ(a, true)
#define ASSERT_FALSE(a) ASSERT_EQ(a, false)
#define ASSERT_NULL(a) ASSERT_EQ(a, NULL)
#define ASSERT_STREQ(a, b) ASSERT_EQ(0, strcmp(a, b))

#endif /* !__COMPAT_TEST_H */
