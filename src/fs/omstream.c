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

#include "fs/omstream.h"

FORCEINLINE bool_t
omstream_open(omstream_t *self)
{
  init(self, omstream_t);
  return true;
}

FORCEINLINE void
omstream_close(omstream_t *self)
{
  if (self->buf) {
    mem_free(self->buf);
  }
  init(self, omstream_t);
}

SEQ_IMPL_realloc(
  static FORCEINLINE, omstream, char_t, size, cap, cur, buf, mem_realloc,
  mem_free, i8cmp
)

SEQ_IMPL_grow(
  static FORCEINLINE, omstream, char_t, size, cap, cur, buf, mem_realloc,
  mem_free, i8cmp
)

FORCEINLINE usize_t
omstream_write(omstream_t *self, char_t __const *str, usize_t len)
{
  omstream_grow(self, len);
  memcpy(self->buf + self->cur, str, len);
  self->cur += len;
  if (self->cur > self->len) {
    self->len = self->cur;
  }
  return len;
}

usize_t
omstream_writef(omstream_t *self, char_t __const *fmt, ...)
{
  va_list ap;
  usize_t n;

  va_start(ap, fmt);
  n = omstream_vwritef(self, fmt, ap);
  va_end(ap);
  return n;
}

FORCEINLINE usize_t
omstream_vwritef(omstream_t *self, char_t __const *fmt, va_list ap)
{
  char_t buf[FS_PAGE_SIZE];
  i32_t n;

  n = vsprintf(buf, fmt, ap);
  if (n < 0 || n > FS_PAGE_SIZE) {
    THROW(inval("Unable to writef, format is too long"));
  }
  return omstream_write(self, buf, (usize_t) n);
}

FORCEINLINE usize_t
omstream_puts(omstream_t *self, char_t __const *str)
{
  return omstream_write(self, str, strlen(str));
}

FORCEINLINE usize_t
omstream_putc(omstream_t *self, char_t c)
{
  return omstream_write(self, &c, 1);
}

FORCEINLINE void
omstream_flush(omstream_t *self)
{
  (void) self;
}

FORCEINLINE bool_t
omstream_rewind(omstream_t *self, usize_t n)
{
  if (n > self->cur) {
    THROW(inval("Unable to rewind by '%zu' when cursor is '%zu'",
      n, self->cur));
  }
  self->cur -= n;
  return true;
}

FORCEINLINE bool_t
omstream_forward(omstream_t *self, usize_t n)
{
  if (self->cur + n > self->len) {
    THROW(inval("Unable to rewind by '%zu' when remaining distance is '%zu'",
      n, self->len - self->cur));
  }
  self->cur += n;
  return true;
}

FORCEINLINE void
omstream_resume(omstream_t *self)
{
  self->cur = self->len;
}

FORCEINLINE bool_t
omstream_seek(omstream_t *self, usize_t off)
{
  if (self->cur > off)
    return omstream_rewind(self, self->cur - off);
  return omstream_forward(self, off - self->cur);
}

FORCEINLINE usize_t
omstream_tell(omstream_t __const *self)
{
  return self->cur;
}
