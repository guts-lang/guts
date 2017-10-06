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

/*!@file m/pow.h
 * @author uael
 */
#ifndef __M_POW_H
# define __M_POW_H

namespace m {

  template <
    typename T
  >
  static constexpr bool is_pow2(T n) {
    return (n != 0) && (n & (~n + 1)) == n;
  }

  namespace pow2 {

    template <
      typename T
    >
    static const T next(T n) {
      u32 j;
      u32 i;

      if (n == limits::max<T>()) {
        return n;
      }
      i = static_cast<u32>(n);
      i = static_cast<u32>(
        (void) ((j = i & 0xFFFF0000) || (j = i)),
        (void) ((i = j & 0xFF00FF00) || (i = j)),
        (void) ((j = i & 0xF0F0F0F0) || (j = i)),
        (void) ((i = j & 0xCCCCCCCC) || (i = j)),
        (void) ((j = i & 0xAAAAAAAA) || (j = i)),
        j << 1
      );
      return (i > limits::max<T>() || i < n)
        ? limits::max<T>() :
        static_cast<T>(i);
    }
  }
}

#endif /* !__M_POW_H */
