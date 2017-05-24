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

#include <stdio.h>
#include <assert.h>

#include "xmalloc.h"

#include "attr.h"

#undef WITH_LTALLOC

#ifdef WITH_LTALLOC
#include "ltalloc.h"
#endif

static JL_NORETURN xnomem(void) {
  fputs("out of memory", stderr);
  abort();
}

void *xmalloc(size_t size) {
#ifdef WITH_LTALLOC
  return ltalloc(size);
#else
  void *ret = malloc(size);
  if (!ret) xnomem();
  return ret;
#endif
}

void *xrealloc(void *ptr, size_t size) {
#ifdef WITH_LTALLOC
  if (ptr) {
    size_t uSize = ltalloc_usable_size(ptr);
    if (size <= uSize)
      return ptr;
    void *newp = ltalloc(size);
    memcpy(newp, ptr, uSize);
    ltfree(ptr);
    return newp;
  }
  return ltalloc(size);
#else
  void *ret = realloc(ptr, size);
  if (!ret) xnomem();
  return ret;
#endif
}

void *xcalloc(size_t n, size_t esize) {
#ifdef WITH_LTALLOC
  size_t size = n * esize;
  if (esize == 0 || size / esize == n) {
    xnomem();
  }
  void *result = ltalloc(size);
  if (result && size <= (64*1024))//memory obtained directly from the system are already zero filled
    memset(result, 0, size);
  return result;
#else
  void *ret = calloc(n, esize);
  if (!ret) xnomem();
  return ret;
#endif
}

void xfree(void *ptr) {
#ifdef WITH_LTALLOC
  ltfree(ptr);
#else
  free(ptr);
#endif
}
