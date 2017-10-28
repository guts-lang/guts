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

/*!@file lex/tokval.h
 * @author uael
 */
#ifndef __LEX_TOKVAL_H
# define __LEX_TOKVAL_H

#include <ds/dstr.h>
#include <fs/ostream.h>

enum tokval_kind {
  TOKVAL_STR,
  TOKVAL_IDENT,
  TOKVAL_I8,
  TOKVAL_I16,
  TOKVAL_I32,
  TOKVAL_I64,
  TOKVAL_U8,
  TOKVAL_U16,
  TOKVAL_U32,
  TOKVAL_U64,
  TOKVAL_F32,
  TOKVAL_F64
};

typedef enum tokval_kind tokval_kind_t;
typedef struct tokval tokval_t;

struct tokval {
  tokval_kind_t kind: 4;
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

VEC32_DEFINE(tokvals, tokval_t, addrcmp)

__api__ void
tokval_init_str(tokval_t *self, char_t *str);

__api__ void
tokval_init_ident(tokval_t *self, char_t *ident);

__api__ void
tokval_init_i8(tokval_t *self, i8_t i);

__api__ void
tokval_init_i16(tokval_t *self, i16_t i);

__api__ void
tokval_init_i32(tokval_t *self, i32_t i);

__api__ void
tokval_init_i64(tokval_t *self, i64_t i);

__api__ void
tokval_init_u8(tokval_t *self, u8_t i);

__api__ void
tokval_init_u16(tokval_t *self, u16_t i);

__api__ void
tokval_init_u32(tokval_t *self, u32_t i);

__api__ void
tokval_init_u64(tokval_t *self, u64_t i);

__api__ void
tokval_init_f32(tokval_t *self, f32_t f);

__api__ void
tokval_init_f64(tokval_t *self, f64_t f);

__api__ void
tokval_parse_i64(tokval_t *self, char_t __const *str);

__api__ void
tokval_parse_f64(tokval_t *self, char_t __const *str);

__api__ void
tokval_dtor(tokval_t *self);

__api__ void
tokval_dump(tokval_t *self, ostream_t *stream);

#endif /* !__LEX_TOKVAL_H */
