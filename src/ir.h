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

#ifndef   JL_IR_H__
# define  JL_IR_H__

#include <u/vector.h>

#include "token.h"

struct jl_entity;
struct jl_expr;
struct jl_stmt;
struct jl_type;

typedef struct jl_type jl_type_t;
typedef struct jl_literal jl_literal_t;
typedef struct jl_pointer jl_pointer_t;
typedef struct jl_array jl_array_t;
typedef struct jl_compound jl_compound_t;

enum jl_type_n {
  JL_TYPE_UNDEFINED = 0,
  JL_TYPE_VOID,
  JL_TYPE_POINTER,
  JL_TYPE_ARRAY,
  JL_TYPE_COMPOUND,
  JL_TYPE_BOOL,
  JL_TYPE_CHAR,
  JL_TYPE_SHORT,
  JL_TYPE_INT,
  JL_TYPE_LONG,
  JL_TYPE_FLOAT,
  JL_TYPE_DOUBLE,
  JL_TYPE_LONG_LONG,
  JL_TYPE_LONG_DOUBLE
};

enum jl_type_specifier_n {
  JL_TYPE_SPECIFIER_NONE = 0,
  JL_TYPE_SPECIFIER_SIGNED = 1 << 0,
  JL_TYPE_SPECIFIER_UNSIGNED = 1 << 1,
  JL_TYPE_SPECIFIER_COMPLEX = 1 << 2,
  JL_TYPE_SPECIFIER_IMAGINARY = 1 << 3,
  JL_TYPE_SPECIFIER_TYPEDEF = 1 << 4,
  JL_TYPE_SPECIFIER_EXTERN = 1 << 5,
  JL_TYPE_SPECIFIER_STATIC = 1 << 6,
  JL_TYPE_SPECIFIER_THREAD_LOCAL = 1 << 7,
  JL_TYPE_SPECIFIER_AUTO = 1 << 8,
  JL_TYPE_SPECIFIER_REGISTER = 1 << 9,
  JL_FUNC_SPECIFIER_INLINE = 1 << 10,
  JL_FUNC_SPECIFIER_NORETURN = 1 << 11
};

enum jl_type_qualifier_n {
  JL_TYPE_QUALIFIER_NONE = 0,
  JL_TYPE_QUALIFIER_CONST = 1 << 0,
  JL_TYPE_QUALIFIER_VOLATILE = 1 << 1,
  JL_TYPE_QUALIFIER_RESTRICT = 1 << 2,
  JL_TYPE_QUALIFIER_ATOMIC = 1 << 3
};

struct jl_literal {
  jl_lloc_t lloc;
  size_t size, align;
  jl_type_t *next;
  enum jl_type_specifier_n specifiers;
  enum jl_type_qualifier_n qualifiers;
};

struct jl_pointer {
  jl_lloc_t lloc;
  size_t size, align;
  jl_type_t *of;
  enum jl_type_specifier_n specifiers;
  enum jl_type_qualifier_n qualifiers;
};

struct jl_array {
  jl_lloc_t lloc;
  size_t size, align;
  jl_type_t *of;
  enum jl_type_specifier_n specifiers;
  enum jl_type_qualifier_n qualifiers;

  struct jl_expr *length;
};

struct jl_compound {
  jl_lloc_t lloc;
  size_t size, align;
  jl_type_t *next;
  enum jl_type_specifier_n specifiers;
  enum jl_type_qualifier_n qualifiers;

  struct jl_entity *entity;
};

struct jl_type {
  union {
    struct {
      jl_lloc_t lloc;
      size_t size, align;
      jl_type_t *next;
      enum jl_type_specifier_n specifiers;
      enum jl_type_qualifier_n qualifiers;
    };
    jl_literal_t literal;
    jl_pointer_t pointer;
    jl_array_t array;
    jl_compound_t compound;
  };
  enum jl_type_n kind;
};

struct jl_field;

typedef struct jl_val jl_val_t;
typedef struct jl_expr jl_expr_t;
typedef struct jl_exprs jl_exprs_t;
typedef struct jl_id jl_id_t;
typedef struct jl_const jl_const_t;
typedef struct jl_unary jl_unary_t;
typedef struct jl_binary jl_binary_t;
typedef struct jl_ternary jl_ternary_t;
typedef struct jl_array_read jl_array_read_t;
typedef struct jl_array_write jl_array_write_t;
typedef struct jl_field_read jl_field_read_t;
typedef struct jl_field_write jl_field_write_t;
typedef struct jl_call jl_call_t;

typedef vec_of(jl_expr_t) jl_expr_r;

enum jl_expr_n {
  JL_EXPR_UNDEFINED = 0,
  JL_EXPR_EXPRS,
  JL_EXPR_ID,
  JL_EXPR_CONST,
  JL_EXPR_UNARY,
  JL_EXPR_BINARY,
  JL_EXPR_TERNARY,
  JL_EXPR_ARRAY_READ,
  JL_EXPR_ARRAY_WRITE,
  JL_EXPR_FIELD_READ,
  JL_EXPR_FIELD_WRITE,
  JL_EXPR_CALL
};

enum jl_op_n {
  JL_OP_CAST,   /* (T) l        */
  JL_OP_EN,     /* (l)          */
  JL_OP_CALL,   /* l()          */
  JL_OP_VA_ARG, /* va_arg(l, T) */
  JL_OP_NOT,    /* !l           */
  JL_OP_ASSIGN, /* l = r        */
  JL_OP_ADD,    /* l + r        */
  JL_OP_SUB,    /* l - r        */
  JL_OP_MUL,    /* l * r        */
  JL_OP_DIV,    /* l / r        */
  JL_OP_MOD,    /* l % r        */
  JL_OP_AND,    /* l & r        */
  JL_OP_LAND,    /* l && r        */
  JL_OP_OR,     /* l | r        */
  JL_OP_LOR,     /* l || r        */
  JL_OP_XOR,    /* l ^ r        */
  JL_OP_SHL,    /* l << r       */
  JL_OP_SHR,    /* l >> r       */
  JL_OP_EQ,     /* l == r       */
  JL_OP_NE,     /* l != r       */
  JL_OP_GE,     /* l >= r       */
  JL_OP_GT,     /* l > r        */
  JL_OP_LE,     /* l <= r       */
  JL_OP_LT,     /* l < r        */
  JL_OP_NEG     /* l ~ r        */
};

struct jl_val {
  jl_type_t type;
  union {
    const char *s;
    float f;
    double d;
    long double ld;
    unsigned long int ul;
  };
};

struct jl_exprs {
  jl_lloc_t lloc;
  jl_type_t type;

  jl_expr_r vector;
};

struct jl_id {
  jl_lloc_t lloc;
  jl_type_t type;

  const char *id;
  bool is_keyword;
};

struct jl_const {
  jl_lloc_t lloc;

  union {
    jl_val_t value;

    /* only accessed, value field must be set. */
    struct {
      jl_type_t type;
      union {
        const char *s;
        float f;
        double d;
        long double ld;
        unsigned long int ul;
      };
    };
  };
};

struct jl_unary {
  jl_lloc_t lloc;
  jl_type_t type;

  enum jl_op_n op;
  jl_expr_t *operand;
};

struct jl_binary {
  jl_lloc_t lloc;
  jl_type_t type;

  enum jl_op_n op;
  jl_expr_t *lhs, *rhs;
};

struct jl_ternary {
  jl_lloc_t lloc;
  jl_type_t type;

  jl_expr_t *lhs, *mhs, *rhs;
};

struct jl_array_read {
  jl_lloc_t lloc;
  jl_type_t type;

  jl_expr_t *lhs, *pos;
};

struct jl_array_write {
  union {
    jl_array_read_t read;

    /* only accessed, array_read field must be set. */
    struct {
      jl_lloc_t lloc;
      jl_type_t type;

      jl_expr_t *lhs, *pos;
    };
  };

  jl_expr_t *rhs;
};

struct jl_field_read {
  jl_lloc_t lloc;
  jl_type_t type;

  jl_expr_t *lhs;
  jl_id_t id;
};

struct jl_field_write {
  union {
    jl_field_read_t read;

    /* only accessed, field_read field must be set. */
    struct {
      jl_lloc_t lloc;
      jl_type_t type;

      jl_expr_t *lhs;
      jl_id_t id;
    };
  };

  jl_expr_t *rhs;
};

struct jl_call {
  jl_lloc_t lloc;
  jl_type_t type;

  jl_expr_t *lhs;
  jl_exprs_t args;
};

struct jl_expr {
  union {
    struct {
      jl_lloc_t lloc;
      jl_type_t type;
    };
    jl_exprs_t exprs;
    jl_id_t id;
    jl_const_t constant;
    jl_unary_t unary;
    jl_binary_t binary;
    jl_ternary_t ternary;
    jl_array_read_t array_read;
    jl_array_write_t array_write;
    jl_field_read_t field_read;
    jl_field_write_t field_write;
    jl_call_t call;
  };
  enum jl_expr_n kind;
};

typedef struct jl_stmt jl_stmt_t;
typedef struct jl_stmt_expr jl_stmt_expr_t;
typedef struct jl_stmt_label jl_stmt_label_t;
typedef struct jl_stmt_case jl_stmt_case_t;
typedef struct jl_stmt_default jl_stmt_default_t;
typedef struct jl_stmt_compound jl_stmt_compound_t;
typedef struct jl_stmt_if jl_stmt_if_t;
typedef struct jl_stmt_switch jl_stmt_switch_t;
typedef struct jl_stmt_while jl_stmt_while_t;
typedef struct jl_stmt_do jl_stmt_do_t;
typedef struct jl_stmt_for jl_stmt_for_t;
typedef struct jl_stmt_goto jl_stmt_goto_t;
typedef struct jl_stmt_break jl_stmt_break_t;
typedef struct jl_stmt_continue jl_stmt_continue_t;
typedef struct jl_stmt_return jl_stmt_return_t;
typedef struct jl_stmt_decl jl_stmt_decl_t;

typedef vec_of(jl_stmt_t) jl_stmt_r;

enum jl_stmt_n {
  JL_STMT_UNDEFINED = 0,
  JL_STMT_EXPR,
  JL_STMT_LABEL,
  JL_STMT_CASE,
  JL_STMT_DEFAULT,
  JL_STMT_COMPOUND,
  JL_STMT_IF,
  JL_STMT_SWITCH,
  JL_STMT_WHILE,
  JL_STMT_DO,
  JL_STMT_FOR,
  JL_STMT_GOTO,
  JL_STMT_BREAK,
  JL_STMT_CONTINUE,
  JL_STMT_RETURN,
  JL_STMT_DECL
};

struct jl_stmt_expr {
  jl_lloc_t lloc;

  jl_expr_t expr;
};

struct jl_stmt_label {
  jl_lloc_t lloc;

  const char *name;
};

struct jl_stmt_case {
  jl_lloc_t lloc;

  jl_expr_t cond;
  jl_stmt_t *body;
};

struct jl_stmt_default {
  jl_lloc_t lloc;

  jl_stmt_t *body;
};

struct jl_stmt_compound {
  jl_lloc_t lloc;

  jl_stmt_r stmts;
};

struct jl_stmt_if {
  jl_lloc_t lloc;

  jl_expr_t cond;
  jl_stmt_t *then_body, *else_body;
};

struct jl_stmt_switch {
  jl_lloc_t lloc;

  jl_expr_t cond;
  jl_stmt_t *body;
};

struct jl_stmt_while {
  jl_lloc_t lloc;

  jl_expr_t cond;
  jl_stmt_t *body;
};

struct jl_stmt_do {
  jl_lloc_t lloc;

  jl_expr_t cond;
  jl_stmt_t *body;
};

struct jl_stmt_for {
  jl_lloc_t lloc;

  jl_stmt_expr_t lhs, mhs;
  jl_expr_t rhs;
  jl_stmt_t *body;
};

struct jl_stmt_goto {
  jl_lloc_t lloc;

  const char *label;
};

struct jl_stmt_break {
  jl_lloc_t lloc;
};

struct jl_stmt_continue {
  jl_lloc_t lloc;
};

struct jl_stmt_return {
  jl_lloc_t lloc;

  jl_expr_t expr;
};

struct jl_stmt_decl {
  jl_lloc_t lloc;

  struct jl_entity *entity;
};

struct jl_stmt {
  union {
    jl_lloc_t lloc;
    jl_stmt_expr_t expr;
    jl_stmt_label_t label;
    jl_stmt_case_t case_stmt;
    jl_stmt_default_t default_stmt;
    jl_stmt_compound_t compound;
    jl_stmt_if_t if_stmt;
    jl_stmt_switch_t switch_stmt;
    jl_stmt_while_t while_stmt;
    jl_stmt_do_t do_stmt;
    jl_stmt_for_t for_stmt;
    jl_stmt_goto_t goto_stmt;
    jl_stmt_break_t break_stmt;
    jl_stmt_continue_t continue_stmt;
    jl_stmt_return_t return_stmt;
    jl_stmt_decl_t decl;
  };
  enum jl_stmt_n kind;
};


typedef struct jl_entity jl_entity_t;
typedef struct jl_field jl_field_t;
typedef struct jl_var jl_var_t;
typedef struct jl_param jl_param_t;
typedef struct jl_func jl_func_t;
typedef struct jl_enum jl_enum_t;
typedef struct jl_struct jl_struct_t;
typedef struct jl_union jl_union_t;
typedef struct jl_label jl_label_t;

typedef vec_of(jl_entity_t) jl_entity_r;

enum jl_entity_n {
  JL_ENTITY_UNDEFINED = 0,
  JL_ENTITY_FIELD,
  JL_ENTITY_VAR,
  JL_ENTITY_PARAM,
  JL_ENTITY_FUNC,
  JL_ENTITY_ENUM,
  JL_ENTITY_STRUCT,
  JL_ENTITY_UNION,
  JL_ENTITY_LABEL
};

struct jl_field {
  jl_lloc_t lloc;
  size_t size, align;
  jl_type_t type;
  const char *name;

  size_t offset;
  unsigned short field_width, field_offset;
};

struct jl_var {
  jl_lloc_t lloc;
  size_t size, align;
  jl_type_t type;
  const char *name;

  jl_expr_t initializer;
};

struct jl_param {
  jl_lloc_t lloc;
  size_t size, align;
  jl_type_t type;
  const char *name;

  unsigned position;
  jl_expr_t initializer;
};

struct jl_func {
  jl_lloc_t lloc;
  size_t size, align;
  jl_type_t return_type;
  const char *name;

  jl_entity_r params;
  jl_stmt_t body;
};

struct jl_enum {
  jl_lloc_t lloc;
  size_t size, align;
  jl_type_t type;
  const char *name;

  jl_entity_r vars;
};

struct jl_struct {
  jl_lloc_t lloc;
  size_t size, align;
  jl_type_t type;
  const char *name;

  jl_entity_r fields;
};

struct jl_union {
  jl_lloc_t lloc;
  size_t size, align;
  jl_type_t type;
  const char *name;

  jl_entity_r fields;
};

struct jl_label {
  jl_lloc_t lloc;
  size_t size, align;
  jl_type_t type;
  const char *name;

  jl_stmt_t next;
};

struct jl_entity {
  union {
    struct {
      jl_lloc_t lloc;
      size_t size, align;
      jl_type_t type;
      const char *name;
    };
    jl_field_t field;
    jl_var_t variable;
    jl_param_t parameter;
    jl_func_t function;
    jl_enum_t enumerable;
    jl_struct_t structure;
    jl_union_t u_structure;
    jl_label_t label;
  };
  enum jl_entity_n kind;
};


typedef struct jl_node jl_node_t;

enum jl_node_n {
  JL_NODE_UNDEFINED = 0,
  JL_NODE_ENTITY,
  JL_NODE_EXPR,
  JL_NODE_STMT,
  JL_NODE_TYPE
};

struct jl_node {
  union {
    jl_lloc_t lloc;
    jl_entity_t entity;
    jl_expr_t expr;
    jl_stmt_t stmt;
    jl_type_t type;
  };
  enum jl_node_n kind;
};

#endif /* JL_IR_H__ */
