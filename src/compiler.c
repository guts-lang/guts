/*
 * MIT License
 *
 * Copyright (c) 2014 CreoLabs. All rights reserved.
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

#include <stdarg.h>
#include <stdio.h>

#include "compiler.h"

#include "util/io.h"

void jl_init(jl_compiler_t *self, int argc, char **argv) {
  char *sep;

  if ((sep = strrchr(argv[0], '/'))) {
    argv[0] = ++sep;
  }
  *self = (jl_compiler_t) {
    .program = argv[0]
  };
  if (!jl_opts_init(&self->opts, argc, argv)) {
    jl_opts_dtor(&self->opts);
    exit(EXIT_SUCCESS);
  }
  if (!self->opts.in) {
    jl_fatal_err(self, "no input file");
  }
  if (ds_size(self->opts.opts_errs)) {
    jl_err(self, ds_front(self->opts.opts_errs));
  }
  jl_fe_init(&self->fe, JL_PARSER_C, self);
  jl_fe_push_src(&self->fe, self->opts.in);
}

void jl_dtor(jl_compiler_t *self) {
  size_t i;

  jl_opts_dtor(&self->opts);
  if (jl_defined(self->fe)) {
    jl_fe_dtor(&self->fe);
  }
  foreach(self->strtab, i) {
    xfree((void *) ds_at(self->strtab, i));
  }
  vec_dtor(self->strtab);
}

void jl_parse(jl_compiler_t *self) {
  jl_fe_parse(&self->fe, NULL, &self->ast);
}

JL_NORETURN jl_err(jl_compiler_t *self, const char *format, ...) {
  va_list args;

  if (self) {
    printf(BOLD "%s: " BOLD RED "error: " RESET, self->program);
  } else {
    fputs(BOLD RED "error: " RESET, stdout);
  }
  va_start(args, format);
  vprintf(format, args);
  va_end(args);
  putc('\n', stdout);

  if (self) {
    jl_dtor(self);
  }
  exit(EXIT_FAILURE);
}

JL_NORETURN jl_fatal_err(jl_compiler_t *self, const char *format, ...) {
  va_list args;

  if (self) {
    printf(BOLD "%s: " BOLD RED "fatal error: " RESET, self->program);
  } else {
    fputs(BOLD RED "fatal error: " RESET, stdout);
  }
  va_start(args, format);
  vprintf(format, args);
  va_end(args);
  putc('\n', stdout);

  if (self) {
    jl_dtor(self);
  }
  exit(EXIT_FAILURE);
}

JL_NORETURN jl_parse_err(jl_compiler_t *self, jl_loc_t loc, const char *format, ...) {
  size_t begin = loc.position - loc.colno;
  const char *ptr, *file;
  jl_lexer_t lexer = (jl_lexer_t) {0};
  jl_token_t eol;
  va_list args;

  file = ds_at(self->fe.sources, loc.file_id);
  jl_lexer_fork(&lexer, self->fe.lexer);
  lexer.loc = loc;
  ptr = lexer.buffer + begin;
  while ((eol = jl_lexer_next(&lexer)).type != '\n' && eol.type != '\0');
  printf(BOLD "%s:" RESET " In function ‘" BOLD "syntax_error" RESET "’:\n", file);
  printf(BOLD "%s:%d:%d: " BOLD RED "error: " RESET, file, loc.colno + 1, loc.lineno + 1);
  va_start(args, format);
  vprintf(format, args);
  va_end(args);
  printf("\n%.*s\n", (int) (eol.loc.position - begin), ptr);
  for (unsigned i = 0; i < loc.colno; ++i) {
    putc(' ', stdout);
  }
  fputs(BOLD YELLOW "^\n" RESET, stdout);

  jl_lexer_dtor(&lexer, true);
  jl_dtor(self);
  exit(EXIT_FAILURE);
}

const char *jl_strdup(jl_compiler_t *self, const char *str) {
  return jl_strndup(self, str, strlen(str));
}

const char *jl_strndup(jl_compiler_t *self, const char *str, size_t n) {
  vec_push(self->strtab, "");
  ds_back(self->strtab) = xmalloc(n + 1);
  strncpy((char *) ds_back(self->strtab), str, n + 1);
  return ds_back(self->strtab);
}
