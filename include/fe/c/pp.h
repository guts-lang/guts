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

/*!@file fe/c/pp.h
 * @author uael
 */
#ifndef __FE_C_PP_H
# define __FE_C_PP_H

#include <ev/obs.h>
#include <lex/lexer.h>

#include "macro.h"

typedef struct c_pp c_pp_t;

OBSERVER(c_pp, lexer_t, lexer_ev_t,
  c_macros_t macros;
);

__api__ c_pp_t *
c_pp_new(void);

__api__ void
c_pp_dtor(c_pp_t *self);

__api__ bool_t
c_pp_update(c_pp_t *self, lexer_t *lexer, lexer_ev_t code, void *arg);

#endif /* !__FE_C_PP_H */
