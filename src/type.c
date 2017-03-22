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

#include "type.h"

#include "entity.h"
#include "expr.h"
#include "stmt.h"

void jl_literal_dtor(jl_type_t *self);
void jl_pointer_dtor(jl_type_t *self);
void jl_array_dtor(jl_type_t *self);
void jl_compound_dtor(jl_type_t *self);

jl_type_t jl_type_undefined() {
  return (jl_type_t) {JL_TYPE_UNDEFINED};
}

void jl_type_undef(jl_type_t *self) {
  *self = jl_type_undefined();
}

void jl_type_dtor(jl_type_t *self) {
  switch (self->kind) {
    case JL_TYPE_UNDEFINED:
      return;
    case JL_TYPE_LITERAL:
      if (self->u._literal->refs <= 0) {
        jl_literal_dtor(self);
        free(self->u._literal);
        self->u._literal = NULL;
      }
      break;
    case JL_TYPE_POINTER:
      if (self->u._pointer->refs <= 0) {
        jl_pointer_dtor(self);
        free(self->u._pointer);
        self->u._pointer = NULL;
      }
      break;
    case JL_TYPE_ARRAY:
      if (self->u._array->refs <= 0) {
        jl_array_dtor(self);
        free(self->u._array);
        self->u._array = NULL;
      }
      break;
    case JL_TYPE_COMPOUND:
      if (self->u._compound->refs <= 0) {
        jl_compound_dtor(self);
        free(self->u._compound);
        self->u._compound = NULL;
      }
      break;
  }
  *self = jl_type_undefined();
}

void jl_type_switch(jl_type_t *self, enum jl_type_n kind) {
  enum jl_type_specifier_n specifiers;
  enum jl_type_qualifier_n qualifiers;

  if (self->kind != kind || !jl_ptype_is_defined(self)) {
    specifiers = self->specifiers;
    qualifiers = self->qualifiers;
    jl_type_dtor(self);
    *self = (jl_type_t) {
      .kind = kind,
      .specifiers = specifiers,
      .qualifiers = qualifiers
    };
    switch (kind) {
      case JL_TYPE_UNDEFINED:
        break;
      case JL_TYPE_LITERAL:
        self->u._literal = xmalloc(sizeof(jl_literal_t));
        break;
      case JL_TYPE_POINTER:
        self->u._pointer = xmalloc(sizeof(jl_pointer_t));
        break;
      case JL_TYPE_ARRAY:
        self->u._array = xmalloc(sizeof(jl_array_t));
        break;
      case JL_TYPE_COMPOUND:
        self->u._compound = xmalloc(sizeof(jl_compound_t));
        break;
    }
  }
}

void jl_type_acquire(jl_type_t *self) {
  switch (self->kind) {
    case JL_TYPE_UNDEFINED:
      puts("cannot acquire undefined type");
      exit(1);
    case JL_TYPE_LITERAL:
      ++self->u._literal->refs;
      break;
    case JL_TYPE_POINTER:
      ++self->u._pointer->refs;
      break;
    case JL_TYPE_ARRAY:
      ++self->u._array->refs;
      break;
    case JL_TYPE_COMPOUND:
      ++self->u._compound->refs;
      break;
  }
}

void jl_type_release(jl_type_t *self) {
  switch (self->kind) {
    case JL_TYPE_UNDEFINED:
      puts("cannot release undefined type");
      exit(1);
    case JL_TYPE_LITERAL:
      --self->u._literal->refs;
      break;
    case JL_TYPE_POINTER:
      --self->u._pointer->refs;
      break;
    case JL_TYPE_ARRAY:
      --self->u._array->refs;
      break;
    case JL_TYPE_COMPOUND:
      --self->u._compound->refs;
      break;
  }
}

bool jl_type_is_defined(jl_type_t self) {
  switch (self.kind) {
    case JL_TYPE_LITERAL:
      return self.u._literal != NULL;
    case JL_TYPE_POINTER:
      return self.u._pointer != NULL;
    case JL_TYPE_ARRAY:
      return self.u._array != NULL;
    case JL_TYPE_COMPOUND:
      return self.u._compound != NULL;
    case JL_TYPE_UNDEFINED:
    default:
      return false;
  }
}

bool jl_ptype_is_defined(jl_type_t *self) {
  return jl_type_is_defined(*self);
}


  jl_type_t jl_void() {
  static jl_type_t type = {JL_TYPE_UNDEFINED};
  if (!jl_type_is_defined(type)) {
    jl_literal_init(&type, JL_LITERAL_VOID);
  }
  return type;
}

jl_type_t jl_null() {
  static jl_type_t type = {JL_TYPE_UNDEFINED};
  if (!jl_type_is_defined(type)) {
    jl_literal_init(&type, JL_LITERAL_NULL);
  }
  return type;
}

jl_type_t jl_string() {
  static jl_type_t type = {JL_TYPE_UNDEFINED};
  if (!jl_type_is_defined(type)) {
    jl_literal_init(&type, JL_LITERAL_STRING);
  }
  return type;
}

jl_type_t jl_bool() {
  static jl_type_t type = {JL_TYPE_UNDEFINED};
  if (!jl_type_is_defined(type)) {
    jl_literal_init(&type, JL_LITERAL_BOOL);
  }
  return type;
}

jl_type_t jl_char() {
  static jl_type_t type = {JL_TYPE_UNDEFINED};
  if (!jl_type_is_defined(type)) {
    jl_literal_init(&type, JL_LITERAL_CHAR);
  }
  return type;
}

jl_type_t jl_short() {
  static jl_type_t type = {JL_TYPE_UNDEFINED};
  if (!jl_type_is_defined(type)) {
    jl_literal_init(&type, JL_LITERAL_INT);
  }
  return type;
}

jl_type_t jl_int() {
  static jl_type_t type = {JL_TYPE_UNDEFINED};
  if (!jl_type_is_defined(type)) {
    jl_literal_init(&type, JL_LITERAL_INT);
  }
  return type;
}

jl_type_t jl_long() {
  static jl_type_t type = {JL_TYPE_UNDEFINED};
  if (!jl_type_is_defined(type)) {
    jl_literal_init(&type, JL_LITERAL_LONG);
  }
  return type;
}

jl_type_t jl_double() {
  static jl_type_t type = {JL_TYPE_UNDEFINED};
  if (!jl_type_is_defined(type)) {
    jl_literal_init(&type, JL_LITERAL_DOUBLE);
  }
  return type;
}

jl_type_t jl_float() {
  static jl_type_t type = {JL_TYPE_UNDEFINED};
  if (!jl_type_is_defined(type)) {
    jl_literal_init(&type, JL_LITERAL_FLOAT);
  }
  return type;
}

jl_type_t jl_literal(enum jl_literal_n kind) {
  jl_type_t type = {JL_TYPE_UNDEFINED};

  jl_literal_init(&type, kind);
  return type;
}

void jl_literal_init(jl_type_t *self, enum jl_literal_n kind) {
  jl_type_switch(self, JL_TYPE_LITERAL);
  jl_ptype_literal(self)->kind = kind;
}

void jl_literal_dtor(jl_type_t *self) {}


jl_type_t jl_pointer(jl_type_t of) {
  jl_type_t type = {JL_TYPE_UNDEFINED};

  jl_pointer_init(&type, of);
  return type;
}

void jl_pointer_init(jl_type_t *self, jl_type_t of) {
  jl_type_switch(self, JL_TYPE_POINTER);
  jl_ptype_pointer(self)->of = of;
}

void jl_pointer_dtor(jl_type_t *self) {}


jl_type_t jl_array(jl_type_t of) {
  jl_type_t type = {JL_TYPE_UNDEFINED};

  jl_array_init(&type, of, jl_expr_undefined());
  return type;
}

jl_type_t jl_narray(jl_type_t of, jl_expr_t size) {
  jl_type_t type = {JL_TYPE_UNDEFINED};

  jl_array_init(&type, of, size);
  return type;
}

void jl_array_init(jl_type_t *self, jl_type_t of, jl_expr_t size) {
  jl_type_switch(self, JL_TYPE_ARRAY);
  jl_ptype_array(self)->of = of;
  jl_ptype_array(self)->size = size;
}

void jl_array_dtor(jl_type_t *self) {}


jl_type_t jl_compound(jl_entity_t entity) {
  jl_type_t type = {JL_TYPE_UNDEFINED};

  jl_compound_init(&type, entity);
  return type;
}

void jl_compound_init(jl_type_t *self, jl_entity_t entity) {
  jl_type_switch(self, JL_TYPE_COMPOUND);
  jl_ptype_compound(self)->entity = entity;
}

void jl_compound_dtor(jl_type_t *self) {}
