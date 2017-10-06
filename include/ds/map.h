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

/*!@file ds/map.h
 * @author uael
 */
#ifndef __DS_MAP_H
# define __DS_MAP_H

#include <cstdlib>
#include <cstring>

#include <cor.h>
#include <m.h>

namespace ds {

  template<
    typename TKey,
    typename TValue,
    typename TSize = u32
  >
  struct Map {
    enum Flag : u8 {
      Deleted = 0x1,
      Empty = 0x2,
      Either = Deleted | Empty
    };

    TSize cap, len, occupieds, upper_bound;
    u8 *flags;
    TKey *keys;
    TValue *vals;

    inline Map() : cap(0), len(0), occupieds(0), upper_bound(0),
                   flags(nullptr), keys(nullptr), vals(nullptr) {}

    inline ~Map() {
      if (flags) {
        std::free(flags);
        std::free(keys);
        std::free(vals);
        cap = len = occupieds = upper_bound = 0;
      }
    }

    inline void
    clear() {
      if (flags) {
        std::memset(flags, Flag::Empty, cap);
        len = occupieds = 0;
      }
    }

    bool
    get(TKey key, TSize *out) {
      if (cap) {
        TSize i, last, mask, step;
        u32 k;

        step = 0;
        mask = cap - 1;
        k = m::hash(key);
        i = k & mask;
        last = i;
        while (flags[i] & Flag::Empty != Flag::Empty &&
          (flags[i] & Flag::Deleted == Flag::Deleted || !m::eq(keys[i], key))) {
          i = (i + (++step)) & mask;
          if (i == last) {
            return false;
          }
        }
        if (flags[i] & Flag::Either != 0) {
          return false;
        }
        *out = i;
        return true;
      }
      return false;
    }

    bool
    resize(TSize ensure) {
      u8 *new_buckets;
      u32 j;
      new_buckets = nullptr;
      j = 1;
      {
        ensure = m::pow2::next(ensure);
        if (ensure < 4) ensure = 32;
        if (len >= (u32) (ensure * 0.77 + 0.5)) { j = 0; }
        else {
          new_buckets = (u8 *) std::malloc(ensure);
          if (!new_buckets) return false;
          memset(new_buckets, 2, ensure);
          if (cap < ensure) {
            TKey *new_keys = (TKey *) std::realloc((void *) keys,
              ensure * sizeof(TKey));
            if (!new_keys) {
              std::free(new_buckets);
              return false;
            }
            keys = new_keys;
            TValue *new_vals = (TValue *) std::realloc((void *) vals,
              ensure * sizeof(TValue));
            if (!new_vals) {
              std::free(new_buckets);
              return false;
            }
            vals = new_vals;
          }
        }
      }
      if (j) {
        for (j = 0; j != cap; ++j) {
          if ((((flags)[j] & Flag::Either) != 0) == 0) {
            TKey key = keys[j];
            TValue val;
            u32 new_mask;
            new_mask = ensure - 1;
            val = vals[j];
            ((flags)[j] |= Flag::Deleted);
            while (1) {
              u32 k, i, step = 0;
              k = m::hash(key);
              i = k & new_mask;
              while ((((new_buckets)[i] & Flag::Empty) != Flag::Empty)) {
                i = (i + (++step)) & new_mask;
              }
              (((new_buckets)[i] & Flag::Empty) == Flag::Empty);
              if (i < cap && (((flags)[i] & Flag::Either) != 0) == 0) {
                {
                  TKey tmp = keys[i];
                  keys[i] = key;
                  key = tmp;
                }
                {
                  TValue tmp = vals[i];
                  vals[i] = val;
                  val = tmp;
                }
                ((flags)[i] |= Flag::Deleted);
              } else {
                keys[i] = key;
                vals[i] = val;
                break;
              }
            }
          }
        }
        if (cap > ensure) {
          keys = (TKey *) std::realloc((void *) keys, ensure * sizeof(TKey));
          vals = (TValue *) std::realloc((void *) vals,
            ensure * sizeof(TValue));
        }
        std::free(flags);
        flags = new_buckets;
        cap = ensure;
        occupieds = len;
        upper_bound = (u32) (cap * 0.77 + 0.5);
      }
      return true;
    }

    bool
    put(const TKey key, TSize *out) {
      TSize x;

      if (occupieds >= upper_bound) {
        if (cap > (len << 1)) {
          if (!resize(cap - 1)) {
            return false;
          }
        } else if (!resize(cap + 1)) {
          return false;
        }
      }
      {
        TSize i, site, last, mask = cap - 1, step = 0;
        u32 k;

        x = site = cap;
        k = m::hash(key);
        i = k & mask;
        last = i;
        while (1) {
          unsigned char flag = flags[i];
          if (flag & Flag::Empty) break;
          if ((flag & Flag::Deleted) || !m::eq(keys[i], key)) {
            i = (i + (++step)) & mask;
            if (i == last) {
              x = site;
              break;
            }
          } else {
            break;
          }
        }
        if (x == cap) {
          if ((((flags)[i] & Flag::Empty) == Flag::Empty) && site != cap) {
            x = site;
          } else {
            x = i;
          }
        }
      }
      *out = x;
      if ((((flags)[x] & Flag::Empty) == Flag::Empty)) {
        keys[x] = key;
        ((flags)[x] = 0);
        ++len;
        ++occupieds;
        return true;
      } else if ((((flags)[x] & Flag::Deleted) == Flag::Deleted)) {
        keys[x] = key;
        ((flags)[x] = 0);
        ++len;
        return true;
      }
      return false;
    }

    inline TValue &
    operator[](const TKey key) {
      TSize idx;

      if (!get(key, &idx) && !put(key, &idx)) {
        throw "";
      }
      return vals[idx];
    }
  };
}

#endif /* !__DS_MAP_H */
