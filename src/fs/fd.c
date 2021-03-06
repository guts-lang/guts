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

#include "fs/fd.h"

#if defined FS_FD_MODEL_WIN_UCRT
# if !defined O_RDONLY
#   define O_RDONLY _O_RDONLY
#   define O_WRONLY _O_WRONLY
#   define O_RDWR _O_RDWR
#   define O_APPEND _O_APPEND
#   define O_CREAT _O_CREAT
#   define O_TRUNC _O_TRUNC
#   define O_EXCL _O_EXCL
#   define O_TEXT _O_TEXT
#   define O_BINARY _O_BINARY
#   define O_RAW _O_BINARY
#   define O_TEMPORARY _O_TEMPORARY
#   define O_NOINHERIT _O_NOINHERIT
#   define O_SEQUENTIAL _O_SEQUENTIAL
#   define O_RANDOM _O_RANDOM
# endif
# if !defined S_IFMT
#   define S_IFMT _S_IFMT
#   define S_IFDIR _S_IFDIR
#   define S_IFCHR _S_IFCHR
#   define S_IFREG _S_IFREG
#   define S_IREAD _S_IREAD
#   define S_IWRITE _S_IWRITE
#   define S_IEXEC _S_IEXEC
# endif
#endif

FORCEINLINE void
fd_open(fd_t *__restrict self, char_t __const *filename, u32_t flags)
{
#if defined FS_FD_MODEL_UNIX
  u32_t modes, uflags;

  uflags = flags;
  flags = 0;
  if (uflags & FS_OPEN_RO) flags |= O_RDONLY;
  else if (uflags & FS_OPEN_WO) flags |= O_WRONLY;
  else if (uflags & FS_OPEN_RW) flags |= O_RDWR;
  if (uflags & FS_OPEN_CREAT) flags |= O_CREAT;
  if (uflags & FS_OPEN_APPEND) flags |= O_APPEND;
  if (uflags & FS_OPEN_TRUNC) flags |= O_TRUNC;
  modes = 0;
#   ifdef O_DIRECT
  if (uflags & FS_OPEN_DIRECT) flags |= O_DIRECT;
#   else
  if (uflags & FS_OPEN_DIRECT) flags |= 0x80000;
#   endif
  flags |= O_NONBLOCK;
  if (uflags & FS_OPEN_CREAT) {
    modes = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
  }
  *self = open(filename, flags, modes);
  if (*self == FS_FD_DFT) {
    THROW(ex_errno(ERRLVL_ERROR, errno, "Unable to open file '%s'", filename));
  }
#elif defined FS_FD_MODEL_WIN_UCRT
  u32_t modes, uflags;

  uflags = flags;
  flags = 0;
  if (uflags & FS_OPEN_RO) flags |= O_RDONLY;
  else if (uflags & FS_OPEN_WO) flags |= O_WRONLY;
  else if (uflags & FS_OPEN_RW) flags |= O_RDWR;
  if (uflags & FS_OPEN_CREAT) flags |= O_CREAT;
  if (uflags & FS_OPEN_APPEND) flags |= O_APPEND;
  if (uflags & FS_OPEN_TRUNC) flags |= O_TRUNC;
  modes = 0;
  if (uflags & FS_OPEN_CREAT) {
    modes = S_IREAD | S_IWRITE;
  }
  *self = _open(filename, flags, modes);
  if (*self == FS_FD_DFT) {
    THROW(ex_errno(ERRLVL_ERROR, errno, "Unable to open file '%s'", filename));
  }
#endif
}

FORCEINLINE void
fd_close(fd_t __const *__restrict self)
{
#if defined FS_FD_MODEL_UNIX
  if (close(*self) != 0)
    THROW(ex_errno(ERRLVL_ERROR, errno, "Unable to close fd '%d'", *self));
#elif defined FS_FD_MODEL_WIN_UCRT
  if (_close(*self) != 0)
    THROW(ex_errno(ERRLVL_ERROR, errno, "Unable to close fd '%d'", *self));
#endif
}

FORCEINLINE usize_t
fd_read(fd_t __const *__restrict self, char_t *buf, usize_t len)
{
  isize_t r;

#if defined FS_FD_MODEL_UNIX
  if ((r = read(*self, buf, len)) == 0) {
    return false;
  }
#elif defined FS_FD_MODEL_WIN_UCRT
  if ((r = _read(*self, buf, (unsigned) (len - 1))) == 0) {
    return false;
  }
#endif
  if (r < 0) {
    THROW(ex_errno(ERRLVL_ERROR, errno, "Unable to read fd '%d'", *self));
  }
  return (usize_t) r;
}

FORCEINLINE usize_t
fd_write(fd_t __const *__restrict self, char_t __const *buf, usize_t len)
{
  isize_t r;

#if defined FS_FD_MODEL_UNIX
  if ((r = write(*self, buf, len)) == 0) {
    return false;
  }
#elif defined FS_FD_MODEL_WIN_UCRT
  if ((r = _write(*self, buf, (unsigned) len)) == 0) {
    return false;
  }
#endif
  if (r < 0) {
    THROW(ex_errno(ERRLVL_ERROR, errno, "Unable to write fd '%d'", *self));
  }
  return (usize_t) r;
}

FORCEINLINE usize_t
fd_seek(fd_t __const *__restrict self, isize_t off, fs_seek_mod_t whence)
{
#if defined FS_FD_MODEL_NONE
  return 0;
#elif defined FS_FD_MODEL_UNIX
  isize_t r;

  if ((r = lseek(*self, (long) off, whence)) < 0) {
    THROW(ex_errno(ERRLVL_ERROR, errno, "Unable to seek fd '%d'", *self));
  }
  return (usize_t) r;
#elif defined FS_FD_MODEL_WIN_UCRT
  isize_t r;

  if ((r = _lseek(*self, (long) off, whence)) < 0) {
    THROW(ex_errno(ERRLVL_ERROR, errno, "Unable to seek fd '%d'", *self));
  }
  return (usize_t) r;
#elif defined FS_FD_MODEL_WIN_NT
  return 0;
#endif
}

FORCEINLINE usize_t
fd_offset(fd_t __const *__restrict self)
{
  return fd_seek(self, 0, FS_SEEK_CUR);
}
