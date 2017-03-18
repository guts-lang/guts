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

#ifndef   JL_TYPE_T_H__
# define  JL_TYPE_T_H__

#include <assert.h>

typedef enum jl_type_n jl_type_n;
typedef enum jl_literal_n jl_literal_n;
typedef enum jl_type_specifier_n jl_type_specifier_n;
typedef enum jl_type_qualifier_n jl_type_qualifier_n;

typedef struct jl_type_t jl_type_t;
typedef struct jl_literal_t jl_literal_t;
typedef struct jl_pointer_t jl_pointer_t;
typedef struct jl_array_t jl_array_t;
typedef struct jl_compound_t jl_compound_t;

enum jl_type_n {
  JL_TYPE_UNDEFINED = 0,
  JL_TYPE_LITERAL,
  JL_TYPE_POINTER,
  JL_TYPE_ARRAY,
  JL_TYPE_COMPOUND
};

enum jl_literal_n {
  JL_LITERAL_VOID = 0,
  JL_LITERAL_NULL,
  JL_LITERAL_STRING,
  JL_LITERAL_BOOL,
  JL_LITERAL_CHAR,
  JL_LITERAL_SHORT,
  JL_LITERAL_INT,
  JL_LITERAL_LONG,
  JL_LITERAL_FLOAT,
  JL_LITERAL_DOUBLE
};

enum jl_type_specifier_n {
  JL_TYPE_SPECIFIER_NONE = 0,
  JL_TYPE_SPECIFIER_SIGNED = 1 << 0,
  JL_TYPE_SPECIFIER_UNSIGNED = 1 << 1,
  JL_TYPE_SPECIFIER_BOOL = 1 << 2,
  JL_TYPE_SPECIFIER_CHAR = 1 << 3,
  JL_TYPE_SPECIFIER_SHORT = 1 << 4,
  JL_TYPE_SPECIFIER_INT = 1 << 5,
  JL_TYPE_SPECIFIER_LONG = 1 << 6,
  JL_TYPE_SPECIFIER_FLOAT = 1 << 7,
  JL_TYPE_SPECIFIER_DOUBLE = 1 << 8,
  JL_TYPE_SPECIFIER_EXTERN = 1 << 9,
  JL_TYPE_SPECIFIER_STATIC = 1 << 10,
  JL_TYPE_SPECIFIER_THREAD_LOCAL = 1 << 11,
  JL_TYPE_SPECIFIER_AUTO = 1 << 12,
  JL_TYPE_SPECIFIER_REGISTER = 1 << 13
};

enum jl_type_qualifier_n {
  JL_TYPE_QUALIFIER_NONE = 0,
  JL_TYPE_QUALIFIER_CONST = 1 << 0,
  JL_TYPE_QUALIFIER_VOLATILE = 1 << 1,
  JL_TYPE_QUALIFIER_RESTRICT = 1 << 2,
  JL_TYPE_QUALIFIER_ATOMIC = 1 << 3
};

struct jl_type_t {
  jl_type_n kind : 8;
  jl_type_specifier_n specifiers;
  jl_type_qualifier_n qualifiers;
  union {
    jl_literal_t *_literal;
    jl_pointer_t *_pointer;
    jl_array_t *_array;
    jl_compound_t *_compound;
  };
};

jl_type_t jl_type_undefined();
void jl_type_dtor(jl_type_t *self);
void jl_type_switch(jl_type_t *self, jl_type_n kind);
void jl_type_acquire(jl_type_t *self);
void jl_type_release(jl_type_t *self);
bool jl_type_is_defined(jl_type_t self);
bool jl_ptype_is_defined(jl_type_t *self);

#define jl_type_is_literal(t) ((t).kind == JL_TYPE_LITERAL)
#define jl_type_is_pointer(t) ((t).kind == JL_TYPE_POINTER)
#define jl_type_is_array(t) ((t).kind == JL_TYPE_ARRAY)
#define jl_type_is_compound(t) ((t).kind == JL_TYPE_COMPOUND)
#define jl_ptype_is_literal(t) ((t)->kind == JL_TYPE_LITERAL)
#define jl_ptype_is_pointer(t) ((t)->kind == JL_TYPE_POINTER)
#define jl_ptype_is_array(t) ((t)->kind == JL_TYPE_ARRAY)
#define jl_ptype_is_compound(t) ((t)->kind == JL_TYPE_COMPOUND)

#define jl_type_is_specified(t) ((t).specifiers != 0)
#define jl_type_is_signed(t) ((t).specifiers & JL_TYPE_SPECIFIER_SIGNED)
#define jl_type_is_unsigned(t) ((t).specifiers & JL_TYPE_SPECIFIER_UNSIGNED)
#define jl_type_is_extern(t) ((t).specifiers & JL_TYPE_SPECIFIER_EXTERN)
#define jl_type_is_static(t) ((t).specifiers & JL_TYPE_SPECIFIER_STATIC)
#define jl_type_is_thread_local(t) ((t).specifiers & JL_TYPE_SPECIFIER_THREAD_LOCAL)
#define jl_type_is_auto(t) ((t).specifiers & JL_TYPE_SPECIFIER_AUTO)
#define jl_type_is_register(t) ((t).specifiers & JL_TYPE_SPECIFIER_REGISTER)
#define jl_ptype_is_specified(t) ((t)->specifiers != 0)
#define jl_ptype_is_signed(t) ((t)->specifiers & JL_TYPE_SPECIFIER_SIGNED)
#define jl_ptype_is_unsigned(t) ((t)->specifiers & JL_TYPE_SPECIFIER_UNSIGNED)
#define jl_ptype_is_extern(t) ((t)->specifiers & JL_TYPE_SPECIFIER_EXTERN)
#define jl_ptype_is_static(t) ((t)->specifiers & JL_TYPE_SPECIFIER_STATIC)
#define jl_ptype_is_thread_local(t) ((t)->specifiers & JL_TYPE_SPECIFIER_THREAD_LOCAL)
#define jl_ptype_is_auto(t) ((t)->specifiers & JL_TYPE_SPECIFIER_AUTO)
#define jl_ptype_is_register(t) ((t)->specifiers & JL_TYPE_SPECIFIER_REGISTER)

#define jl_type_is_qualified(t) ((t).qualifiers != 0)
#define jl_type_is_const(t) ((t).qualifiers & JL_TYPE_QUALIFIER_CONST)
#define jl_type_is_volatile(t) ((t).qualifiers & JL_TYPE_QUALIFIER_VOLATILE)
#define jl_type_is_restrict(t) ((t).qualifiers & JL_TYPE_QUALIFIER_RESTRICT)
#define jl_type_is_atomic(t) ((t).qualifiers & JL_TYPE_QUALIFIER_ATOMIC)
#define jl_ptype_is_qualified(t) ((t)->qualifiers != 0)
#define jl_ptype_is_const(t) ((t)->qualifiers & JL_TYPE_QUALIFIER_CONST)
#define jl_ptype_is_volatile(t) ((t)->qualifiers & JL_TYPE_QUALIFIER_VOLATILE)
#define jl_ptype_is_restrict(t) ((t)->qualifiers & JL_TYPE_QUALIFIER_RESTRICT)
#define jl_ptype_is_atomic(t) ((t)->qualifiers & JL_TYPE_QUALIFIER_ATOMIC)

#define jl_type_literal(t) ((void) assert(jl_type_is_literal(t)), (t)._literal)
#define jl_type_pointer(t) ((void) assert(jl_type_is_pointer(t)), (t)._pointer)
#define jl_type_array(t) ((void) assert(jl_type_is_array(t)), (t)._array)
#define jl_type_compound(t) ((void) assert(jl_type_is_compound(t)), (t)._compound)
#define jl_ptype_literal(t) ((void) assert(jl_ptype_is_literal(t)), (t)->_literal)
#define jl_ptype_pointer(t) ((void) assert(jl_ptype_is_pointer(t)), (t)->_pointer)
#define jl_ptype_array(t) ((void) assert(jl_ptype_is_array(t)), (t)->_array)
#define jl_ptype_compound(t) ((void) assert(jl_ptype_is_compound(t)), (t)->_compound)

#endif /* JL_TYPE_T_H__ */