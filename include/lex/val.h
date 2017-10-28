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

/*!@file lex/val.h
 * @author uael
 */
#ifndef __LEX_VAL_H
# define __LEX_VAL_H

#include <ds/dstr.h>
#include <fs/ostream.h>

enum val_kind {
  VAL_NULL,
  VAL_STR,
  VAL_IDENT,
  VAL_I8,
  VAL_I16,
  VAL_I32,
  VAL_I64,
  VAL_U8,
  VAL_U16,
  VAL_U32,
  VAL_U64,
  VAL_F32,
  VAL_F64
};

typedef enum val_kind val_kind_t;
typedef struct val val_t;

struct val {
  val_kind_t kind : 4;
  union {
    dstr_t str;
    dstr8_t ident;
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
  } val;
};

VEC32_DEFINE(vals, val_t, addrcmp)

__api__ void
val_init_str(val_t *self, char_t *str);

__api__ void
val_init_ident(val_t *self, char_t *ident);

__api__ void
val_init_i8(val_t *self, i8_t i);

__api__ void
val_init_i16(val_t *self, i16_t i);

__api__ void
val_init_i32(val_t *self, i32_t i);

__api__ void
val_init_i64(val_t *self, i64_t i);

__api__ void
val_init_u8(val_t *self, u8_t i);

__api__ void
val_init_u16(val_t *self, u16_t i);

__api__ void
val_init_u32(val_t *self, u32_t i);

__api__ void
val_init_u64(val_t *self, u64_t i);

__api__ void
val_init_f32(val_t *self, f32_t f);

__api__ void
val_init_f64(val_t *self, f64_t f);

__api__ void
val_parse_i64(val_t *self, char_t __const *str);

__api__ void
val_parse_f64(val_t *self, char_t __const *str);

__api__ void
val_dtor(val_t *self);

__api__ void
val_dump(val_t *self, ostream_t *stream);

#endif /* !__LEX_VAL_H */
