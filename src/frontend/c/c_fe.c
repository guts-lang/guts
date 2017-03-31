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

#include <stdio.h>

#include "c_fe.h"

#include "c_lexer.h"
#include "program.h"
#include "entity.h"
#include "expr.h"
#include "stmt.h"
#include "type.h"

static jl_expr_t primary_expression(jl_fe_t *self, jl_lexer_t *lexer, jl_program_t *out);
static jl_expr_t postfix_expression(jl_fe_t *self, jl_lexer_t *lexer, jl_program_t *out);
static jl_expr_t unary_expression(jl_fe_t *self, jl_lexer_t *lexer, jl_program_t *out);
static jl_expr_t cast_expression(jl_fe_t *self, jl_lexer_t *lexer, jl_program_t *out);
static jl_expr_t multiplicative_expression(jl_fe_t *self, jl_lexer_t *lexer, jl_program_t *out);
static jl_expr_t additive_expression(jl_fe_t *self, jl_lexer_t *lexer, jl_program_t *out);
static jl_expr_t shift_expression(jl_fe_t *self, jl_lexer_t *lexer, jl_program_t *out);
static jl_expr_t relational_expression(jl_fe_t *self, jl_lexer_t *lexer, jl_program_t *out);
static jl_expr_t equality_expression(jl_fe_t *self, jl_lexer_t *lexer, jl_program_t *out);
static jl_expr_t and_expression(jl_fe_t *self, jl_lexer_t *lexer, jl_program_t *out);
static jl_expr_t exclusive_or_expression(jl_fe_t *self, jl_lexer_t *lexer, jl_program_t *out);
static jl_expr_t inclusive_or_expression(jl_fe_t *self, jl_lexer_t *lexer, jl_program_t *out);
static jl_expr_t logical_and_expression(jl_fe_t *self, jl_lexer_t *lexer, jl_program_t *out);
static jl_expr_t logical_or_expression(jl_fe_t *self, jl_lexer_t *lexer, jl_program_t *out);
static jl_expr_t conditional_expression(jl_fe_t *self, jl_lexer_t *lexer, jl_program_t *out);
static jl_expr_t assignment_expression(jl_fe_t *self, jl_lexer_t *lexer, jl_program_t *out);
static jl_expr_t expression(jl_fe_t *self, jl_lexer_t *lexer, jl_program_t *out);
static jl_expr_t constant_expression(jl_fe_t *self, jl_lexer_t *lexer, jl_program_t *out);
static jl_entity_t declaration(jl_fe_t *self, jl_lexer_t *lexer, jl_program_t *out);
static jl_type_t declaration_specifiers(jl_fe_t *self, jl_lexer_t *lexer, jl_program_t *out);
static jl_entity_t init_declarator_list(jl_fe_t *self, jl_lexer_t *lexer, jl_program_t *out);
static jl_entity_t init_declarator(jl_fe_t *self, jl_lexer_t *lexer, jl_program_t *out);
static jl_type_t type_specifier(jl_fe_t *self, jl_lexer_t *lexer, jl_program_t *out);
static jl_entity_t struct_or_union_specifier(jl_fe_t *self, jl_lexer_t *lexer, jl_program_t *out);
static jl_entity_t struct_or_union(jl_fe_t *self, jl_lexer_t *lexer, jl_program_t *out);
static jl_entity_t struct_declaration_list(jl_fe_t *self, jl_lexer_t *lexer, jl_program_t *out);
static jl_entity_t struct_declaration(jl_fe_t *self, jl_lexer_t *lexer, jl_program_t *out);
static jl_entity_t specifier_qualifier_list(jl_fe_t *self, jl_lexer_t *lexer, jl_program_t *out);
static jl_entity_t struct_declarator_list(jl_fe_t *self, jl_lexer_t *lexer, jl_program_t *out);
static jl_entity_t struct_declarator(jl_fe_t *self, jl_lexer_t *lexer, jl_program_t *out);
static jl_entity_t enum_specifier(jl_fe_t *self, jl_lexer_t *lexer, jl_program_t *out);
static jl_entity_t enumerator_list(jl_fe_t *self, jl_lexer_t *lexer, jl_program_t *out);
static jl_entity_t enumerator(jl_fe_t *self, jl_lexer_t *lexer, jl_program_t *out);
static jl_entity_t atomic_type_specifier(jl_fe_t *self, jl_lexer_t *lexer, jl_program_t *out);
static jl_entity_t type_qualifier(jl_fe_t *self, jl_lexer_t *lexer, jl_program_t *out);
static jl_entity_t function_specifier(jl_fe_t *self, jl_lexer_t *lexer, jl_program_t *out);
static jl_entity_t alignment_specifier(jl_fe_t *self, jl_lexer_t *lexer, jl_program_t *out);
static jl_entity_t declarator(jl_fe_t *self, jl_lexer_t *lexer, jl_program_t *out);
static jl_entity_t direct_declarator(jl_fe_t *self, jl_lexer_t *lexer, jl_program_t *out);
static jl_entity_t pointer(jl_fe_t *self, jl_lexer_t *lexer, jl_program_t *out);
static jl_entity_t type_qualifier_list(jl_fe_t *self, jl_lexer_t *lexer, jl_program_t *out);
static jl_entity_t parameter_type_list(jl_fe_t *self, jl_lexer_t *lexer, jl_program_t *out);
static jl_entity_t parameter_list(jl_fe_t *self, jl_lexer_t *lexer, jl_program_t *out);
static jl_entity_t parameter_declaration(jl_fe_t *self, jl_lexer_t *lexer, jl_program_t *out);
static jl_entity_t identifier_list(jl_fe_t *self, jl_lexer_t *lexer, jl_program_t *out);
static jl_type_t type_name(jl_fe_t *self, jl_lexer_t *lexer, jl_program_t *out);
static jl_entity_t abstract_declarator(jl_fe_t *self, jl_lexer_t *lexer, jl_program_t *out);
static jl_entity_t direct_abstract_declarator(jl_fe_t *self, jl_lexer_t *lexer, jl_program_t *out);
static jl_entity_t initializer(jl_fe_t *self, jl_lexer_t *lexer, jl_program_t *out);
static jl_entity_t initializer_list(jl_fe_t *self, jl_lexer_t *lexer, jl_program_t *out);
static jl_entity_t designation(jl_fe_t *self, jl_lexer_t *lexer, jl_program_t *out);
static jl_entity_t designator_list(jl_fe_t *self, jl_lexer_t *lexer, jl_program_t *out);
static jl_entity_t designator(jl_fe_t *self, jl_lexer_t *lexer, jl_program_t *out);
static jl_entity_t static_assert_declaration(jl_fe_t *self, jl_lexer_t *lexer, jl_program_t *out);
static jl_entity_t statement(jl_fe_t *self, jl_lexer_t *lexer, jl_program_t *out);
static jl_entity_t labeled_statement(jl_fe_t *self, jl_lexer_t *lexer, jl_program_t *out);
static jl_entity_t compound_statement(jl_fe_t *self, jl_lexer_t *lexer, jl_program_t *out);
static jl_entity_t block_item_list(jl_fe_t *self, jl_lexer_t *lexer, jl_program_t *out);
static jl_entity_t block_item(jl_fe_t *self, jl_lexer_t *lexer, jl_program_t *out);
static jl_entity_t expression_statement(jl_fe_t *self, jl_lexer_t *lexer, jl_program_t *out);
static jl_entity_t selection_statement(jl_fe_t *self, jl_lexer_t *lexer, jl_program_t *out);
static jl_entity_t iteration_statement(jl_fe_t *self, jl_lexer_t *lexer, jl_program_t *out);
static jl_entity_t jump_statement(jl_fe_t *self, jl_lexer_t *lexer, jl_program_t *out);
static jl_entity_t translation_unit(jl_fe_t *self, jl_lexer_t *lexer, jl_program_t *out);
static jl_entity_t external_declaration(jl_fe_t *self, jl_lexer_t *lexer, jl_program_t *out);
static jl_entity_t function_definition(jl_fe_t *self, jl_lexer_t *lexer, jl_program_t *out);
static jl_entity_t declaration_list(jl_fe_t *self, jl_lexer_t *lexer, jl_program_t *out);

void c_fe_parse(jl_fe_t *self, jl_lexer_t *lexer, jl_program_t *out) {
  jl_program_init(out);

  jl_fe_scope(self);
  self->scope->sym = jl_sym_put(self->scope, "foo");
  jl_fe_unscope(self);

  jl_expr_t expr = expression(self, lexer, out);
  jl_expr_dtor(&expr);

  while (jl_lexer_next(lexer).type != 0);
}

enum {
  C_TOKEN_FLAG_NONE = 0,
  C_TOKEN_FLAG_ENUMERATION_CONSTANT = 1 << 0,
};

static jl_expr_t primary_expression(jl_fe_t *self, jl_lexer_t *lexer, jl_program_t *out) {
  jl_sym_t *symbol;
  jl_token_t token;
  jl_expr_t r1;

  switch ((token = jl_lexer_peek(lexer)).type) {
    case C_TOK_IDENTIFIER:
      jl_lexer_next(lexer);
      symbol = jl_sym_get(self->scope, token.value);
      if (symbol->flags & C_TOKEN_FLAG_ENUMERATION_CONSTANT) {
        return jl_u(symbol->entity, var)->initializer;
      }
      return jl_id(symbol->id, jl_entity_type(symbol->entity));
    case C_TOK_NUMBER:
      jl_lexer_next(lexer);
      return jl_const_parse(token.value, token.length);
    case C_TOK_STRING:
      jl_lexer_next(lexer);
      return jl_const_string(token.value);
    case C_TOK_FUNC_NAME:
      if (!(symbol = self->scope->sym) || !jl_entity_is_func(symbol->entity)) {
        fprintf(stderr, "access of __func__ outside of function");
        exit(1);
      }
      jl_lexer_next(lexer);
      return jl_const_string(jl_u(symbol->entity, func)->name);
    case '(':
      jl_lexer_next(lexer);
      if (!jl_defined(r1 = expression(self, lexer, out))) {
        puts("expected expression");
        exit(1);
      }
      jl_lexer_consume(lexer, ')');
      return jl_unary(JL_OP_EN, r1);
    default:
      printf("Unexpected '%s', not a valid primary expression.", token.value ? token.value : token.name);
      exit(1);;
  }
}

static jl_expr_t postfix_expression(jl_fe_t *self, jl_lexer_t *lexer, jl_program_t *out) {
  jl_token_t token;
  jl_expr_t r1, r2;

  r1 = primary_expression(self, lexer, out);
  while (true) {
    switch ((token = jl_lexer_peek(lexer)).type) {
      case '[':
        if (!jl_type_is_ref(r1.type)) {
          puts("array access on non array element");
          exit(1);
        }
        jl_lexer_next(lexer);
        r1 = jl_array_read(r1, expression(self, lexer, out));
      case '(': {
        unsigned i;
        jl_entity_t param;
        jl_func_t *func;
        jl_expr_r args = (jl_expr_r) {0};
        jl_type_t type = jl_expr_get_type(r1);

        if (jl_type_is_pointer(type) && jl_type_is_func(jl_type_pointer(type)->of)) {
          type = jl_type_pointer(type)->of;
        } else if (!jl_type_is_func(type)) {
          fprintf(stderr, "Expression must have type pointer to function");
          exit(1);
        }
        func = jl_u(jl_u(type, compound)->entity, func);
        jl_lexer_next(lexer);
        for (i = 0; i < adt_vector_size(func->params); ++i) {
          if (jl_lexer_peek(lexer).type == ')') {
            fprintf(stderr, "Too few arguments, expected %zu but got %d.", adt_vector_size(func->params), i);
            exit(1);
          }
          param = adt_vector_at(func->params, i);
          r2 = assignment_expression(self, lexer, out);
          if (!jl_type_equals(jl_expr_get_type(r2), jl_entity_type(param))) {
            r2 = jl_cast(jl_entity_type(param), r2);
          }
          adt_vector_push(args, r2);
          if (i < adt_vector_size(func->params) - 1) {
            jl_lexer_consume(lexer, ',');
          }
        }
        jl_lexer_consume(lexer, ')');
        r1 = jl_call(r1, args);
        break;
      }
      case '.': {
        jl_field_t *field;

        jl_lexer_next(lexer);
        token = jl_lexer_consume(lexer, C_TOK_IDENTIFIER);
        field = jl_field_lookup(jl_expr_get_type(r1), token.value);
        if (!field) {
          printf("Invalid access, no member named '%s'.", token.value);
          exit(1);
        }
        r1 = jl_field_read(r1, jl_id(field->name, field->type), field->field_width, field->field_offset);
        break;
      }
      case C_TOK_PTR_OP: {
        jl_type_t type;
        jl_field_t *field;

        jl_lexer_next(lexer);
        if (!jl_type_is_pointer(type = jl_expr_get_type(r1))) {
          printf("Invalid access on non pointer element '%s'.", token.value);
          exit(1);
        }
        token = jl_lexer_consume(lexer, C_TOK_IDENTIFIER);
        field = jl_field_lookup(jl_type_deref(type), token.value);
        if (!field) {
          printf("Invalid access, no member named '%s'.", token.value);
          exit(1);
        }
        r1 = jl_field_read(r1, jl_id(field->name, field->type), field->field_width, field->field_offset);
        break;
      }
      default:
        return r1;
    }
  }
}

static jl_expr_t unary_expression(jl_fe_t *self, jl_lexer_t *lexer, jl_program_t *out) {
  jl_expr_t r1;
  enum jl_op_n op;

  switch (jl_lexer_peek(lexer).type) {
    case '&':
      op = JL_OP_AND;
      break;
    case '*':
      op = JL_OP_AND;
      break;
    case '-':
      op = JL_OP_SUB;
      break;
    case '~':
      op = JL_OP_NEG;
      break;
    case '!':
      op = JL_OP_NOT;
      break;
    case C_TOK_INCREMENT:
      op = JL_OP_ADD;
      goto self_assign;
    case C_TOK_DECREMENT:
      op = JL_OP_SUB;
      goto self_assign;
    default:
      return postfix_expression(self, lexer, out);
    self_assign:
      jl_lexer_next(lexer);
      r1 = unary_expression(self, lexer, out);
      return jl_unary(JL_OP_EN, jl_binary(JL_OP_ASSIGN, r1, jl_binary(op, r1, jl_const_int(1))));
  }
  jl_lexer_next(lexer);
  return jl_unary(op, cast_expression(self, lexer, out));
}

static jl_expr_t cast_expression(jl_fe_t *self, jl_lexer_t *lexer, jl_program_t *out) {
  jl_type_t type;
  jl_token_t token;

  if ((token = jl_lexer_peek(lexer)).type == '(') {
    jl_lexer_next(lexer);
    if (!jl_defined(type = type_name(self, lexer, out))) {
      jl_lexer_undo(lexer, token);
      return unary_expression(self, lexer, out);
    }
    jl_lexer_consume(lexer, ')');
    return jl_cast(type, cast_expression(self, lexer, out));
  }

  return unary_expression(self, lexer, out);
}

static jl_expr_t multiplicative_expression(jl_fe_t *self, jl_lexer_t *lexer, jl_program_t *out) {
  jl_expr_t r1;

  r1 = cast_expression(self, lexer, out);
  while (true) {
    switch (jl_lexer_peek(lexer).type) {
      case '*':
        jl_lexer_next(lexer);
        r1 = jl_binary(JL_OP_MUL, r1, cast_expression(self, lexer, out));
        break;
      case '/':
        jl_lexer_next(lexer);
        r1 = jl_binary(JL_OP_DIV, r1, cast_expression(self, lexer, out));
        break;
      case '%':
        jl_lexer_next(lexer);
        r1 = jl_binary(JL_OP_MOD, r1, cast_expression(self, lexer, out));
        break;
      default:
        return r1;
    }
  }
}

static jl_expr_t additive_expression(jl_fe_t *self, jl_lexer_t *lexer, jl_program_t *out) {
  jl_expr_t r1;

  r1 = multiplicative_expression(self, lexer, out);
  while (true) {
    switch (jl_lexer_peek(lexer).type) {
      case '+':
        jl_lexer_next(lexer);
        r1 = jl_binary(JL_OP_ADD, r1, multiplicative_expression(self, lexer, out));
        break;
      case '-':
        jl_lexer_next(lexer);
        r1 = jl_binary(JL_OP_SUB, r1, multiplicative_expression(self, lexer, out));
        break;
      default:
        return r1;
    }
  }
}

static jl_expr_t shift_expression(jl_fe_t *self, jl_lexer_t *lexer, jl_program_t *out) {
  jl_expr_t r1;

  r1 = additive_expression(self, lexer, out);
  while (true) {
    switch (jl_lexer_peek(lexer).type) {
      case C_TOK_LEFT_OP:
        jl_lexer_next(lexer);
        r1 = jl_binary(JL_OP_SHL, r1, additive_expression(self, lexer, out));
        break;
      case C_TOK_RIGHT_OP:
        jl_lexer_next(lexer);
        r1 = jl_binary(JL_OP_SHR, r1, additive_expression(self, lexer, out));
        break;
      default:
        return r1;
    }
  }
}

static jl_expr_t relational_expression(jl_fe_t *self, jl_lexer_t *lexer, jl_program_t *out) {
  jl_expr_t r1;

  r1 = shift_expression(self, lexer, out);
  while (true) {
    switch (jl_lexer_peek(lexer).type) {
      case '<':
        jl_lexer_next(lexer);
        r1 = jl_binary(JL_OP_LT, r1, shift_expression(self, lexer, out));
        break;
      case C_TOK_LE_OP:
        jl_lexer_next(lexer);
        r1 = jl_binary(JL_OP_LE, r1, shift_expression(self, lexer, out));
        break;
      case '>':
        jl_lexer_next(lexer);
        r1 = jl_binary(JL_OP_GT, r1, shift_expression(self, lexer, out));
        break;
      case C_TOK_GE_OP:
        jl_lexer_next(lexer);
        r1 = jl_binary(JL_OP_GE, r1, shift_expression(self, lexer, out));
        break;
      default:
        return r1;
    }
  }
}

static jl_expr_t equality_expression(jl_fe_t *self, jl_lexer_t *lexer, jl_program_t *out) {
  jl_expr_t r1;

  r1 = relational_expression(self, lexer, out);
  while (true) {
    switch (jl_lexer_peek(lexer).type) {
      case C_TOK_EQ_OP:
        jl_lexer_next(lexer);
        r1 = jl_binary(JL_OP_EQ, r1, relational_expression(self, lexer, out));
        break;
      case C_TOK_NE_OP:
        jl_lexer_next(lexer);
        r1 = jl_binary(JL_OP_NE, r1, relational_expression(self, lexer, out));
        break;
      default:
        return r1;
    }
  }
}

static jl_expr_t and_expression(jl_fe_t *self, jl_lexer_t *lexer, jl_program_t *out) {
  jl_expr_t r1;

  r1 = equality_expression(self, lexer, out);
  while (true) {
    if (jl_lexer_peek(lexer).type != '&') {
      return r1;
    }
    jl_lexer_next(lexer);
    r1 = jl_binary(JL_OP_AND, r1, equality_expression(self, lexer, out));
  }
}

static jl_expr_t exclusive_or_expression(jl_fe_t *self, jl_lexer_t *lexer, jl_program_t *out) {
  jl_expr_t r1;

  r1 = and_expression(self, lexer, out);
  while (true) {
    if (jl_lexer_peek(lexer).type != '^') {
      return r1;
    }
    jl_lexer_next(lexer);
    r1 = jl_binary(JL_OP_XOR, r1, and_expression(self, lexer, out));
  }
}

static jl_expr_t inclusive_or_expression(jl_fe_t *self, jl_lexer_t *lexer, jl_program_t *out) {
  jl_expr_t r1;

  r1 = exclusive_or_expression(self, lexer, out);
  while (true) {
    if (jl_lexer_peek(lexer).type != '|') {
      return r1;
    }
    jl_lexer_next(lexer);
    r1 = jl_binary(JL_OP_OR, r1, exclusive_or_expression(self, lexer, out));
  }
}

static jl_expr_t logical_and_expression(jl_fe_t *self, jl_lexer_t *lexer, jl_program_t *out) {
  jl_expr_t r1;

  r1 = inclusive_or_expression(self, lexer, out);
  while (true) {
    if (jl_lexer_peek(lexer).type != C_TOK_LOGICAL_AND) {
      return r1;
    }
    jl_lexer_next(lexer);
    r1 = jl_binary(JL_OP_LAND, r1, inclusive_or_expression(self, lexer, out));
  }
}

static jl_expr_t logical_or_expression(jl_fe_t *self, jl_lexer_t *lexer, jl_program_t *out) {
  jl_expr_t r1;

  r1 = logical_and_expression(self, lexer, out);
  while (true) {
    if (jl_lexer_peek(lexer).type != C_TOK_LOGICAL_OR) {
      return r1;
    }
    jl_lexer_next(lexer);
    r1 = jl_binary(JL_OP_LOR, r1, logical_and_expression(self, lexer, out));
  }
}

static jl_expr_t conditional_expression(jl_fe_t *self, jl_lexer_t *lexer, jl_program_t *out) {
  jl_expr_t r1, r2;

  r1 = logical_or_expression(self, lexer, out);
  if (jl_lexer_peek(lexer).type != '?') {
    return r1;
  }
  jl_lexer_next(lexer);
  r2 = expression(self, lexer, out);
  jl_lexer_consume(lexer, ':');
  return jl_ternary(r1, r2, conditional_expression(self, lexer, out));
}

static jl_expr_t assignment_expression(jl_fe_t *self, jl_lexer_t *lexer, jl_program_t *out) {
  jl_expr_t r1;
  enum jl_op_n op;

  r1 = conditional_expression(self, lexer, out);
  switch (jl_lexer_peek(lexer).type) {
    case C_TOK_ASSIGN:
      jl_lexer_next(lexer);
      op = JL_OP_ASSIGN;
      break;
    case C_TOK_MUL_ASSIGN:
      jl_lexer_next(lexer);
      op = JL_OP_MUL;
      break;
    case C_TOK_DIV_ASSIGN:
      jl_lexer_next(lexer);
      op = JL_OP_DIV;
      break;
    case C_TOK_MOD_ASSIGN:
      jl_lexer_next(lexer);
      op = JL_OP_MOD;
      break;
    case C_TOK_PLUS_ASSIGN:
      jl_lexer_next(lexer);
      op = JL_OP_ADD;
      break;
    case C_TOK_MINUS_ASSIGN:
      jl_lexer_next(lexer);
      op = JL_OP_SUB;
      break;
    case C_TOK_AND_ASSIGN:
      jl_lexer_next(lexer);
      op = JL_OP_AND;
      break;
    case C_TOK_OR_ASSIGN:
      jl_lexer_next(lexer);
      op = JL_OP_OR;
      break;
    case C_TOK_XOR_ASSIGN:
      jl_lexer_next(lexer);
      op = JL_OP_XOR;
      break;
    case C_TOK_RSHIFT_ASSIGN:
      jl_lexer_next(lexer);
      op = JL_OP_SHR;
      break;
    case C_TOK_LSHIFT_ASSIGN:
      jl_lexer_next(lexer);
      op = JL_OP_SHL;
      break;
    default:
      return r1;
  }
  if (jl_expr_is_binary(r1)) {
    printf("unexpected left assign operand");
    exit(1);
  }
  return jl_binary(JL_OP_ASSIGN, r1, op != JL_OP_ASSIGN
    ? jl_binary(op, r1, assignment_expression(self, lexer, out))
    : assignment_expression(self, lexer, out)
  );
}

static jl_expr_t expression(jl_fe_t *self, jl_lexer_t *lexer, jl_program_t *out) {
  jl_expr_t r1;

  r1 = assignment_expression(self, lexer, out);
  while (true) {
    if (jl_lexer_peek(lexer).type != ',') {
      return r1;
    }
    jl_lexer_next(lexer);
    if (!jl_expr_is_list(r1)) {
      r1 = jl_exprs_start(r1);
    }
    adt_vector_push(jl_u(r1, list)->exprs, assignment_expression(self, lexer, out));
  }
}

static jl_expr_t constant_expression(jl_fe_t *self, jl_lexer_t *lexer, jl_program_t *out) {
  jl_expr_t r1;

  r1 = conditional_expression(self, lexer, out);
  if (!jl_expr_is_constant(r1)) {
    puts("Constant expression must be computable at compile time");
    exit(1);
  }
  return r1;
}

static jl_type_t declaration_specifiers(jl_fe_t *self, jl_lexer_t *lexer, jl_program_t *out) {
  jl_type_t type = jl_type_undefined();

  while (true) {
    switch (jl_lexer_peek(lexer).type) {
      case C_TOK_TYPEDEF:
        jl_lexer_next(lexer);
        type.specifiers |= JL_TYPE_SPECIFIER_TYPEDEF;
        break;
      case C_TOK_EXTERN:
        jl_lexer_next(lexer);
        type.specifiers |= JL_TYPE_SPECIFIER_EXTERN;
        break;
      case C_TOK_STATIC:
        jl_lexer_next(lexer);
        type.specifiers |= JL_TYPE_SPECIFIER_STATIC;
        break;
      case C_TOK_THREAD_LOCAL:
        jl_lexer_next(lexer);
        type.specifiers |= JL_TYPE_SPECIFIER_THREAD_LOCAL;
        break;
      case C_TOK_AUTO:
        jl_lexer_next(lexer);
        type.specifiers |= JL_TYPE_SPECIFIER_AUTO;
        break;
      case C_TOK_REGISTER:
        jl_lexer_next(lexer);
        type.specifiers |= JL_TYPE_SPECIFIER_REGISTER;
        break;
      case C_TOK_INLINE:
        jl_lexer_next(lexer);
        type.specifiers |= JL_FUNC_SPECIFIER_INLINE;
        break;
      case C_TOK_NORETURN:
        jl_lexer_next(lexer);
        type.specifiers |= JL_FUNC_SPECIFIER_NORETURN;
        break;
      case C_TOK_CONST:
        jl_lexer_next(lexer);
        type.qualifiers |= JL_TYPE_QUALIFIER_CONST;
        break;
      case C_TOK_RESTRICT:
        jl_lexer_next(lexer);
        type.qualifiers |= JL_TYPE_QUALIFIER_RESTRICT;
        break;
      case C_TOK_VOLATILE:
        jl_lexer_next(lexer);
        type.qualifiers |= JL_TYPE_QUALIFIER_VOLATILE;
        break;
      case C_TOK_ATOMIC:
        jl_lexer_next(lexer);
        type.qualifiers |= JL_TYPE_QUALIFIER_ATOMIC;
        break;
      case C_TOK_ALIGNAS:
        puts("_Alignas still unsupported");
        exit(1);
      default:
        return type;
    }
  }
}

static jl_type_t type_specifier(jl_fe_t *self, jl_lexer_t *lexer, jl_program_t *out) {
  jl_token_t token;
  jl_type_t type = jl_type_undefined();
  enum jl_type_n type_kind;

  while (true) {
    switch ((token = jl_lexer_peek(lexer)).type) {
      case C_TOK_VOID:
        type_kind = JL_TYPE_VOID;
        goto dft_literal;
      case C_TOK_CHAR:
        type_kind = JL_TYPE_CHAR;
        goto dft_literal;
      default:
        return type;
      dft_literal:
        if (jl_defined(type)) {
          puts("two or more data types in declaration specifiers");
          exit(1);
        }
        jl_lexer_next(lexer);
        jl_type_switch(&type, type_kind);
        break;
    }
  }
}

static jl_type_t type_name(jl_fe_t *self, jl_lexer_t *lexer, jl_program_t *out) {
  return jl_type_undefined();
}
