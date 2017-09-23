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

/*!@file il/ty/const.h
 * @author uael
 */
#ifndef __IL_TY_CONST_H
# define __IL_TY_CONST_H

#include <uty.h>

#include "const/kind.h"
#include "const/str.h"

typedef struct il_ty_const il_ty_const_t;

struct il_ty_const {
  il_ty_const_kind_t kind : 1;
  union {
    i8_t i8;
    i16_t i16;
    i32_t i32;
    i64_t i64;
    u8_t u8;
    u16_t u16;
    u32_t u32;
    u64_t u64;
    f32_t f32;
    f64_t f64;
    il_ty_cstr_t str;
  };
};

#endif /* !__IL_TY_CONST_H */
