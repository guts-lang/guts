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

#include "fs/imstream.h"

FORCEINLINE bool_t
imstream_open(imstream_t *self, char_t __const *str)
{
  init(self, imstream_t);
  self->buf = str;
  self->len = strlen(str);
  return true;
}

FORCEINLINE bool_t
imstream_nopen(imstream_t *self, char_t __const *str, usize_t n)
{
  init(self, imstream_t);
  self->buf = str;
  self->len = n;
  return true;
}

FORCEINLINE bool_t
imstream_close(imstream_t *self)
{
  init(self, imstream_t);
  return true;
}

FORCEINLINE usize_t
imstream_read(imstream_t *self, char_t *buf, usize_t len)
{
  if (len > self->len) {
    len = self->len;
  }
  if (LIKELY(len)) {
    memcpy(buf, self->buf + self->cur, len * sizeof(char_t));
    self->cur += len;
  }
  return len;
}

FORCEINLINE usize_t
imstream_readf(imstream_t *self, char_t *fmt, ...)
{
  (void) self;
  (void) fmt;
  THROW(nimpl(__pretty_func__));
}

FORCEINLINE usize_t
imstream_vreadf(imstream_t *self, char_t *fmt, va_list ap)
{
  (void) self;
  (void) fmt;
  (void) ap;
  THROW(nimpl(__pretty_func__));
}

FORCEINLINE char_t
imstream_getc(imstream_t *self)
{
  if (self->len - self->cur)
    return self->buf[self->cur++];
  return '\0';
}

FORCEINLINE usize_t
imstream_get(imstream_t *self, char_t *buf, usize_t len)
{
  if (len > self->len) {
    len = self->len;
  }
  if (LIKELY(len)) {
    memcpy(buf, self->buf + self->cur, len * sizeof(char_t));
  }
  return len;
}

FORCEINLINE char_t
imstream_peek(imstream_t *self, usize_t n)
{
  usize_t cur;

  cur = self->cur + n;
  if (cur <= self->len)
    return self->buf[cur];
  return '\0';
}

FORCEINLINE void
imstream_flush(imstream_t *self)
{
  (void) self;
}

FORCEINLINE bool_t
imstream_rewind(imstream_t *self, usize_t n)
{
  if (n > self->cur) {
    THROW(inval("Unable to rewind by '%zu' when cursor is '%zu'",
      n, self->cur));
  }
  self->cur -= n;
  return true;
}

FORCEINLINE bool_t
imstream_forward(imstream_t *self, usize_t n)
{
  usize_t cur;

  cur = self->cur + n;
  if (cur > self->len) {
    THROW(inval("Unable to rewind by '%zu' when remaining distance is '%zu'",
      n, self->len - self->cur));
  }
  self->cur = cur;
  return true;
}

FORCEINLINE void
imstream_resume(imstream_t *self)
{
  (void) self;
}

FORCEINLINE bool_t
imstream_seek(imstream_t *self, usize_t off)
{
  if (self->cur > off)
    return imstream_rewind(self, self->cur - off);
  return imstream_forward(self, off - self->cur);
}

FORCEINLINE usize_t
imstream_tell(imstream_t __const *self)
{
  return self->cur;
}
