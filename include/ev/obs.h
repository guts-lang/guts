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

/*!@file ev/obs.h
 * @author uael
 */
#ifndef __EV_OBS_H
# define __EV_OBS_H

#include <nt/ex.h>
#include <ds/vec.h>

#define OBSERVER(NAME, TObservable, TCode, ...) \
  struct NAME { \
    bool_t (*update)(struct NAME *self, TObservable *sender, \
      TCode code, void *arg); \
    void (*dtor)(struct NAME *self); \
    __VA_ARGS__ \
  }

typedef OBSERVER(observer, void, i32_t, void *dummy;) observer_t;

#define observer_cmp(a, b) i64cmp(&(a), &(b))

VEC16_DEFINE(observers, observer_t *, observer_cmp)

#define OBSERVABLE(NAME, ...) \
  struct NAME { \
    observers_t observers; \
    __VA_ARGS__ \
  }

#define OBSERVABLE_DEFINE(ID, TObservable, TCode) \
  static FORCEINLINE bool_t \
  ID##_attach(TObservable *__restrict self, void *observer) { \
    return observers_push( \
      &self->observers, \
      (observer_t *) observer \
    ); \
  } \
  static FORCEINLINE bool_t \
  ID##_notify(TObservable *__restrict self, TCode code, void *arg) { \
    u16_t i; \
    bool_t ret; \
    for (i = 0; i < self->observers.len; ++i) { \
      if ((ret = self->observers.buf[i]->update( \
        self->observers.buf[i], self, code, arg)) \
        != true) { \
        return ret; \
      } \
    } \
    return true; \
  } \
  static FORCEINLINE void \
  ID##_detach(TObservable *__restrict self) { \
    u16_t i; \
    for (i = 0; i < self->observers.len; ++i) { \
      if (self->observers.buf[i]->dtor) { \
        self->observers.buf[i]->dtor(self->observers.buf[i]); \
      } \
    } \
  }

#endif /* !__EV_OBS_H */
