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

/*!@file m/cmp.h
 * @author uael
 */
#ifndef __M_CMP_H
# define __M_CMP_H

#include <cor.h>

namespace m {

  template<
    typename T
  >
  inline i32
  cmp(const T a, const T b) {
    return static_cast<i32>((a > b) - (a < b));
  }

  template<
    typename T
  >
  inline i32
  cmp(const T *a, const T *b) {
    while (*a == *b++) {
      if (*a++ == 0) {
        return (0);
      }
    }
    return static_cast<i32>(
      *static_cast<const u8 *>(a) - *static_cast<const u8 *>(b - 1)
    );
  }
}

#endif /* !__M_CMP_H */
