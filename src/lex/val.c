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

FORCEINLINE void
val_init_str(val_t *self, char_t *str)
{
  init(self, val_t);
  self->kind = VAL_STR;
  dstr_ctor(&self->val.str);
  dstr_append(&self->val.str, str);
}

FORCEINLINE void
val_init_ident(val_t *self, char_t *ident)
{
  init(self, val_t);
  self->kind = VAL_IDENT;
  dstr8_ctor(&self->val.ident);
  dstr8_append(&self->val.ident, ident);
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
val_parse_i64(val_t *self, char_t __const *str)
{
  i64_t i64;

  for (i64 = 0; *str; ++str)
    if (isdigit(*str))
      i64 = i64 * 10 + *str - '0';
  val_init_i64(self, i64);
}

FORCEINLINE void
val_parse_f64(val_t *self, char_t __const *str)
{
  f64_t f64, f;
  bool_t floating;

  for (floating = false, f64 = 0.0, f = 1; *str; ++str) {
    if (*str == '.') {
      floating = true;
      continue;
    }
    if (isdigit(*str)) {
      if (floating) f /= 10.0f;
      f64 = f64 * 10.0f + (f32_t) *str - '0';
    }
  }
  val_init_f64(self, f64 * f);
}

FORCEINLINE void
val_dtor(val_t *self)
{
  switch (self->kind) {
    case VAL_STR: dstr_dtor(&self->val.str, nil);
      break;
    case VAL_IDENT: dstr8_dtor(&self->val.ident, nil);
      break;
    default: break;
  }
}

FORCEINLINE void
val_dump(val_t *self, ostream_t *stream)
{
  switch (self->kind) {
    case VAL_NULL: ostream_puts(stream, "nil");
      break;
    case VAL_STR: ostream_puts(stream, self->val.str.buf);
      break;
    case VAL_IDENT: ostream_puts(stream, self->val.ident.buf);
      break;
    case VAL_I8: ostream_writef(stream, "%d", self->val.i8);
      break;
    case VAL_I16: ostream_writef(stream, "%d", self->val.i16);
      break;
    case VAL_I32: ostream_writef(stream, "%d", self->val.i32);
      break;
    case VAL_I64: ostream_writef(stream, "%li", self->val.i64);
      break;
    case VAL_U8: ostream_writef(stream, "%d", self->val.u8);
      break;
    case VAL_U16: ostream_writef(stream, "%d", self->val.u16);
      break;
    case VAL_U32: ostream_writef(stream, "%d", self->val.u32);
      break;
    case VAL_U64: ostream_writef(stream, "%lu", self->val.u64);
      break;
    case VAL_F32: ostream_writef(stream, "%f", self->val.f32);
      break;
    case VAL_F64: ostream_writef(stream, "%lf", self->val.f64);
      break;
  }
}
