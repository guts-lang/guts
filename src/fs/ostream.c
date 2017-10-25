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
  nil, 1, true, __cout_buf, 0, 0, 0, 0, nil
};

ostream_t *cout = &__cout;

__inline bool_t
ostream_open(ostream_t *self, char_t __const *filename)
{
  if (self->opened)
    return true;
  else {
    ex_t *e;

    init(self, ostream_t);
    TRY {
      fd_open(&self->fd, filename, FS_OPEN_WO | FS_OPEN_APPEND | FS_OPEN_CREAT);
    } CATCH(e) {
      self->ex = e;
      return false;
    }
    self->beg = self->cur = self->end = fd_offset(&self->fd);
    self->filename = filename;
    return self->opened = true;
  }
}

__inline void
ostream_close(ostream_t *self)
{
  if (self->opened && self->fd > 1) {
    ex_t *e;

    ostream_flush(self);
    TRY {
      fd_close(&self->fd);
    } CATCH(e) {
      self->ex = e;
    }
    if (self->buf) {
      mem_free(self->buf);
      self->buf = nil;
    }
    self->opened = false;
  }
}

usize_t
ostream_write(ostream_t *self, char_t __const *buf, usize_t len)
{
  char_t __const *beg;

  beg = buf;
  while (len) {
    usize_t b, cur;

    if (!self->len) {
      while (len > FS_PAGE_SIZE) {
        usize_t n;

        n = fd_write(&self->fd, buf, FS_PAGE_SIZE);
        len -= n;
        buf += n;
      }
    }
    b = FS_PAGE_SIZE - self->len;
    if (b > len) b = len;
    if (!self->buf) {
      if ((self->buf = mem_malloc(FS_PAGE_SIZE * sizeof(char_t))) == nil) {
        THROW(ex_errno(ERRLVL_FATAL, errno, nil));
      }
    }
    cur = self->cur - self->beg;
    memcpy(self->buf + cur, buf, (usize_t) b * sizeof(char_t));
    self->cur += b;
    cur += b;
    if (cur > self->len) {
      self->len = cur;
      self->end += b;
    }
    len -= b;
    buf += b;
    if (self->len == FS_PAGE_SIZE) {
      ostream_flush(self);
    }
  }
  return buf - beg;
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
  char_t buf[FS_PAGE_SIZE];
  i32_t n;

  n = vsprintf(buf, fmt, ap);
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

__inline void
ostream_flush(ostream_t *self)
{
  if (self->len) {
    ex_t *e;

    TRY {
      self->beg += fd_write(&self->fd, self->buf, self->len);
      self->len = 0;
    } CATCH(e) {
      self->ex = e;
    }
  }
}

__inline bool_t
ostream_rewind(ostream_t *self, usize_t n)
{
  if (!self->opened)
    return false;
  else {
    ex_t *e;

    if (n > self->cur) {
      THROW(inval("Unable to rewind by '%zu' when cursor is '%zu'",
        n, self->cur));
    }
    if (n >= self->len) {
      ostream_flush(self);
      TRY {
        self->cur = self->beg = fd_seek(&self->fd, n, FS_SEEK_END);
      } CATCH(e) {
        self->ex = e;
        return false;
      }
    } else {
      self->cur -= n;
    }
    return true;
  }
}

__inline bool_t
ostream_forward(ostream_t *self, usize_t n)
{
  if (!self->opened)
    return false;
  else {
    ex_t *e;

    if (self->cur + n > self->end) {
      THROW(inval("Unable to rewind by '%zu' when remaining distance is '%zu'",
        n, self->end - self->cur));
    }
    if (n > self->len) {
      ostream_flush(self);
      TRY {
        fd_seek(&self->fd, n, FS_SEEK_CUR);
      } CATCH(e) {
        self->ex = e;
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
  if (self->cur - self->beg < self->len) {
    self->cur = self->beg + self->len;
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
