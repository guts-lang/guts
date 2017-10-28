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

#include "fs/ostream.h"

static char_t __cout_buf[FS_PAGE_SIZE];
static ostream_t __cout = {
  OSTREAM_FILE, { { nil, 1, __cout_buf, 0, 0, 0, 0, nil } }
};

ostream_t *cout = &__cout;

FORCEINLINE bool_t
ostream_open(ostream_t *self, char_t __const *filename)
{
  self->kind = OSTREAM_FILE;
  return ofstream_open(&self->u.file, filename);
}

FORCEINLINE bool_t
ostream_memopen(ostream_t *self)
{
  self->kind = OSTREAM_MEM;
  return omstream_open(&self->u.mem);
}

FORCEINLINE void
ostream_close(ostream_t *self)
{
  switch (self->kind) {
    case OSTREAM_FILE:
      ofstream_close(&self->u.file);
      break;
    case OSTREAM_MEM:
      omstream_close(&self->u.mem);
      break;
    default:
      THROW(inval_enum(STRINGIFY(istream_kind_t), self->kind));
  }
}

FORCEINLINE usize_t
ostream_write(ostream_t *self, char_t __const *str, usize_t len)
{
  switch (self->kind) {
    case OSTREAM_FILE:
      return ofstream_write(&self->u.file, str, len);
    case OSTREAM_MEM:
      return omstream_write(&self->u.mem, str, len);
    default:
      THROW(inval_enum(STRINGIFY(istream_kind_t), self->kind));
  }
}

usize_t
ostream_writef(ostream_t *self, char_t __const *fmt, ...)
{
  va_list ap;
  usize_t n;

  va_start(ap, fmt);
  n = ostream_vwritef(self, fmt, ap);
  va_end(ap);
  return n;
}

FORCEINLINE usize_t
ostream_vwritef(ostream_t *self, char_t __const *fmt, va_list ap)
{
  switch (self->kind) {
    case OSTREAM_FILE:
      return ofstream_vwritef(&self->u.file, fmt, ap);
    case OSTREAM_MEM:
      return omstream_vwritef(&self->u.mem, fmt, ap);
    default:
      THROW(inval_enum(STRINGIFY(istream_kind_t), self->kind));
  }
}

FORCEINLINE usize_t
ostream_puts(ostream_t *self, char_t __const *str)
{
  switch (self->kind) {
    case OSTREAM_FILE:
      return ofstream_puts(&self->u.file, str);
    case OSTREAM_MEM:
      return omstream_puts(&self->u.mem, str);
    default:
      THROW(inval_enum(STRINGIFY(istream_kind_t), self->kind));
  }
}

FORCEINLINE usize_t
ostream_putc(ostream_t *self, char_t c)
{
  switch (self->kind) {
    case OSTREAM_FILE:
      return ofstream_putc(&self->u.file, c);
    case OSTREAM_MEM:
      return omstream_putc(&self->u.mem, c);
    default:
      THROW(inval_enum(STRINGIFY(istream_kind_t), self->kind));
  }
}

FORCEINLINE void
ostream_flush(ostream_t *self)
{
  switch (self->kind) {
    case OSTREAM_FILE:
      ofstream_flush(&self->u.file);
      break;
    case OSTREAM_MEM:
      omstream_flush(&self->u.mem);
      break;
    default:
      THROW(inval_enum(STRINGIFY(istream_kind_t), self->kind));
  }
}

FORCEINLINE bool_t
ostream_rewind(ostream_t *self, usize_t n)
{
  switch (self->kind) {
    case OSTREAM_FILE:
      return ofstream_rewind(&self->u.file, n);
    case OSTREAM_MEM:
      return omstream_rewind(&self->u.mem, n);
    default:
      THROW(inval_enum(STRINGIFY(istream_kind_t), self->kind));
  }
}

FORCEINLINE bool_t
ostream_forward(ostream_t *self, usize_t n)
{
  switch (self->kind) {
    case OSTREAM_FILE:
      return ofstream_forward(&self->u.file, n);
    case OSTREAM_MEM:
      return omstream_forward(&self->u.mem, n);
    default:
      THROW(inval_enum(STRINGIFY(istream_kind_t), self->kind));
  }
}

FORCEINLINE void
ostream_resume(ostream_t *self)
{
  switch (self->kind) {
    case OSTREAM_FILE:
      ofstream_resume(&self->u.file);
      break;
    case OSTREAM_MEM:
      omstream_resume(&self->u.mem);
      break;
    default:
      THROW(inval_enum(STRINGIFY(istream_kind_t), self->kind));
  }
}

FORCEINLINE bool_t
ostream_seek(ostream_t *self, usize_t off)
{
  switch (self->kind) {
    case OSTREAM_FILE:
      return ofstream_seek(&self->u.file, off);
    case OSTREAM_MEM:
      return omstream_seek(&self->u.mem, off);
    default:
      THROW(inval_enum(STRINGIFY(istream_kind_t), self->kind));
  }
}

FORCEINLINE usize_t
ostream_tell(ostream_t __const *self)
{
  switch (self->kind) {
    case OSTREAM_FILE:
      return ofstream_tell(&self->u.file);
    case OSTREAM_MEM:
      return omstream_tell(&self->u.mem);
    default:
      THROW(inval_enum(STRINGIFY(istream_kind_t), self->kind));
  }
}
