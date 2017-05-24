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

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include <assert.h>

#include "expr.h"

#include "entity.h"
#include "stmt.h"
#include "type.h"

jl_expr_t jl_exprs(jl_expr_t *exprs) {
  jl_expr_t expr = (jl_expr_t) {
    .kind = JL_EXPR_EXPRS
  };

  if (exprs) {
    while (exprs->kind) {
      vec_push(expr.exprs.vector, *exprs);
      ++exprs;
    }
  }
  return expr;
}

jl_expr_t jl_exprs_start(jl_expr_t expr) {
  jl_expr_t exprs = (jl_expr_t) {
    .kind = JL_EXPR_EXPRS
  };

  vec_push(exprs.exprs.vector, expr);
  return exprs;
}

static void jl_exprs_dtor(jl_exprs_t *self) {
  size_t i;

  foreach(self->vector, i) {
    jl_expr_dtor(ds_data(self->vector) + i);
  }
  vec_dtor(self->vector);
}

jl_expr_t jl_id(jl_lloc_t lloc, const char *id, jl_type_t type) {
  return (jl_expr_t) {
    .kind = JL_EXPR_ID,
    .id = {
      .id = id,
      .lloc = lloc,
      .type = type
    }
  };
}

static void jl_id_dtor(jl_id_t *self) {
  jl_type_dtor(&self->type);
}

jl_expr_t jl_const_int(jl_lloc_t lloc, int d) {
  return jl_const(lloc, jl_val(jl_int(), .ul = (unsigned long) d));
}

jl_expr_t jl_const_float(jl_lloc_t lloc, float f) {
  return jl_const(lloc, jl_val(jl_float(), .f = f));
}

jl_expr_t jl_const_string(jl_lloc_t lloc, const char *s) {
  return jl_const(lloc, jl_val(jl_pointer(jl_char()), .s = s));
}

jl_expr_t jl_const(jl_lloc_t lloc, jl_val_t value) {
  assert(jl_defined(value.type));
  return (jl_expr_t) {
    .kind = JL_EXPR_CONST,
    .constant = {
      .value = value,
      .lloc = lloc
    }
  };
}

static void jl_const_dtor(jl_const_t *self) {
  jl_type_dtor(&self->type);
}

jl_expr_t jl_unary(jl_lloc_t op_lloc, enum jl_op_n op, jl_expr_t operand) {
  jl_expr_t expr = (jl_expr_t) {
    .kind = JL_EXPR_UNARY,
    .unary = {
      .op = op,
      .operand = xmalloc(sizeof(jl_expr_t)),
      .lloc = jl_lloc(op_lloc, operand.lloc),
      .type = operand.type
    }
  };

  *expr.unary.operand = operand;
  return expr;
}

jl_expr_t jl_cast(jl_lloc_t type_lloc, jl_type_t type, jl_expr_t operand) {
  jl_expr_t expr = (jl_expr_t) {
    .kind = JL_EXPR_UNARY,
    .unary = {
      .op = JL_OP_CAST,
      .operand = xmalloc(sizeof(jl_expr_t)),
      .lloc = jl_lloc(type_lloc, operand.lloc),
      .type = type
    }
  };

  *expr.unary.operand = operand;
  return expr;
}

static void jl_unary_dtor(jl_unary_t *self) {
  if (self->op == JL_OP_CAST) {
    jl_type_dtor(&self->type);
  }
  jl_expr_dtor(self->operand);
  xfree(self->operand);
  self->operand = NULL;
}

jl_expr_t jl_binary(enum jl_op_n op, jl_expr_t lhs, jl_expr_t rhs) {
  jl_expr_t expr = (jl_expr_t) {
    .kind = JL_EXPR_BINARY,
    .binary = {
      .op = op,
      .lhs = xmalloc(sizeof(jl_expr_t)),
      .rhs = xmalloc(sizeof(jl_expr_t)),
      .lloc = jl_lloc(lhs.lloc, rhs.lloc),
      .type = lhs.type
    }
  };

  *expr.binary.lhs = lhs;
  *expr.binary.rhs = rhs;
  return expr;
}

static void jl_binary_dtor(jl_binary_t *self) {
  jl_expr_dtor(self->lhs);
  xfree(self->lhs);
  self->lhs = NULL;
  jl_expr_dtor(self->rhs);
  xfree(self->rhs);
  self->rhs = NULL;
}

jl_expr_t jl_ternary(jl_expr_t lhs, jl_expr_t mhs, jl_expr_t rhs) {
  jl_expr_t expr = (jl_expr_t) {
    .kind = JL_EXPR_TERNARY,
    .ternary = {
      .lhs = xmalloc(sizeof(jl_expr_t)),
      .mhs = xmalloc(sizeof(jl_expr_t)),
      .rhs = xmalloc(sizeof(jl_expr_t)),
      .lloc = jl_lloc(lhs.lloc, rhs.lloc),
      .type = mhs.type
    }
  };

  *expr.ternary.lhs = lhs;
  *expr.ternary.mhs = mhs;
  *expr.ternary.rhs = rhs;
  return expr;
}

static void jl_ternary_dtor(jl_ternary_t *self) {
  jl_expr_dtor(self->lhs);
  xfree(self->lhs);
  self->lhs = NULL;
  jl_expr_dtor(self->mhs);
  xfree(self->mhs);
  self->mhs = NULL;
  jl_expr_dtor(self->rhs);
  xfree(self->rhs);
  self->rhs = NULL;
}

jl_expr_t jl_array_read(jl_expr_t lhs, jl_lloc_t pos_lloc, jl_expr_t pos) {
  jl_expr_t expr = (jl_expr_t) {
    .kind = JL_EXPR_ARRAY_READ,
    .array_read = {
      .lhs = xmalloc(sizeof(jl_expr_t)),
      .pos = xmalloc(sizeof(jl_expr_t)),
      .lloc = jl_lloc(lhs.lloc, pos_lloc),
      .type = jl_type_deref(lhs.type)
    }
  };

  *expr.array_read.lhs = lhs;
  *expr.array_read.pos = pos;
  return expr;
}

static void jl_array_read_dtor(jl_array_read_t *self) {
  jl_type_dtor(&self->type);
  jl_expr_dtor(self->lhs);
  xfree(self->lhs);
  self->lhs = NULL;
  jl_expr_dtor(self->pos);
  xfree(self->pos);
  self->pos = NULL;
}

jl_expr_t jl_array_write(jl_array_read_t array_read, jl_expr_t rhs) {
  jl_expr_t expr = (jl_expr_t) {
    .kind = JL_EXPR_ARRAY_WRITE,
    .array_write = {
      .read = array_read,
      .rhs = xmalloc(sizeof(jl_expr_t)),
    }
  };

  expr.lloc = jl_lloc(array_read.lloc, rhs.lloc);
  *expr.array_write.rhs = rhs;
  return expr;
}

static void jl_array_write_dtor(jl_array_write_t *self) {
  jl_array_read_dtor(&self->read);
  jl_expr_dtor(self->rhs);
  xfree(self->rhs);
  self->rhs = NULL;
}

jl_expr_t jl_field_read(jl_expr_t lhs, jl_id_t id) {
  jl_field_t *field = jl_field_lookup(lhs.type, id.id);
  jl_expr_t expr = (jl_expr_t) {
    .kind = JL_EXPR_FIELD_READ,
    .field_read = {
      .lhs = xmalloc(sizeof(jl_expr_t)),
      .id = id,
      .lloc = jl_lloc(lhs.lloc, id.lloc),
      .type = field ? field->type : jl_type_undefined()
    }
  };

  *expr.field_read.lhs = lhs;
  return expr;
}

static void jl_field_read_dtor(jl_field_read_t *self) {
  jl_id_dtor(&self->id);
  jl_expr_dtor(self->lhs);
  xfree(self->lhs);
  self->lhs = NULL;
}

jl_expr_t jl_field_write(jl_field_read_t field_read, jl_expr_t rhs) {
  jl_expr_t expr = (jl_expr_t) {
    .kind = JL_EXPR_FIELD_WRITE,
    .field_write = {
      .read = field_read,
      .rhs = xmalloc(sizeof(jl_expr_t))
    }
  };

  expr.lloc = jl_lloc(field_read.lloc, rhs.lloc);
  *expr.field_write.rhs = rhs;
  return expr;
}

static void jl_field_write_dtor(jl_field_write_t *self) {
  jl_field_read_dtor(&self->read);
  jl_expr_dtor(self->rhs);
  xfree(self->rhs);
  self->rhs = NULL;
}

jl_expr_t jl_call(jl_expr_t lhs, jl_lloc_t args_lloc, jl_exprs_t args) {
  jl_expr_t expr = (jl_expr_t) {
    .kind = JL_EXPR_CALL,
    .call = {
      .lhs = xmalloc(sizeof(jl_expr_t)),
      .args = args,
      .lloc = jl_lloc(lhs.lloc, args_lloc)
    }
  };

  *expr.call.lhs = lhs;
  return expr;
}

static void jl_call_dtor(jl_call_t *self) {
  jl_expr_dtor(self->lhs);
  xfree(self->lhs);
  self->lhs = NULL;
  jl_exprs_dtor(&self->args);
}

void jl_expr_dtor(jl_expr_t *self) {
  switch (self->kind) {
    case JL_EXPR_UNDEFINED:
      break;
    case JL_EXPR_EXPRS:
      jl_exprs_dtor(&self->exprs);
      break;
    case JL_EXPR_ID:
      jl_id_dtor(&self->id);
      break;
    case JL_EXPR_CONST:
      jl_const_dtor(&self->constant);
      break;
    case JL_EXPR_UNARY:
      jl_unary_dtor(&self->unary);
      break;
    case JL_EXPR_BINARY:
      jl_binary_dtor(&self->binary);
      break;
    case JL_EXPR_TERNARY:
      jl_ternary_dtor(&self->ternary);
      break;
    case JL_EXPR_ARRAY_READ:
      jl_array_read_dtor(&self->array_read);
      break;
    case JL_EXPR_ARRAY_WRITE:
      jl_array_write_dtor(&self->array_write);
      break;
    case JL_EXPR_FIELD_READ:
      jl_field_read_dtor(&self->field_read);
      break;
    case JL_EXPR_FIELD_WRITE:
      jl_field_write_dtor(&self->field_write);
      break;
    case JL_EXPR_CALL:
      jl_call_dtor(&self->call);
      break;
    default:
      break;
  }
  *self = jl_expr_undefined();
}

bool jl_expr_is_constant(jl_expr_t self) {
  switch (self.kind) {
    case JL_EXPR_CONST:
      return true;
    case JL_EXPR_UNARY:
      return jl_expr_is_constant(*self.unary.operand);
    case JL_EXPR_BINARY:
      return jl_expr_is_constant(*self.binary.lhs) && jl_expr_is_constant(*self.binary.rhs);
    case JL_EXPR_TERNARY:
      return jl_expr_is_constant(*self.ternary.lhs) && jl_expr_is_constant(*self.ternary.mhs)
             && jl_expr_is_constant(*self.ternary.rhs);
    default:
      return false;
  }
}

enum suffix {
  SUFFIX_NONE = 0,
  SUFFIX_U = 0x1,
  SUFFIX_L = 0x2,
  SUFFIX_UL = SUFFIX_U | SUFFIX_L,
  SUFFIX_LL = (SUFFIX_L << 1) | SUFFIX_L,
  SUFFIX_ULL = SUFFIX_U | SUFFIX_LL
};

static enum suffix read_integer_suffix(char *ptr, char **endptr) {
  enum suffix s = SUFFIX_NONE;

  if (tolower(*ptr) == 'u') {
    s = SUFFIX_U;
    ptr++;
  }
  if (tolower(*ptr) == 'l') {
    s |= SUFFIX_L;
    ptr++;
    if (*ptr == ptr[-1]) {
      s |= SUFFIX_LL;
      ptr++;
    }

    if (!(s & SUFFIX_U) && tolower(*ptr) == 'u') {
      s |= SUFFIX_U;
      ptr++;
    }
  }
  *endptr = ptr;
  return s;
}

static jl_type_t constant_integer_type(unsigned long int value, enum suffix suffix, int is_decimal) {
  switch (suffix) {
    case SUFFIX_U:
      if (value <= UINT_MAX) {
        return jl_uint();
      } else {
        return jl_ulong();
      }
    case SUFFIX_L:
    case SUFFIX_LL:
      if (value <= LONG_MAX) {
        return jl_long();
      } else {
        if (is_decimal) {
          puts("Conversion of decimal constant to unsigned.");
        }
        return jl_ulong();
      }
    case SUFFIX_UL:
    case SUFFIX_ULL:
      return jl_ulong();
    case SUFFIX_NONE:
    default:
      if (value <= INT_MAX) {
        return jl_int();
      } else if (!is_decimal && value <= UINT_MAX) {
        return jl_uint();
      } else if (value <= LONG_MAX) {
        return jl_long();
      } else {
        if (is_decimal) {
          puts("Conversion of decimal constant to unsigned.");
        }
        return jl_ulong();
      }
  }
}

int jl_const_parse(jl_lloc_t lloc, const char *s, size_t len, jl_expr_t *out) {
  const char *str;
  char *endptr;
  enum suffix suffix;

  str = s;
  /*
   * Try to read as integer. Handle suffixes u, l, ll, ul, ull, in all
   * permutations of upper- and lower case.
   */
  errno = 0;
  *out = (jl_expr_t) {.kind = JL_EXPR_CONST, .lloc = lloc};
  out->constant.ul = strtoul(str, &endptr, 0);
  suffix = read_integer_suffix(endptr, &endptr);
  if ((size_t) (endptr - str) == len) {
    assert(isdigit(*str));
    out->type = constant_integer_type(out->constant.ul, suffix, *str != '0');
  } else {
    /*
     * If the integer conversion did not consume the whole token,
     * try to read as floating point number.
     *
     * Note: not using strtold for long double conversion, so might
     * get incorrect results compared to other compilers.
     */
    errno = 0;
    out->constant.d = strtod(str, &endptr);
    if ((size_t) (endptr - str) < len) {
      if (*endptr == 'f' || *endptr == 'F') {
        out->constant.f = (float) out->constant.d;
        out->type = jl_float();
        endptr++;
      } else if (*endptr == 'l' || *endptr == 'L') {
        out->constant.ld = out->constant.d;
        out->type = jl_long_double();
        endptr++;
      } else {
        out->type = jl_double();
      }
    } else {
      out->type = jl_double();
    }
  }
  if (errno || ((size_t) (endptr - str) != len)) {
    if (errno == ERANGE) {
      return ERANGE;
    } else {
      return 1;
    }
  }
  return 0;
}

long jl_eval_long(jl_expr_t expr) {
  return 0;
}

unsigned long jl_eval_ulong(jl_expr_t expr) {
  return 0;
}
