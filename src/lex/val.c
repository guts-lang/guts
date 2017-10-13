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

#include "lex/val.h"

FORCEINLINE ret_t
val_init_str(val_t *self, char_t *str)
{
  ret_t ret;

  init(self, val_t);
  self->kind = VAL_STR;
  dstr_ctor(&self->val.str);
  if ((ret = dstr_append(&self->val.str, str)) > 0) return ret;
  return ret;
}

FORCEINLINE ret_t
val_init_ident(val_t *self, char_t *ident)
{
  ret_t ret;

  init(self, val_t);
  self->kind = VAL_IDENT;
  dstr8_ctor(&self->val.ident);
  if ((ret = dstr8_append(&self->val.ident, ident)) > 0) return ret;
  return ret;
}

FORCEINLINE void
val_init_i8(val_t *self, i8_t i)
{
  init(self, val_t);
  self->kind = VAL_I8;
  self->val.i8 = i;
}

FORCEINLINE void
val_init_i16(val_t *self, i16_t i)
{
  init(self, val_t);
  self->kind = VAL_I16;
  self->val.i16 = i;
}

FORCEINLINE void
val_init_i32(val_t *self, i32_t i)
{
  init(self, val_t);
  self->kind = VAL_I32;
  self->val.i32 = i;
}

FORCEINLINE void
val_init_i64(val_t *self, i64_t i)
{
  init(self, val_t);
  self->kind = VAL_I64;
  self->val.i64 = i;
}

FORCEINLINE void
val_init_u8(val_t *self, u8_t i)
{
  init(self, val_t);
  self->kind = VAL_U8;
  self->val.u8 = i;
}

FORCEINLINE void
val_init_u16(val_t *self, u16_t i)
{
  init(self, val_t);
  self->kind = VAL_U16;
  self->val.u16 = i;
}

FORCEINLINE void
val_init_u32(val_t *self, u32_t i)
{
  init(self, val_t);
  self->kind = VAL_U32;
  self->val.u32 = i;
}

FORCEINLINE void
val_init_u64(val_t *self, u64_t i)
{
  init(self, val_t);
  self->kind = VAL_U64;
  self->val.u64 = i;
}

FORCEINLINE void
val_init_f32(val_t *self, f32_t f)
{
  init(self, val_t);
  self->kind = VAL_F32;
  self->val.f32 = f;
}

FORCEINLINE void
val_init_f64(val_t *self, f64_t f)
{
  init(self, val_t);
  self->kind = VAL_F64;
  self->val.f64 = f;
}

FORCEINLINE void
val_dtor(val_t *self)
{
  switch (self->kind) {
    case VAL_STR: dstr_dtor(&self->val.str);
      break;
    case VAL_IDENT: dstr8_dtor(&self->val.ident);
      break;
    default: break;
  }
}
