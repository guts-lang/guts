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

#include <cute.h>
#include <ds/vec.h>

#include "app/opt.h"

#define STRNSIZE(s) (s), (sizeof(s)-1)

typedef struct my_app my_app_t;

struct my_app {
  bool_t echo, pp;
  char_t __const *output;
  strvec_t inputs;
};

ret_t
my_app_echo(my_app_t *app, UNUSED char_t __const *val) {
  app->echo = true;
  return RET_SUCCESS;
}

ret_t
my_app_pp(my_app_t *app, UNUSED char_t __const *val) {
  app->pp = true;
  return RET_SUCCESS;
}

ret_t
my_app_set_output(my_app_t *app, char_t __const *val) {
  app->output = val;
  return RET_SUCCESS;
}

ret_t
my_app_add_input(my_app_t *app, char_t __const *val) {
  return strvec_push(&app->inputs, (char_t *) val);
}

CUTEST_DATA { my_app_t my_app;opts_t opts; };

CUTEST_SETUP {
  static opt_t opts[4] = {
    {
      'S', "prepossess", "output the prepossessed input file", 
      (optcb_t) my_app_pp
    },
    { 0, "echo", "print the content of the input file", (optcb_t) my_app_echo },
    { 'o', "output", "output file name", (optcb_t) my_app_set_output, true },
    { 0, nil }
  };
  init(&self->my_app, my_app_t);
  init(&self->opts, opts_t);
  strvec_ctor(&self->my_app.inputs);
  opts_ctor(&self->opts, opts, (optcb_t) my_app_add_input);
}

CUTEST_TEARDOWN {
  strvec_dtor(&self->my_app.inputs);
  opts_dtor(&self->opts);
}

CUTEST(opt, input);
CUTEST(opt, catval);
CUTEST(opt, mmatch);
CUTEST(opt, unrecognized);
CUTEST(opt, missing1);
CUTEST(opt, missing2);
CUTEST(opt, duplicate);

i32_t
main(void) {
  CUTEST_DATA test = { 0 };

  CUTEST_PASS(opt, input);
  CUTEST_PASS(opt, catval);
  CUTEST_PASS(opt, mmatch);
  CUTEST_PASS(opt, unrecognized);
  CUTEST_PASS(opt, missing1);
  CUTEST_PASS(opt, missing2);
  CUTEST_PASS(opt, duplicate);
  return EXIT_SUCCESS;
}

CUTEST(opt, input) {
  char_t *args[5] = { "cli", "-o", "bla", "-S", "test/app.c" };

  opts_parse(&self->opts, &self->my_app, 5, args);
  ASSERT(self->my_app.output);
  ASSERT_SEQ("bla", self->my_app.output);
  ASSERT_EQ(true, self->my_app.pp);
  return CUTE_SUCCESS;
}

CUTEST(opt, catval) {
  char_t *args[2] = { "cli", "-obla" };

  opts_parse(&self->opts, &self->my_app, 2, args);
  ASSERT(self->my_app.output);
  ASSERT_SEQ("bla", self->my_app.output);
  return CUTE_SUCCESS;
}

CUTEST(opt, mmatch) {
  char_t *args[5] = { "cli", "--output", "bla", "--echo", "--prepossess" };

  opts_parse(&self->opts, &self->my_app, 5, args);
  ASSERT(self->my_app.output);
  ASSERT_SEQ("bla", self->my_app.output);
  ASSERT_EQ(true, self->my_app.echo);
  ASSERT_EQ(true, self->my_app.pp);
  return CUTE_SUCCESS;
}

CUTEST(opt, unrecognized) {
  err_t er;
  char_t *args[5] = { "cli", "--foo", "-foo", "-b", "--echo" };

  opts_parse(&self->opts, &self->my_app, 5, args);
  ASSERT_EQ(3, self->opts.errs.len);
  er = self->opts.errs.buf[0];
  ASSERT_SEQ("unrecognized command line option ‘foo’", er.msg);
  er = self->opts.errs.buf[1];
  ASSERT_SEQ("unrecognized command line option ‘f’", er.msg);
  er = self->opts.errs.buf[2];
  ASSERT_SEQ("unrecognized command line option ‘b’", er.msg);
  return CUTE_SUCCESS;
}

CUTEST(opt, missing1) {
  err_t er;
  char_t *args[5] = { "cli", "--output" };

  opts_parse(&self->opts, &self->my_app, 2, args);
  ASSERT_EQ(1, self->opts.errs.len);
  er = self->opts.errs.buf[0];
  ASSERT_SEQ("missing argument for command line option ‘output’", er.msg);
  return CUTE_SUCCESS;
}

CUTEST(opt, missing2) {
  err_t er;
  char_t *args[5] = { "cli", "-o" };

  opts_parse(&self->opts, &self->my_app, 2, args);
  ASSERT_EQ(1, self->opts.errs.len);
  er = self->opts.errs.buf[0];
  ASSERT_SEQ("missing argument for command line option ‘o’", er.msg);
  return CUTE_SUCCESS;
}

CUTEST(opt, duplicate) {
  err_t er;
  char_t *args[4] = { "cli", "-obla", "--echo", "-S" };
  char_t *args2[8] = {
    "cli", "--output", "bla", "--echo", "-obla", "-o", "bla", "-S"
  };

  opts_parse(&self->opts, &self->my_app, 4, args);
  opts_parse(&self->opts, &self->my_app, 8, args2);
  ASSERT_EQ(5, self->opts.errs.len);
  er = self->opts.errs.buf[0];
  ASSERT_SEQ("duplicate value for command line option ‘output’: ‘bla’", er.msg);
  er = self->opts.errs.buf[1];
  ASSERT_SEQ("duplicate command line option ‘echo’", er.msg);
  er = self->opts.errs.buf[2];
  ASSERT_SEQ("duplicate value for command line option ‘o’: ‘bla’", er.msg);
  er = self->opts.errs.buf[3];
  ASSERT_SEQ("duplicate value for command line option ‘o’: ‘bla’", er.msg);
  er = self->opts.errs.buf[4];
  ASSERT_SEQ("duplicate command line option ‘S’", er.msg);
  return CUTE_SUCCESS;
}
