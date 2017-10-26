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

#include "fs/istream.h"

static char_t __cin_buf[FS_PAGE_SIZE];
static istream_t __cin = {
  ISTREAM_FILE, { { nil, 0, __cin_buf, 0, 0, 0, 0, 0, nil } }
};

istream_t *cin = &__cin;

FORCEINLINE bool_t
istream_open(istream_t *self, char_t __const *filename)
{
  self->kind = ISTREAM_FILE;
  return ifstream_open(&self->u.file, filename);
}

FORCEINLINE bool_t
istream_memopen(istream_t *self, char_t __const *str)
{
  self->kind = ISTREAM_MEM;
  return imstream_open(&self->u.mem, str);
}

FORCEINLINE bool_t
istream_nmemopen(istream_t *self, char_t __const *str, usize_t n)
{
  self->kind = ISTREAM_MEM;
  return imstream_nopen(&self->u.mem, str, n);
}

FORCEINLINE bool_t
istream_close(istream_t *self)
{
  switch (self->kind) {
    case ISTREAM_FILE:
      return ifstream_close(&self->u.file);
    case ISTREAM_MEM:
      return imstream_close(&self->u.mem);
    default:
      THROW(inval_enum(STRINGIFY(istream_kind_t), self->kind));
  }
}

FORCEINLINE usize_t
istream_read(istream_t *self, char_t *buf, usize_t len)
{
  switch (self->kind) {
    case ISTREAM_FILE:
      return ifstream_read(&self->u.file, buf, len);
    case ISTREAM_MEM:
      return imstream_read(&self->u.mem, buf, len);
    default:
      THROW(inval_enum(STRINGIFY(istream_kind_t), self->kind));
  }
}

usize_t
istream_readf(istream_t *self, char_t *fmt, ...)
{
  va_list ap;
  usize_t n;

  va_start(ap, fmt);
  n = istream_vreadf(self, fmt, ap);
  va_end(ap);
  return n;
}

FORCEINLINE usize_t
istream_vreadf(istream_t *self, char_t *fmt, va_list ap)
{
  switch (self->kind) {
    case ISTREAM_FILE:
      return ifstream_vreadf(&self->u.file, fmt, ap);
    case ISTREAM_MEM:
      return imstream_vreadf(&self->u.mem, fmt, ap);
    default:
      THROW(inval_enum(STRINGIFY(istream_kind_t), self->kind));
  }
}

FORCEINLINE char_t
istream_getc(istream_t *self)
{
  switch (self->kind) {
    case ISTREAM_FILE:
      return ifstream_getc(&self->u.file);
    case ISTREAM_MEM:
      return imstream_getc(&self->u.mem);
    default:
      THROW(inval_enum(STRINGIFY(istream_kind_t), self->kind));
  }
}

FORCEINLINE usize_t
istream_get(istream_t *self, char_t *buf, usize_t len)
{
  switch (self->kind) {
    case ISTREAM_FILE:
      return ifstream_get(&self->u.file, buf, len);
    case ISTREAM_MEM:
      return imstream_get(&self->u.mem, buf, len);
    default:
      THROW(inval_enum(STRINGIFY(istream_kind_t), self->kind));
  }
}

FORCEINLINE char_t
istream_peek(istream_t *self, usize_t n)
{
  switch (self->kind) {
    case ISTREAM_FILE:
      return ifstream_peek(&self->u.file, n);
    case ISTREAM_MEM:
      return imstream_peek(&self->u.mem, n);
    default:
      THROW(inval_enum(STRINGIFY(istream_kind_t), self->kind));
  }
}

FORCEINLINE void
istream_flush(istream_t *self)
{
  switch (self->kind) {
    case ISTREAM_FILE:
      return ifstream_flush(&self->u.file);
    case ISTREAM_MEM:
      return imstream_flush(&self->u.mem);
    default:
      THROW(inval_enum(STRINGIFY(istream_kind_t), self->kind));
  }
}

FORCEINLINE bool_t
istream_rewind(istream_t *self, usize_t n)
{
  switch (self->kind) {
    case ISTREAM_FILE:
      return ifstream_rewind(&self->u.file, n);
    case ISTREAM_MEM:
      return imstream_rewind(&self->u.mem, n);
    default:
      THROW(inval_enum(STRINGIFY(istream_kind_t), self->kind));
  }
}

FORCEINLINE bool_t
istream_forward(istream_t *self, usize_t n)
{
  switch (self->kind) {
    case ISTREAM_FILE:
      return ifstream_forward(&self->u.file, n);
    case ISTREAM_MEM:
      return imstream_forward(&self->u.mem, n);
    default:
      THROW(inval_enum(STRINGIFY(istream_kind_t), self->kind));
  }
}

FORCEINLINE void
istream_resume(istream_t *self)
{
  switch (self->kind) {
    case ISTREAM_FILE:
      ifstream_resume(&self->u.file);
      break;
    case ISTREAM_MEM:
      imstream_resume(&self->u.mem);
      break;
    default:
      THROW(inval_enum(STRINGIFY(istream_kind_t), self->kind));
  }
}

FORCEINLINE bool_t
istream_seek(istream_t *self, usize_t off)
{
  switch (self->kind) {
    case ISTREAM_FILE:
      return ifstream_seek(&self->u.file, off);
    case ISTREAM_MEM:
      return imstream_seek(&self->u.mem, off);
    default:
      THROW(inval_enum(STRINGIFY(istream_kind_t), self->kind));
  }
}

FORCEINLINE usize_t
istream_tell(istream_t __const *self)
{
  switch (self->kind) {
    case ISTREAM_FILE:
      return ifstream_tell(&self->u.file);
    case ISTREAM_MEM:
      return imstream_tell(&self->u.mem);
    default:
      THROW(inval_enum(STRINGIFY(istream_kind_t), self->kind));
  }
}
