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

static ostream_t __cout = {
  1, true, 0, 0, 0, { 0, 0, nil }, { 0, 0, nil }
};

ostream_t *cout = &__cout;

FORCEINLINE bool_t
ostream_open(ostream_t *self, char_t __const *filename)
{
  if (self->opened)
    return true;
  else {
    ex_t e;

    init(self, ostream_t);
    TRY {
      fd_open(&self->fd, filename, FS_OPEN_WO | FS_OPEN_APPEND | FS_OPEN_CREAT);
    } CATCH(e) {
      errs_push(&self->errs, e);
      return false;
    }
    self->cur = self->beg = fd_offset(&self->fd);
    return self->opened = true;
  }
}

FORCEINLINE void
ostream_close(ostream_t *self)
{
  if (self->opened && self->fd > 1) {
    ex_t e;

    ostream_flush(self);
    TRY {
      fd_close(&self->fd);
    } CATCH(e) {
      errs_push(&self->errs, e);
    }
    if (self->buf.buf) {
      mem_free(self->buf.buf);
      self->buf.buf = nil;
    }
    self->opened = false;
  }
}

FORCEINLINE usize_t
ostream_write(ostream_t *self, char_t __const *buf, usize_t len)
{
  char_t __const *beg;

  beg = buf;
  while (len) {
    usize_t b, cur;

    if (!self->buf.len) {
      while (len > FS_PAGE_SIZE) {
        usize_t n;

        n = fd_write(&self->fd, buf, FS_PAGE_SIZE);
        len -= n;
        buf += n;
      }
    }
    b = FS_PAGE_SIZE - self->buf.len;
    if (b > len) b = len;
    if (self->buf.cap == 0) {
      if ((self->buf.buf = mem_malloc(FS_PAGE_SIZE * sizeof(char_t))) == nil) {
        THROW(ex_errno(ERRLVL_FATAL, errno, nil));
      }
      self->buf.cap = FS_PAGE_SIZE;
    }
    cur = self->cur - self->beg;
    memcpy(self->buf.buf + cur, buf, (usize_t) b * sizeof(char_t));
    self->cur += b;
    cur += b;
    if (cur > self->buf.len) {
      self->buf.len = cur;
      self->end += b;
    }
    len -= b;
    buf += b;
    if (self->buf.len == FS_PAGE_SIZE) {
      ostream_flush(self);
    }
  }
  return buf - beg;
}

usize_t
ostream_writef(ostream_t *self, char_t __const *format, ...)
{
  va_list args;
  char_t buf[FS_PAGE_SIZE];
  i32_t n;

  va_start(args, format);
  n = vsprintf(buf, format, args);
  va_end(args);
  if (n < 0 || n > FS_PAGE_SIZE) {
    THROW(inval("Unable to writef, format is too long"));
  }
  return ostream_write(self, buf, (usize_t) n);
}

FORCEINLINE usize_t
ostream_puts(ostream_t *self, char_t __const *buf)
{
  return ostream_write(self, buf, strlen(buf));
}

FORCEINLINE usize_t
ostream_putc(ostream_t *self, char_t c)
{
  return ostream_write(self, &c, 1);
}

FORCEINLINE void
ostream_flush(ostream_t *self)
{
  if (self->buf.len) {
    ex_t e;

    TRY {
      self->beg += fd_write(&self->fd, self->buf.buf, self->buf.len);
      self->buf.len = 0;
    } CATCH(e) {
      errs_push(&self->errs, e);
    }
  }
}

FORCEINLINE bool_t
ostream_rewind(ostream_t *self, usize_t n)
{
  if (!self->opened)
    return false;
  else {
    ex_t e;

    if (n > self->cur) {
      THROW(inval("Unable to rewind by '%zu' when cursor is '%zu'",
        n, self->cur));
    }
    if (n >= self->buf.len) {
      ostream_flush(self);
      TRY {
        self->cur = self->beg = fd_seek(&self->fd, n, FS_SEEK_END);
      } CATCH(e) {
        errs_push(&self->errs, e);
        return false;
      }
    } else {
      self->cur -= n;
    }
    return true;
  }
}

FORCEINLINE bool_t
ostream_forward(ostream_t *self, usize_t n)
{
  if (!self->opened)
    return false;
  else {
    ex_t e;

    if (self->cur + n > self->end) {
      THROW(inval("Unable to rewind by '%zu' when remaining distance is '%zu'",
        n, self->end - self->cur));
    }
    if (n > self->buf.len) {
      ostream_flush(self);
      TRY {
        fd_seek(&self->fd, n, FS_SEEK_CUR);
      } CATCH(e) {
        errs_push(&self->errs, e);
        return false;
      }
    } else {
      self->cur += n;
    }
    return true;
  }
}

FORCEINLINE void
ostream_resume(ostream_t *self)
{
  if (self->cur - self->beg < self->buf.len) {
    self->cur = self->beg + self->buf.len;
  }
}

FORCEINLINE bool_t
ostream_seek(ostream_t *self, usize_t off)
{
  if (!self->opened)
    return 0;
  if (self->cur > off)
    return ostream_rewind(self, self->cur - off);
  return ostream_forward(self, off - self->cur);
}

FORCEINLINE usize_t
ostream_tell(ostream_t __const *self)
{
  return self->cur;
}
