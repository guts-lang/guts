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

/*!@file m/hash.h
 * @author uael
 */
#ifndef __M_HASH_H
# define __M_HASH_H

#include <cor.h>

namespace m {

  template<
    typename T
  >
  inline u32
  hash(T x) {
    return static_cast<u32>(x);
  }

  template<
    typename T
  >
  inline u32
  hash(T *x) {
    auto h = (u32) *x;
    if (h) for (++x; *x; ++x) h = (h << 5) - h + hash(*x);
    return h;
  }

  template<
  >
  inline u32
  hash<i64>(i64 x) {
    return static_cast<u32>(x >> 33 ^ x << 11);
  }

  template<
  >
  inline u32
  hash<u64>(u64 x) {
    return static_cast<u32>(x >> 33 ^ x << 11);
  }
}

#endif /* !__M_HASH_H */
