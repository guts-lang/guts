/*
 * MIT License
 *
 * Copyright (c) 2016-2017 uael <www.github.com/uael>
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

#include <stdlib.h>
#include <stdio.h>

#include <adt/string.h>

#include "lexer.h"

#include "util/io.h"
#include "compiler.h"
#include "c/c_lexer.h"

void jl_lexer_init(jl_lexer_t *self, jl_compiler_t *compiler, uint32_t file_id, const char *buffer, size_t length) {
  *self = (jl_lexer_t) {
    .compiler = compiler,
    .loc = (jl_loc_t) {
      .file_id = file_id
    },
    .buffer = buffer ? xstrndup(buffer, length) : NULL,
    .length = length,
    .cap = 10
  };

  if (self->buffer) {
    self->buffer[length] = '\0';
  }
  switch (compiler->fe.kind) {
    case JL_PARSER_C:
      c_lexer_init(self);
      break;
    case JL_PARSER_JAY:
      break;
    default:
      break;
  }
}

void jl_lexer_init_f(jl_lexer_t *self, jl_parser_t *fe) {
  size_t len;
  uint32_t file_id;
  const char *filename, *buffer;

  if (!adt_deque_length(fe->sources)) {
    jl_fatal_err(fe->compiler, "No input files");
  }

  file_id = (uint32_t) adt_deque_cursor(fe->sources);
  filename = adt_deque_shift(fe->sources);
  if (!jl_fexists(filename)) {
    jl_fatal_err(fe->compiler, "Input file does not exists '%s'", filename);
  }
  buffer = jl_fread(filename, &len);
  if (!buffer) {
    jl_fatal_err(fe->compiler, "Unable to read input file '%s'", filename);
  }

  jl_lexer_init(self,  fe->compiler, file_id, buffer, len);
  xfree((char *) buffer);
}

void jl_lexer_dtor(jl_lexer_t *self, bool free_all) {
  size_t i;
  jl_token_t token;
  jl_lexer_event_t event;

  if (self->buffer && jl_lexer_is_root(self)) {
    xfree(self->buffer);
  }
  self->queue.cursor = 0;
  adt_deque_foreach(self->queue, token) {
    jl_token_dtor(&token);
  }
  foreach(self->events, i) {
    event = ds_at(self->events, i);
    if (event.dtor) {
      event.dtor(&event);
    }
  }
  foreach(self->childs, i) {
    jl_lexer_dtor(ds_data(self->childs) + i, true);
  }
  if (free_all) {
    vec_dtor(self->childs);
    adt_deque_dtor(self->queue);
    vec_dtor(self->events);
  } else {
    vec_clear(self->childs);
    adt_deque_clear(self->queue);
    vec_clear(self->events);
  }
}

void jl_lexer_fork(jl_lexer_t *destination, jl_lexer_t *source) {
  destination->parent = source;
  destination->compiler = source->compiler;
  destination->loc = source->loc;
  destination->buffer = source->buffer;
  destination->length = source->length;
  destination->enqueue = source->enqueue;
  destination->cap = 1;
}

void jl_lexer_join(jl_lexer_t *fork) {
  if (adt_deque_size(fork->queue)) {
    fork->parent->loc = fork->loc;
  }
  adt_deque_clear(fork->queue);
}

void jl_lexer_skip(jl_lexer_t *self, unsigned n) {
  self->loc.position += n;
}

size_t jl_lexer_length(jl_lexer_t *self) {
  return adt_deque_length(self->queue);
}

bool jl_lexer_push(jl_lexer_t *self, jl_token_t token) {
  jl_lexer_event_t event;
  unsigned i;

  foreach(self->events, i) {
    event = ds_at(self->events, i);
    if (event.kind == JL_LEXER_EVENT_ON_PUSH) {
      if (!event.callback(&event, &token)) {
        jl_token_dtor(&token);
        return false;
      }
    }
  }
  token.cursor = adt_deque_size(self->queue);
  adt_deque_push(self->queue, token);
  if (jl_lexer_is_root(self) && self->compiler->opts.echo) {
    for (i = 0; i<token.leading_ws; ++i) {
      putc(' ', stdout);
    }
    switch (token.kind) {
      case JL_TOKEN_IDENTIFIER:
      case JL_TOKEN_NUMBER:
        fputs(token.value, stdout);
        break;
      default:
        fputs(token.name, stdout);
        break;
    }
  }
  return true;
}

void jl_lexer_attach(jl_lexer_t *self, jl_lexer_event_t event) {
  event.lexer = self;
  vec_push(self->events, event);
}

bool jl_lexer_is_root(jl_lexer_t *self) {
  return self->parent == NULL;
}

void jl_lexer_next_src(jl_lexer_t *self, jl_parser_t *fe) {
  jl_lexer_dtor(self, false);
  jl_lexer_init_f(self, fe);
  self->compiler = fe->compiler;
}

void jl_lexer_enqueue(jl_lexer_t *self, unsigned n) {
  if (ds_size(self->childs)) {
    jl_lexer_enqueue(&ds_back(self->childs), n);
  } else if (self->loc.position < self->length) {
    self->enqueue(self, n);
  } else if (!jl_lexer_is_root(self)) {
    (void) vec_pop(self->parent->childs);
  } else if (!jl_lexer_length(self)) {
    if (adt_deque_length(self->compiler->fe.sources)) {
      jl_lexer_next_src(self, &self->compiler->fe);
    } else {
      jl_token_t token = {'\0'};
      jl_lexer_push(self, token);
    }
  }
}

jl_token_t jl_lexer_peek(jl_lexer_t *self) {
  if (jl_lexer_length(self) < 1) {
    jl_lexer_enqueue(self, self->cap);
  }
  return adt_deque_front(self->queue);
}

jl_token_t jl_lexer_peekn(jl_lexer_t *self, unsigned n) {
  n++;
  if (jl_lexer_length(self) <= n) {
    jl_lexer_enqueue(self, n < self->cap ? self->cap : n);
  }
  return adt_deque_at(
    self->queue,
    (jl_lexer_length(self) < n ? jl_lexer_length(self) : n)-1
  );
}

jl_token_t jl_lexer_next(jl_lexer_t *self) {
  jl_token_t result;

  if (jl_lexer_length(self) < 1) {
    jl_lexer_enqueue(self, self->cap);
  }
  if (jl_lexer_length(self) && adt_deque_front(self->queue).type != 0) {
    result = adt_deque_shift(self->queue);
  } else {
    result = adt_deque_front(self->queue);
  }
  return result;
}

jl_token_t jl_lexer_consume(jl_lexer_t *self, unsigned char type) {
  jl_token_t result;

  if (type != '\n') {
    while (jl_lexer_peek(self).type == '\n') {
      jl_lexer_next(self);
    }
  }

  if ((result = jl_lexer_peek(self)).type != type) {
    jl_parse_err(self->compiler, result.loc,
      "Unexpected token '" BOLD "%c" RESET "' expected '" BOLD "%c" "'" RESET,
      result.type,
      type
    );
  }
  jl_lexer_next(self);
  return result;
}

jl_token_t jl_lexer_consume_id(jl_lexer_t *self, const char *id) {
  jl_token_t result;
  if ((result = jl_lexer_peek(self)).kind != JL_TOKEN_IDENTIFIER) {
    jl_parse_err(self->compiler, result.loc,
      "Unexpected token '" BOLD "%c" RESET "' expected identifier '" BOLD "%s" "'" RESET,
      result.type,
      id
    );
  }
  if (id && strcmp(id, result.value) != 0) {
    jl_parse_err(self->compiler, result.loc,
      "Unexpected identifier '" BOLD "%s" RESET "' expected '" BOLD "%s" "'" RESET,
      result.value,
      id
    );
  }
  jl_lexer_next(self);
  return result;
}

void jl_lexer_undo(jl_lexer_t *lexer, jl_token_t until) {
  while (adt_deque_cursor(lexer->queue) > until.cursor) --adt_deque_cursor(lexer->queue);
}

void jl_lexer_undon(jl_lexer_t *lexer, unsigned n) {
  adt_deque_cursor(lexer->queue) -= n;
}
