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

#include "fs/path.h"

SEQ_IMPL_realloc(static FORCEINLINE, fs_path, char_t, 16, cap, len, buf,
  realloc, free, i8cmp)

SEQ_IMPL_ensure_strict(static FORCEINLINE, fs_path, char_t, 16, cap, len, buf,
  realloc, free, i8cmp)

SEQ_IMPL_grow_strict(static FORCEINLINE, fs_path, char_t, 16, cap, len, buf,
  realloc, free, i8cmp)

SEQ_IMPL_append_nt(static FORCEINLINE, fs_path, char_t, 16, cap, len, buf,
  realloc, free, i8cmp)

SEQ_IMPL_nappend(static FORCEINLINE, fs_path, char_t, 16, cap, len, buf,
  realloc, free, i8cmp)

SEQ_IMPL_dtor(FORCEINLINE, fs_path, char_t, 16, cap, len, buf, realloc, free,
  i8cmp)

SEQ_IMPL_cpy_nt(FORCEINLINE, fs_path, char_t, 16, cap, len, buf, realloc, free,
  i8cmp)

FORCEINLINE void
fs_path_ctor(fs_path_t *__restrict self)
{
  self->buf = nil;
  self->cap = 0;
  self->len = 0;
}

FORCEINLINE bool_t
fs_path(fs_path_t *self, char_t __const *path)
{
  fs_path_ctor(self);
  return fs_path_append(self, (char_t *) path);
}

FORCEINLINE bool_t
fs_pathn(fs_path_t *self, char_t __const *path, u16_t n)
{
  fs_path_ctor(self);
  return fs_path_nappend(self, (char_t *) path, (__const u16_t) n);
}

FORCEINLINE bool_t
fs_path_cwd(fs_path_t *self)
{
  char_t path[FS_PATH_MAX];
  u16_t n;

  if ((n = fs_cwd(path, FS_PATH_MAX)) > 0) {
    return fs_pathn(self, path, n);
  }
  return false;
}

FORCEINLINE bool_t
fs_path_is_abs(fs_path_t __const *self)
{
#ifdef OS_WIN
  return *self->buf == '~' || (self->len > 2
    && isalpha(self->buf[0])
    && self->buf[1] == ':'
    && (self->buf[2] == '/' || self->buf[2] == '\\'));
#else
  return *self->buf == '/' || *self->buf == '\\' || *self->buf == '~';
#endif
}

FORCEINLINE bool_t
fs_path_is_rel(fs_path_t __const *self)
{
  return !fs_path_is_abs(self);
}

FORCEINLINE bool_t
fs_path_absolute(fs_path_t *self, fs_path_t *out)
{
  char_t path[FS_PATH_MAX], *ptr;

#ifdef OS_WIN
  if (_fullpath(self->buf, path, FS_PATH_MAX) == nil) {
    THROW(ex_errno(ERRLVL_ERROR, errno,
      "Unable to get absolute path from '%s'", self->buf));
  }
  ptr = path;
#else
  if ((ptr = realpath(self->buf, path)) == nil) {
    THROW(ex_errno(ERRLVL_ERROR, errno,
      "Unable to get absolute path from '%s'", self->buf));
  }
#endif
  if (out != nil) {
    return fs_path(out, ptr);
  }
  self->len = 0;
  return fs_path_append(self, ptr);
}

FORCEINLINE bool_t
fs_path_join(fs_path_t *self, fs_path_t *other)
{
  fs_path_ensure(self, (u16_t) (other->len + 1));
  self->buf[self->len++] = DS;
  return fs_path_nappend(self, other->buf, other->len);
}
