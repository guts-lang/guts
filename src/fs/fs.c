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

#include "fs.h"

FORCEINLINE void
fs_absolute(char_t __const *path, char_t *out)
{
#ifdef OS_WIN
  if (_fullpath(out, path, FS_PATH_MAX) == nil) {
    THROW(ex_errno(ERRLVL_ERROR, errno,
      "Unable to get absolute path from '%s'", path));
  }
#else
  char_t *ptr;

  if ((ptr = realpath(path, out)) == nil) {
    THROW(ex_errno(
      ERRLVL_ERROR, errno, "Unable to get absolute path from '%s'", path
    ));
  }
  (void) ptr;
#endif
}

FORCEINLINE bool_t
fs_exists(char_t __const *path)
{
#ifdef OS_WIN
  if (_access(path, 0) == 0) {
#else
  if (access(path, F_OK) == 0) {
#endif
    return true;
  }
  return false;
}

FORCEINLINE u16_t
fs_cwd(char_t *path, u16_t n)
{
  char *ret;

#ifdef OS_WIN
  if ((ret = _getcwd(path, n)) == nil)
#else
  if ((ret = getcwd(path, n)) == nil)
#endif
    return 0;
  return (i16_t) strlen(ret);
}

FORCEINLINE void
fs_mkdir(char_t __const *path)
{
#ifdef OS_WIN
  if (_mkdir(path) < 0) {
#else
  if (mkdir(path, S_IRWXU | S_IRWXG | S_IRWXO) < 0) {
#endif
    char_t *t, full[FS_PATH_MAX], temp[FS_PATH_MAX] = { 0 };
    char_t __const *p, *e;

    fs_absolute(path, full);
    p = full;
    t = temp;
    e = temp + FS_PATH_MAX - 1;
    for (; t < e && *p; t++) {
      *t = *p;
      if (*p == '/') {
        if (!fs_exists(temp)) {
#ifdef OS_WIN
          if (_mkdir(temp) < 0) {
#else
          if (mkdir(temp, S_IRWXU | S_IRWXG | S_IRWXO) < 0) {
#endif
            THROW(ex_errno(
              ERRLVL_ERROR, errno, "Unable to create directory '%s'", path
            ));
          }
        }
        while (*p == '/') p++;
      } else p++;
    }
#ifdef OS_WIN
    if (_mkdir(path) < 0) {
#else
    if (mkdir(path, S_IRWXU | S_IRWXG | S_IRWXO) < 0) {
#endif
      THROW(ex_errno(
        ERRLVL_ERROR, errno, "Unable to create directory '%s'", path
      ));
    }
  }
}

FORCEINLINE bool_t
fs_rm(char_t __const *path)
{
#ifdef OS_WIN
  if (UNLIKELY(_unlink(path) != 0)) {
#else
  if (UNLIKELY(unlink(path) != 0)) {
#endif
    return false;
  }
  return true;
}

FORCEINLINE void
fs_touch(char_t __const *path)
{
  i32_t fd;

  if (!fs_exists(path))
    fs_mkdir(path);
  fd_open(&fd, path, FS_OPEN_RO | FS_OPEN_CREAT);
  fd_close(&fd);
}
