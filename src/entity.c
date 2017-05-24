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
#include <limits.h>
#include <assert.h>

#include <adt/xmalloc.h>

#include "entity.h"

#include "expr.h"
#include "stmt.h"
#include "type.h"
#include "compiler.h"

jl_entity_t jl_field(const char *name, jl_type_t type) {
  return (jl_entity_t) {
    .kind = JL_ENTITY_FIELD,
    .field = {
      .offset = type.align,
      .type = type,
      .size = type.size,
      .align = type.align
    }
  };
}

static void jl_field_dtor(jl_field_t *self) {
  jl_type_dtor(&self->type);
}

jl_entity_t jl_var_int(const char *name, int d) {
  return jl_var(name, jl_type_undefined(), jl_const_int(jl_no_lloc(), d));
}

jl_entity_t jl_var_float(const char *name, float f) {
  return jl_var(name, jl_type_undefined(), jl_const_float(jl_no_lloc(), f));
}

jl_entity_t jl_var_string(const char *name, const char *s) {
  return jl_var(name, jl_type_undefined(), jl_const_string(jl_no_lloc(), s));
}

jl_entity_t jl_var(const char *name, jl_type_t type, jl_expr_t initializer) {
  if (!jl_defined(type)) {
    type = initializer.type;
  } else if (!jl_type_equals(type, initializer.type)) {
    initializer = jl_cast(jl_no_lloc(), type, initializer);
  }
  return (jl_entity_t) {
    .kind = JL_ENTITY_VAR,
    .variable = {
      .initializer = initializer,
      .name = name,
      .type = type,
      .size = type.size,
      .align = type.align
    }
  };
}

static void jl_var_dtor(jl_var_t *self) {
  jl_type_dtor(&self->type);
  jl_expr_dtor(&self->initializer);
}

jl_entity_t jl_param_int(unsigned position, const char *name, int d) {
  return jl_param(position, name, jl_type_undefined(), jl_const_int(jl_no_lloc(), d));
}

jl_entity_t jl_param_float(unsigned position, const char *name, float f) {
  return jl_param(position, name, jl_type_undefined(), jl_const_float(jl_no_lloc(), f));
}

jl_entity_t jl_param_string(unsigned position, const char *name, const char *s) {
  return jl_param(position, name, jl_type_undefined(), jl_const_string(jl_no_lloc(), s));
}

jl_entity_t jl_param(unsigned position, const char *name, jl_type_t type, jl_expr_t initializer) {
  if (!jl_defined(type)) {
    type = initializer.type;
  } else if (!jl_type_equals(type, initializer.type)) {
    initializer = jl_cast(jl_no_lloc(), type, initializer);
  }
  return (jl_entity_t) {
    .kind = JL_ENTITY_PARAM,
    .parameter = {
      .position = position,
      .initializer = initializer,
      .name = name,
      .type = type,
      .size = type.size,
      .align = type.align
    }
  };
}

static void jl_param_dtor(jl_param_t *self) {
  jl_type_dtor(&self->type);
  jl_expr_dtor(&self->initializer);
}

jl_entity_t jl_func_decl(jl_type_t return_type, const char *name, jl_param_t *params) {
  return jl_func(return_type, name, params, jl_stmt_undefined());
}

jl_entity_t jl_proc_decl(const char *name, jl_param_t *params) {
  return jl_func(jl_void(), name, params, jl_stmt_undefined());
}

jl_entity_t jl_func_def(jl_func_t prototype, jl_stmt_t body) {
  size_t i;
  jl_entity_t entity = jl_func(prototype.return_type, prototype.name, NULL, body);
  
  foreach(prototype.params, i) {
    vec_push(entity.function.params, ds_at(prototype.params, i));
  }
  return entity;
}

jl_entity_t jl_func(jl_type_t return_type, const char *name, jl_param_t *params, jl_stmt_t body) {
  unsigned count = 0;
  jl_param_t param;
  jl_entity_t entity = (jl_entity_t) {
    .kind = JL_ENTITY_FUNC,
    .function = {
      .body = body,
      .name = name,
      .return_type = return_type
    }
  };
  if (params) while (params->name) {
    param = *params++;
    vec_push(entity.function.params, jl_param(count++, param.name, param.type, param.initializer));
  }
  return entity;
}

static void jl_func_dtor(jl_func_t *self) {
  size_t i;

  foreach(self->params, i) {
    jl_entity_dtor(ds_data(self->params) + i);
  }
  vec_dtor(self->params);
  jl_type_dtor(&self->return_type);
  jl_stmt_dtor(&self->body);
}

jl_entity_t jl_enum_anonymous(jl_field_t *fields) {
  return jl_enum(NULL, fields);
}

jl_entity_t jl_enum(const char *name, jl_field_t *fields) {
  jl_field_t field;
  jl_entity_t entity = (jl_entity_t) {
    .kind = JL_ENTITY_ENUM,
    .enumerable = {
      .name = name,
      .type = jl_int()
    }
  };
  entity.size = entity.type.size;
  entity.align = entity.type.align;
  if (fields) while (fields->name) {
    field = *fields++;
    vec_push(entity.enumerable.vars, jl_field(field.name, field.type));
  }
  return entity;
}

static void jl_enum_dtor(jl_enum_t *self) {
  size_t i;

  foreach(self->vars, i) {
    jl_entity_dtor(ds_data(self->vars) + i);
  }
  vec_dtor(self->vars);
}

static jl_entity_t *type_add_field(jl_entity_t *self, const char *name, jl_type_t type, unsigned short width);

static void reset_field_alignment(jl_entity_t *self) {
  int d;
  size_t n;
  const jl_entity_t *m;

  assert(jl_pis(self, JL_ENTITY_STRUCT));
  n = ds_size(self->structure.fields);
  if (n) {
    m = &ds_at(self->structure.fields, n - 1);
    if (m->field.field_width) {
      d = m->field.field_offset + m->field.field_width;
      if (d < 32) {
        type_add_field(self, NULL, jl_int(), (unsigned short) (32 - d));
      }
    } else if (self->size % 4 != 0) {
      self->size += self->size % 4;
    }
  }
}

static size_t adjust_member_alignment(jl_entity_t *self, jl_type_t type) {
  size_t align = 0;

  if (jl_pis(self, JL_ENTITY_STRUCT)) {
    align = type.align;
    if (self->size % align) {
      self->size += align - (self->size % align);
      assert(self->size % align == 0);
    }

    align = self->size;
  }
  return align;
}

static jl_entity_t *add_member(jl_entity_t *self, jl_entity_t m) {
  jl_entity_r entities;
  jl_entity_t *member = NULL;

  if (m.name && strcmp(m.name, "...") == 0) {
    // TODO: va_args
    exit(1);
  } else {
    if (m.name && jl_entity_field_lookup(*self, m.name)) {
      jl_fatal_err(NULL, "Member '%s' already exists.", m.name);
    }
    switch (self->kind) {
      case JL_ENTITY_STRUCT:
        entities = self->structure.fields;
        break;
      case JL_ENTITY_UNION:
        entities = self->u_structure.fields;
        break;
      default:
        return NULL;
    }
    vec_push(entities, m);
    member = &ds_back(entities);
    if (m.type.size == 0) {
      jl_fatal_err(NULL, "Member '%s' has incomplete type.", m.name);
    }
    if (LONG_MAX - m.field.offset < m.type.size) {
      jl_fatal_err(NULL, "Object is too large.");
    }
    if (self->size < m.field.offset + m.type.size) {
      self->size = m.field.offset + m.type.size;
    }
  }
  return member;
}

static int pack_field(const jl_entity_t *prev, jl_entity_t *m) {
  unsigned short bits;

  assert(prev);
  bits = prev->field.field_offset + prev->field.field_width;
  if (bits + m->field.field_width <= (unsigned short) (sizeof(int) * 8)) {
    m->field.offset = prev->field.offset;
    m->field.field_offset = bits;
    return 1;
  }
  return 0;
}

static size_t remove_anonymous_fields(jl_entity_t *self) {
  int i;
  size_t align, maxalign;
  jl_field_t *m;
  jl_entity_r entities;

  switch (self->kind) {
    case JL_ENTITY_STRUCT:
      entities = self->structure.fields;
      break;
    case JL_ENTITY_UNION:
      entities = self->u_structure.fields;
      break;
    default:
      return 0;
  }

  maxalign = 0;
  for (i = (int) (ds_size(entities) - 1); i >= 0; --i) {
    m = &ds_at(entities, i).field;
    if (!m->name) {
      jl_entity_dtor(&ds_at(entities, i));
      vec_erase(entities, i);
    } else {
      align = m->align;
      if (align > maxalign) {
        maxalign = align;
      }
    }
  }

  return maxalign;
}

static jl_entity_t *type_add_field(jl_entity_t *self, const char *name, jl_type_t type, unsigned short width) {
  jl_entity_t m;
  const jl_entity_t *prev;

  m = jl_field(name, type);
  m.field.field_width = width;
  if (jl_pis(self, JL_ENTITY_STRUCT)) {
    prev = &ds_back(self->structure.fields);
    if (!prev || !pack_field(prev, &m)) {
      m.field.field_offset = 0;
      m.field.offset = adjust_member_alignment(self, type);
    }
  }
  return add_member(self, m);
}

static jl_entity_t *type_add_anonymous_member(jl_entity_t *self, jl_type_t type) {
  unsigned i;
  size_t offset;
  jl_entity_t m;
  jl_entity_t *member;
  jl_entity_r entities;

  if (!jl_is(type, JL_TYPE_COMPOUND)) {
    // TODO: handle error
    exit(1);
  }
  member = type.compound.entity;
  switch (member->kind) {
    case JL_ENTITY_STRUCT:
      entities = member->structure.fields;
      break;
    case JL_ENTITY_UNION:
      entities = member->u_structure.fields;
      break;
    default:
      return NULL;
  }
  if (jl_pis(self, JL_ENTITY_STRUCT) && jl_pis(member, JL_ENTITY_UNION)) {
    offset = adjust_member_alignment(self, type);
    for (i = 0; i < ds_size(entities); ++i) {
      m = ds_at(entities, i);
      m.field.offset += offset;
      add_member(self, m);
    }
  } else if (jl_pis(self, JL_ENTITY_UNION) && jl_pis(member, JL_ENTITY_STRUCT)) {
    for (i = 0; i < ds_size(entities); ++i) {
      m = ds_at(entities, i);
      add_member(self, m);
    }
  } else {
    for (i = 0; i < ds_size(entities); ++i) {
      m = ds_at(entities, i);
      jl_entity_add_field(self, m.name, m.type, 0);
    }
  }
  return NULL;
}

void type_seal(jl_entity_t *self) {
  size_t align;

  align = remove_anonymous_fields(self);
  if (align == 0) {
    // TODO: handle error
    exit(1);
  }
  if (self->size % align) {
    self->size += align - (self->size % align);
  }
}

jl_entity_t jl_struct(const char *name, jl_field_t *fields) {
  jl_entity_t entity = (jl_entity_t) {
    .kind = JL_ENTITY_STRUCT,
    .structure = {
      .name = name,
      .size = 0
    }
  };

  if (fields) {
    while (fields->name) {
      jl_entity_add_field(&entity, fields->name, fields->type, fields->field_width);
      ++fields;
    }
    type_seal(&entity);
  }
  return entity;
}

jl_entity_t jl_struct_anonymous(jl_field_t *fields) {
  return jl_struct(NULL, fields);
}

void jl_struct_dtor(jl_struct_t *self) {
  size_t i;

  foreach(self->fields, i) {
    jl_entity_dtor(ds_data(self->fields) + i);
  }
  vec_dtor(self->fields);
}

jl_entity_t jl_union(const char *name, jl_field_t *fields) {
  jl_entity_t entity = (jl_entity_t) {
    .kind = JL_ENTITY_UNION,
    .u_structure = {
      .name = name,
      .size = 0
    }
  };

  if (fields) {
    while (fields->name) {
      jl_entity_add_field(&entity, fields->name, fields->type, fields->field_width);
      ++fields;
    }
    type_seal(&entity);
  }
  return entity;
}

jl_entity_t jl_union_anonymous(jl_field_t *fields) {
  return jl_union(NULL, fields);
}

static void jl_union_dtor(jl_union_t *self) {
  size_t i;

  foreach(self->fields, i) {
    jl_entity_dtor(ds_data(self->fields) + i);
  }
  vec_dtor(self->fields);
}

void jl_label_dtor(jl_label_t *self) {}

void jl_entity_dtor(jl_entity_t *self) {
  switch (self->kind) {
    case JL_ENTITY_UNDEFINED:
      return;
    case JL_ENTITY_FIELD:
      jl_field_dtor(&self->field);
      break;
    case JL_ENTITY_VAR:
      jl_var_dtor(&self->variable);
      break;
    case JL_ENTITY_PARAM:
      jl_param_dtor(&self->parameter);
      break;
    case JL_ENTITY_FUNC:
      jl_func_dtor(&self->function);
      break;
    case JL_ENTITY_ENUM:
      jl_enum_dtor(&self->enumerable);
      break;
    case JL_ENTITY_STRUCT:
      jl_struct_dtor(&self->structure);
      break;
    case JL_ENTITY_UNION:
      jl_union_dtor(&self->u_structure);
      break;
    case JL_ENTITY_LABEL:
      jl_label_dtor(&self->label);
      break;
    default:
      break;
  }
  jl_entity_undef(self);
}

bool jl_entity_equals(jl_entity_t a, jl_entity_t b) {
  unsigned i;
  jl_entity_r a_fields, b_fields;

  if (a.kind != b.kind) {
    return false;
  }
  if (!jl_type_specified(a.type, UNSIGNED)) {
    return jl_type_equals(a.type, b.type);
  }
  a_fields = jl_entity_fields(a);
  b_fields = jl_entity_fields(b);
  if (ds_size(a_fields) != ds_size(b_fields)) {
    return false;
  }
  for (i = 0; i < ds_size(a_fields); ++i) {
    if (!jl_entity_equals(ds_at(a_fields, i), ds_at(b_fields, i))) {
      return false;
    }
  }
  return true;
}

jl_entity_r jl_entity_fields(jl_entity_t self) {
  switch (self.kind) {
    case JL_ENTITY_FUNC:
      return self.function.params;
    case JL_ENTITY_ENUM:
      return self.enumerable.vars;
    case JL_ENTITY_STRUCT:
      return self.structure.fields;
    case JL_ENTITY_UNION:
      return self.u_structure.fields;
    default:
      return (jl_entity_r) {0};
  }
}

jl_field_t *jl_entity_field_lookup(jl_entity_t self, const char *name) {
  jl_entity_r fields;
  jl_entity_t entity;
  size_t i;

  switch (self.kind) {
    case JL_ENTITY_STRUCT:
      fields = self.structure.fields;
      foreach(fields, i) {
        if (strcmp(name, ds_at(fields, i).name) == 0) {
          return &ds_at(fields, i).field;
        }
      }
      break;
    case JL_ENTITY_UNION:
      fields = self.u_structure.fields;
      foreach(fields, i) {
        if (strcmp(name, ds_at(fields, i).name) == 0) {
          return &ds_at(fields, i).field;
        }
      }
      break;
    default:
      break;
      return NULL;
  }
  return NULL;
}

jl_entity_t *jl_entity_add_field(jl_entity_t *self, const char *name, jl_type_t type, short width) {
  jl_entity_t m;

  if (width) {
    return type_add_field(self, name, type, (unsigned short) width);
  }
  if (!name) {
    return type_add_anonymous_member(self, type);
  }
  m = jl_field(name, type);
  m.field.offset = adjust_member_alignment(self, type);
  m.name = name;
  m.type = type;
  return add_member(self, m);
}
