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

/*!@file il/ty.h
 * @author uael
 */
#ifndef __IL_TY_H
# define __IL_TY_H

#include <uds/vec.h>

#include "ty/const.h"
#include "ty/kind.h"

typedef struct il_ty il_ty_t;
typedef struct il_ty_void il_ty_void_t;
typedef struct il_ty_scalar il_ty_float_t;
typedef struct il_ty_scalar il_ty_int_t;
typedef struct il_ty_fn il_ty_fn_t;
typedef struct il_ty_struct il_ty_struct_t;
typedef struct il_ty_wrap il_ty_array_t;
typedef struct il_ty_wrap il_ty_pointer_t;
typedef struct il_ty_wrap il_ty_vector_t;
typedef struct il_ty_void il_ty_label_t;
typedef struct il_ty_void il_ty_metadata_t;
typedef struct il_ty_void il_ty_token_t;

struct il_ty {
  il_ty_kind_t kind : 1;
};

struct il_ty_void {
  il_ty_kind_t kind : 1;
};

struct il_ty_scalar {
  il_ty_kind_t kind : 1;
  u8_t bits;
};

struct il_ty_fn {
  il_ty_kind_t kind : 1;
  il_ty_t *ret, **params;
  u32_t param_count;
  bool_t is_vararg;
};

struct il_ty_struct {
  il_ty_kind_t kind : 1;
  il_ty_t **elements;
  u32_t element_couny;
  bool_t is_packed;
};

struct il_ty_wrap {
  il_ty_kind_t kind : 1;
  il_ty_t *element;
  u32_t len;
};

#endif /* !__IL_TY_H */
