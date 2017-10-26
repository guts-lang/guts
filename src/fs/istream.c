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
  nil, 0, true, __cin_buf, 0, 0, 0, 0, 0, nil
};

istream_t *cin = &__cin;

__inline bool_t
istream_open(istream_t *self, char_t __const *filename)
{
  if (self->filename && self->fd >= 0)
    return true;
  else {
    ex_t *e;

    init(self, istream_t);
    TRY {
      fd_open(&self->fd, filename, FS_OPEN_RO);
    } CATCH(e) {
      self->ex = e;
      return false;
    }
    self->filename = filename;
    return true;
  }
}

__inline bool_t
istream_close(istream_t *self)
{
  if (!self->filename || self->fd <= 1)
    return true;
  else {
    ex_t *e;

    istream_flush(self);
    TRY {
      fd_close(&self->fd);
    } CATCH(e) {
      self->ex = e;
      return false;
    }
    if (self->buf) {
      mem_free(self->buf);
      self->buf = nil;
    }
    self->filename = nil;
    return true;
  }
}

static usize_t
istream_bufferize(istream_t *self, usize_t len)
{
  usize_t cur;

  cur = self->cur - self->beg;
  if (self->len - cur >= len) {
    return len;
  } else {
    usize_t b, r;

    b = 0;
    while (self->len - cur < len) {
      if (self->cap - self->len < FS_PAGE_SIZE) {
        usize_t cap;
        void *buf;

        cap = self->cap + FS_PAGE_SIZE;
        if ((buf = mem_realloc(self->buf, cap * sizeof(char_t))) == nil) {
          THROW(ex_errno(ERRLVL_FATAL, errno, nil));
        }
        self->buf = buf;
        self->cap = cap;
      }
      if ((r = fd_read(&self->fd, self->buf + self->len, FS_PAGE_SIZE)) == 0)
        break;
      b += r;
      cur += r;
      self->len += r;
      self->end += r;
    }
    if (b > len) return len;
    return b;
  }
}

usize_t
istream_read(istream_t *self, char_t *buf, usize_t len)
{
  if (!self->filename || self->fd < 0)
    return 0;
  else {
    char_t *beg;
    usize_t cur, r;

    beg = buf;
    while (len) {
      cur = self->cur - self->beg;
      if (self->len - cur <= 0) {
        if (len >= FS_PAGE_SIZE) {
          if ((r = fd_read(&self->fd, buf, FS_PAGE_SIZE)) == 0)
            break;
          if (r < FS_PAGE_SIZE)
            return buf - beg + r;
          len -= r;
          buf += r;
        } else {
          self->beg += self->len;
          if (self->cap < FS_PAGE_SIZE) {
            if ((self->buf = mem_malloc(FS_PAGE_SIZE * sizeof(char_t)))
              == nil) {
              THROW(ex_errno(ERRLVL_FATAL, errno, nil));
            }
            self->cap = FS_PAGE_SIZE;
          }
          if ((self->len = fd_read(&self->fd, self->buf, FS_PAGE_SIZE)) == 0)
            break;
          goto inject;
        }
      } else {
        inject:
        r = self->len - cur;
        if (r > len) {
          memcpy(buf, self->buf + cur, len * sizeof(char_t));
          self->cur += len;
          return len;
        }
        memcpy(buf, self->buf + cur, r * sizeof(char_t));
        self->cur += r;
        len -= r;
        buf += r;
      }
    }
    return buf - beg;
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
  (void) self;
  (void) fmt;
  (void) ap;
  return 0;
}

FORCEINLINE char_t
istream_getc(istream_t *self)
{
  char_t c;

  if (istream_read(self, &c, 1) == 1)
    return c;
  return '\0';
}

FORCEINLINE usize_t
istream_get(istream_t *self, char_t *buf, usize_t len)
{
  if ((len = istream_bufferize(self, len)) > 0) {
    memcpy(buf, self->buf + self->cur - self->beg, len);
  }
  return len;
}

FORCEINLINE char_t
istream_peek(istream_t *self, usize_t n)
{
  if (n > istream_bufferize(self, n + 1))
    return '\0';
  return self->buf[self->cur - self->beg + n];
}

FORCEINLINE void
istream_flush(istream_t *self)
{
  (void) self;
}

FORCEINLINE bool_t
istream_rewind(istream_t *self, usize_t n)
{
  (void) self;
  (void) n;
  return false;
}

FORCEINLINE bool_t
istream_forward(istream_t *self, usize_t n)
{
  (void) self;
  (void) n;
  return false;
}

FORCEINLINE void
istream_resume(istream_t *self)
{
  if (self->cur - self->beg < self->len) {
    self->cur = self->beg + self->len;
  }
}

FORCEINLINE bool_t
istream_seek(istream_t *self, usize_t off)
{
  if (!self->opened)
    return 0;
  if (self->cur > off)
    return istream_rewind(self, self->cur - off);
  return istream_forward(self, off - self->cur);
}

FORCEINLINE usize_t
istream_tell(istream_t __const *self)
{
  return self->cur;
}
