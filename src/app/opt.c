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

#include "app/opt.h"

FORCEINLINE i32_t
opt_cmp(opt_t a, opt_t b)
{
  i32_t cmp;

  if ((cmp = i8cmp(a.f, b.f)) != 0) {
    return cmp;
  }
  return strcmp(a.lf, b.lf);
}

FORCEINLINE opt_t *
opts_get(opts_t *opts, char_t id)
{
  u32_t it;

  if (optmap_sc_get(&opts->shortcuts, id, &it)) {
    return opts->shortcuts.vals[it];
  }
  return nil;
}

FORCEINLINE opt_t *
opts_lget(opts_t *opts, char_t __const *id)
{
  unsigned it;

  if (optmap_get(&opts->conf, id, &it)) {
    return &opts->conf.vals[it];
  }
  return nil;
}

void
opts_ctor(opts_t *self, opt_t *opts, optcb_t callback)
{
  u32_t it;

  self->callback = callback;
  errs_ctor(&self->errs);
  if (opts) {
    while (opts->lf) {
      opt_t *opt = opts++;

      if (optmap_put(&self->conf, opt->lf, &it)) {
        self->conf.vals[it] = *opt;
        opt = self->conf.vals + it;
      }
      if (opt->f) {
        if (optmap_sc_put(&self->shortcuts, opt->f, &it)) {
          self->shortcuts.vals[it] = opt;
        }
      }
    }
  }
}

void
opts_dtor(opts_t *self)
{
  optmap_dtor(&self->conf, nil, nil);
  optmap_sc_dtor(&self->shortcuts, nil, nil);
  errs_dtor(&self->errs, nil);
}

bool_t
opts_parse(opts_t *self, void *app_ptr, i32_t argc, char_t **argv)
{
  char_t *arg, key, *lkey, *val;
  opt_t *opt;
  i32_t i;
  ex_t err;

  if (argc) {
    if ((self->program = strrchr(argv[0], '/'))) {
      ++self->program;
    } else {
      self->program = argv[0];
    }
    for (i = 1; i < argc; ++i) {
      arg = argv[i];
      val = nil;
      if (*arg == '-') {
        if (*(arg + 1) == '-') {
          opt = opts_lget(self, lkey = arg + 2);
          if (opt == nil) {
            err = ex_warn("unrecognized command line option ‘%s’", lkey);
            goto fail;
          }
          if (opt->kval) {
            if (i < argc - 1) {
              val = argv[++i];
            } else {
              err = ex_warn(
                "missing argument for command line option ‘%s’", lkey
              );
              goto fail;
            }
            if (!opt->global && opt->match) {
              err = ex_warn(
                "duplicate value for command line option ‘%s’: ‘%s’", lkey, val
              );
              goto fail;
            }
          } else if (!opt->global && opt->match) {
            err = ex_warn("duplicate command line option ‘%s’", lkey);
            goto fail;
          }
        } else {
          opt = opts_get(self, key = arg[1]);
          if (opt == nil) {
            err = ex_warn("unrecognized command line option ‘%c’", key);
            goto fail;
          }
          if (opt->kval) {
            if (arg[2] != '\0') {
              val = arg + 2;
            } else if (i < argc - 1) {
              val = argv[++i];
            } else {
              err = ex_warn(
                "missing argument for command line option ‘%c’", key
              );
              goto fail;
            }
            if (!opt->global && opt->match) {
              err = ex_warn(
                "duplicate value for command line option ‘%c’: ‘%s’", key, val
              );
              goto fail;
            }
          } else if (arg[2] != '\0') {
            err = ex_warn("unrecognized command line option ‘%c’", key);
            goto fail;
          } else if (!opt->global && opt->match) {
            err = ex_warn("duplicate command line option ‘%c’", key);
            goto fail;
          }
        }
        if (opt->callback == nil || opt->callback(app_ptr, val)) {
          opt->match = true;
        }
      } else if (self->callback && !self->callback(app_ptr, arg)) {
        err = ex_error("invalid command line argument ‘%s’", arg);
        goto fail;
      }
      continue;
      fail:
      errs_push(&self->errs, err);
    }
  }
  return self->errs.len ? false : true;
}
