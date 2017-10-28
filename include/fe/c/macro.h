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

/*!@file fe/c/macro.h
 * @author uael
 */
#ifndef __FE_C_MACRO_H
# define __FE_C_MACRO_H

#include <ds/map.h>
#include <lex/tok.h>

enum c_macro_kind {
  C_MACRO_NONE = 0,
  C_MACRO_OBJECT,
  C_MACRO_FUNC
};

typedef enum c_macro_kind c_macro_kind_t;
typedef struct c_macro c_macro_t;

struct c_macro {
  c_macro_kind_t kind : 2;
  char_t __const *name;
  bool_t is__file__;
  bool_t is__line__;
  bool_t is_vararg;
  toks_t params;
  toks_t expand;
};

__api__ void
c_macro_ctor(c_macro_t *self);

__api__ void
c_macro_dtor(c_macro_t *self);

__api__ i32_t
c_macro_cmp(c_macro_t a, c_macro_t b);

STR_MAP_DEFINE(c_macros, c_macro_t, c_macro_cmp)

#endif /* !__FE_C_MACRO_H */
