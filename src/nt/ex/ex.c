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

#include "nt/ex/ex.h"

static i32_t usr_code = 2000;

FORCEINLINE i32_t
ex_register(void)
{
  return usr_code++;
}

FORCEINLINE void
ex_ctor(ex_t *self, errlvl_t lvl, i32_t code, char_t __const *msg, va_list args)
{
  init(self, ex_t);
  self->lvl = lvl;
  self->code = code;
  if (msg) vsprintf(self->msg, msg, args);
}

ex_t
ex_usr(errlvl_t lvl, i32_t no, char_t __const *msg, ...)
{
  ex_t e;
  va_list args;

  va_start(args, msg);
  ex_ctor(&e, lvl, no, msg, args);
  va_end(args);
  return e;
}

ex_t
ex(errlvl_t lvl, char_t __const *msg, ...)
{
  ex_t e;
  va_list args;

  va_start(args, msg);
  ex_ctor(&e, lvl, -1, msg, args);
  va_end(args);
  return e;
}

ex_t
ex_errno(errlvl_t lvl, i32_t err_no, char_t __const *msg, ...)
{
  ex_t e;
  va_list args;

  va_start(args, msg);
  ex_ctor(&e, lvl, err_no, msg, args);
  va_end(args);
  if (msg) strcat(e.msg, "; ");
  strcat(e.msg, strerror(err_no));
  return e;
}

ex_t
ex_notice(char_t __const *msg, ...)
{
  ex_t e;
  va_list args;

  va_start(args, msg);
  ex_ctor(&e, ERRLVL_NOTICE, -1, msg, args);
  va_end(args);
  return e;
}

ex_t
ex_warn(char_t __const *msg, ...)
{
  ex_t e;
  va_list args;

  va_start(args, msg);
  ex_ctor(&e, ERRLVL_WARNING, -1, msg, args);
  va_end(args);
  return e;
}

ex_t
ex_error(char_t __const *msg, ...)
{
  ex_t e;
  va_list args;

  va_start(args, msg);
  ex_ctor(&e, ERRLVL_ERROR, -1, msg, args);
  va_end(args);
  return e;
}

ex_t
ex_fatal(char_t __const *msg, ...)
{
  ex_t e;
  va_list args;

  va_start(args, msg);
  ex_ctor(&e, ERRLVL_FATAL, -1, msg, args);
  va_end(args);
  return e;
}

static void
ex_dump_impl(ex_t *self, FILE *stream);

void
ex_dump(ex_t *self, FILE *stream) {
  if (self->dump && self->dump != ex_dump) {
    self->dump(self, stream);
  } else {
    ex_dump_impl(self, stream);
  }
}

#ifndef CC_MSVC
# define _COLOR_RESET   "\033[0m"
# define _COLOR_RED     "\033[31m"
# define _COLOR_YELLOW  "\033[33m"
# define _COLOR_CYAN    "\033[36m"
# define _COLOR_BOLD    "\033[1m"
#else
# define _COLOR_RESET ""
# define _COLOR_RED ""
# define _COLOR_YELLOW ""
# define _COLOR_CYAN ""
# define _COLOR_BOLD ""
#endif

FORCEINLINE void
ex_dump_impl(ex_t *self, FILE *stream) {
  char_t __const *lvl, *lvl_color;
  FILE *file;

  if (self->prev) {
    ex_dump(self->prev, stream);
  }
  switch (self->lvl) {
    case ERRLVL_NOTICE:
      lvl = "notice";
      lvl_color = _COLOR_CYAN;
      break;
    case ERRLVL_WARNING:
      lvl = "warning";
      lvl_color = _COLOR_YELLOW;
      break;
    case ERRLVL_FATAL:
      lvl = "fatal";
      lvl_color = _COLOR_RED;
      break;
    default:
      lvl = "error";
      lvl_color = _COLOR_RED;
      break;
  }
  if (self->fn) {
    fprintf(stream,
      _COLOR_BOLD "%s:" _COLOR_RESET " In function '" _COLOR_BOLD
    "%s" _COLOR_RESET "':\n",
      self->file, self->fn
    );
  } else {
    fprintf(stream,
      _COLOR_BOLD "%s:" _COLOR_RESET " In function:\n",
      self->file
    );
  }
  if (self->code > 0) {
    fprintf(stream,
      _COLOR_BOLD "%s:%d:" _COLOR_RESET " %s" _COLOR_BOLD "%s (%d):"
    _COLOR_RESET " %s%s\n" _COLOR_RESET,
      self->file, self->line, lvl_color, lvl, self->code, lvl_color, self->msg
    );
  } else {
    fprintf(stream,
      _COLOR_BOLD "%s:%d:" _COLOR_RESET " %s" _COLOR_BOLD "%s:" _COLOR_RESET
    " %s%s\n" _COLOR_RESET,
      self->file, self->line, lvl_color, lvl, lvl_color, self->msg
    );
  }
  if ((file = fopen(self->file, "r")) != nil) {
    char_t buf[4096], *begin, *end;
    u64_t size;
    u16_t i;
    i16_t j;

    i = 0;
    j = (i16_t) (self->line - 2);
    if (j < 0) j = 0;
    begin = end = nil;
    while ((size = fread(buf, 1, 4096, file)) > 0) {
      end = buf;
      loop:
      while (size && *end && *end != '\n') {
        ++end;
        if (--size == 0) continue;
      }
      if (*end == '\n') {
        if (++i == self->line) break;
        ++end;
        if (i < j) begin = end;
        if (--size) goto loop;
      }
    }
    if (begin && end) {
      fprintf(stream, "%.*s\n", (int) (end - begin), begin);
      if (self->col) {
        while (--self->col) putc(' ', stream);
      } else {
        while (*begin && *(begin++) <= ' ') putc(' ', stream);
      }
      fputs("^\n", stream);
    }
    fclose(file);
  }
}
