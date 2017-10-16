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

/*!@file app/opt.h
 * @author uael
 */
#ifndef __APP_OPT_H
# define __APP_OPT_H

#include <nt/tys.h>
#include <ds/map.h>
#include <ds/vec.h>

typedef struct opt opt_t;
typedef struct opts opts_t;
typedef bool_t (*optcb_t)(void *app, __const char *val);

struct opt {
  char_t f, *lf, *help;
  optcb_t callback;
  bool_t kval, global, match;
};

__api__ i32_t
opt_cmp(opt_t a, opt_t b);

STR_MAP_DEFINE(optmap, opt_t, opt_cmp)
I8_MAP_DEFINE(optmap_sc, opt_t *, i64cmp)

struct opts {
  char_t __const *program;
  optmap_t conf;
  optmap_sc_t shortcuts;
  optcb_t callback;
  errs_t errs;
};

__api__ opt_t *
opts_get(opts_t *opts, char_t id);

__api__ opt_t *
opts_lget(opts_t *opts, char_t __const *id);

__api__ void
opts_ctor(opts_t *self, opt_t *opts, optcb_t callback);

__api__ void
opts_dtor(opts_t *self);

__api__ bool_t
opts_parse(opts_t *self, void *app_ptr, i32_t argc, char_t **argv);

#endif /* !__APP_OPT_H */
