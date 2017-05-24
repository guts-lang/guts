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
#include <errno.h>

#include "c_parser.h"

#include "c_lexer.h"
#include "compiler.h"
#include "entity.h"
#include "expr.h"
#include "stmt.h"
#include "type.h"

static jl_expr_t primary_expression(jl_parser_t *self, jl_program_t *out);
static jl_expr_t postfix_expression(jl_parser_t *self, jl_program_t *out);
static jl_expr_t unary_expression(jl_parser_t *self, jl_program_t *out);
static jl_expr_t cast_expression(jl_parser_t *self, jl_program_t *out);
static jl_expr_t multiplicative_expression(jl_parser_t *self, jl_program_t *out);
static jl_expr_t additive_expression(jl_parser_t *self, jl_program_t *out);
static jl_expr_t shift_expression(jl_parser_t *self, jl_program_t *out);
static jl_expr_t relational_expression(jl_parser_t *self, jl_program_t *out);
static jl_expr_t equality_expression(jl_parser_t *self, jl_program_t *out);
static jl_expr_t and_expression(jl_parser_t *self, jl_program_t *out);
static jl_expr_t exclusive_or_expression(jl_parser_t *self, jl_program_t *out);
static jl_expr_t inclusive_or_expression(jl_parser_t *self, jl_program_t *out);
static jl_expr_t logical_and_expression(jl_parser_t *self, jl_program_t *out);
static jl_expr_t logical_or_expression(jl_parser_t *self, jl_program_t *out);
static jl_expr_t conditional_expression(jl_parser_t *self, jl_program_t *out);
static jl_expr_t assignment_expression(jl_parser_t *self, jl_program_t *out);
static jl_expr_t expression(jl_parser_t *self, jl_program_t *out);
static jl_expr_t constant_expression(jl_parser_t *self, jl_program_t *out);
static jl_entity_t declaration(jl_parser_t *self, jl_program_t *out);
static jl_type_t declaration_specifiers(jl_parser_t *self, jl_program_t *out);
static jl_entity_t init_declarator_list(jl_parser_t *self, jl_program_t *out);
static jl_entity_t init_declarator(jl_parser_t *self, jl_program_t *out);
static jl_type_t type_specifier(jl_parser_t *self, jl_program_t *out);
static jl_entity_t struct_or_union_specifier(jl_parser_t *self, jl_program_t *out);
static jl_entity_t struct_or_union(jl_parser_t *self, jl_program_t *out);
static jl_entity_t struct_declaration_list(jl_parser_t *self, jl_program_t *out);
static jl_entity_t struct_declaration(jl_parser_t *self, jl_program_t *out);
static jl_entity_t specifier_qualifier_list(jl_parser_t *self, jl_program_t *out);
static jl_entity_t struct_declarator_list(jl_parser_t *self, jl_program_t *out);
static jl_entity_t struct_declarator(jl_parser_t *self, jl_program_t *out);
static jl_entity_t enum_specifier(jl_parser_t *self, jl_program_t *out);
static jl_entity_t enumerator_list(jl_parser_t *self, jl_program_t *out);
static jl_entity_t enumerator(jl_parser_t *self, jl_program_t *out);
static jl_entity_t atomic_type_specifier(jl_parser_t *self, jl_program_t *out);
static jl_entity_t type_qualifier(jl_parser_t *self, jl_program_t *out);
static jl_entity_t function_specifier(jl_parser_t *self, jl_program_t *out);
static jl_entity_t alignment_specifier(jl_parser_t *self, jl_program_t *out);
static jl_entity_t declarator(jl_parser_t *self, jl_program_t *out);
static jl_entity_t direct_declarator(jl_parser_t *self, jl_program_t *out);
static jl_entity_t pointer(jl_parser_t *self, jl_program_t *out);
static jl_entity_t type_qualifier_list(jl_parser_t *self, jl_program_t *out);
static jl_entity_t parameter_type_list(jl_parser_t *self, jl_program_t *out);
static jl_entity_t parameter_list(jl_parser_t *self, jl_program_t *out);
static jl_entity_t parameter_declaration(jl_parser_t *self, jl_program_t *out);
static jl_entity_t identifier_list(jl_parser_t *self, jl_program_t *out);
static jl_type_t type_name(jl_parser_t *self, jl_program_t *out);
static jl_entity_t abstract_declarator(jl_parser_t *self, jl_program_t *out);
static jl_entity_t direct_abstract_declarator(jl_parser_t *self, jl_program_t *out);
static jl_entity_t initializer(jl_parser_t *self, jl_program_t *out);
static jl_entity_t initializer_list(jl_parser_t *self, jl_program_t *out);
static jl_entity_t designation(jl_parser_t *self, jl_program_t *out);
static jl_entity_t designator_list(jl_parser_t *self, jl_program_t *out);
static jl_entity_t designator(jl_parser_t *self, jl_program_t *out);
static jl_entity_t static_assert_declaration(jl_parser_t *self, jl_program_t *out);
static jl_entity_t statement(jl_parser_t *self, jl_program_t *out);
static jl_entity_t labeled_statement(jl_parser_t *self, jl_program_t *out);
static jl_entity_t compound_statement(jl_parser_t *self, jl_program_t *out);
static jl_entity_t block_item_list(jl_parser_t *self, jl_program_t *out);
static jl_entity_t block_item(jl_parser_t *self, jl_program_t *out);
static jl_entity_t expression_statement(jl_parser_t *self, jl_program_t *out);
static jl_entity_t selection_statement(jl_parser_t *self, jl_program_t *out);
static jl_entity_t iteration_statement(jl_parser_t *self, jl_program_t *out);
static jl_entity_t jump_statement(jl_parser_t *self, jl_program_t *out);
static jl_entity_t translation_unit(jl_parser_t *self, jl_program_t *out);
static jl_entity_t external_declaration(jl_parser_t *self, jl_program_t *out);
static jl_entity_t function_definition(jl_parser_t *self, jl_program_t *out);
static jl_entity_t declaration_list(jl_parser_t *self, jl_program_t *out);

void c_parser_parse(jl_parser_t *self, jl_program_t *out) {
  int r;

  jl_fe_scope(self);
  self->scope->sym = jl_sym_put(self->scope, "foo", &r);
  jl_fe_unscope(self);

  jl_expr_t expr = expression(self, out);
  jl_expr_dtor(&expr);

  while (jl_lexer_next(self->lexer).type != 0);
}

enum {
  C_TOKEN_FLAG_NONE = 0,
  C_TOKEN_FLAG_ENUMERATION_CONSTANT = 1 << 0
};

static jl_expr_t primary_expression(jl_parser_t *self, jl_program_t *out) {
  int r;
  jl_sym_t *symbol;
  jl_token_t token;
  jl_expr_t r1, r2;
  jl_lloc_t lloc = jl_lloc_begin(self->lexer);

  switch ((token = jl_lexer_peek(self->lexer)).type) {
    case C_TOK_IDENTIFIER:
      jl_lexer_next(self->lexer);
      symbol = jl_sym_get(self->scope, token.value, &r);
      if (r) {
        jl_parse_err(self->compiler, token.loc, "Undefined symbol '%s'", token.value);
      }
      if (symbol->flags & C_TOKEN_FLAG_ENUMERATION_CONSTANT) {
        return symbol->entity.variable.initializer;
      }
      return jl_id(jl_lloc_end(lloc), symbol->id, symbol->entity.type);
    case C_TOK_NUMBER:
      jl_lexer_next(self->lexer);
      switch (jl_const_parse(jl_lloc_end(lloc), token.value, token.length, &r2)) {
        case ERANGE:
          jl_parse_err(self->compiler, token.loc, "Numeric literal '%s' is out of range", token.value);
        case 1:
          jl_parse_err(self->compiler, token.loc, "Invalid numeric literal '%s'", token.value);
        default:
          break;
      }
      return r2;
    case C_TOK_STRING:
      jl_lexer_next(self->lexer);
      return jl_const_string(jl_lloc_end(lloc), token.value);
    case C_TOK_FUNC_NAME:
      if (!(symbol = self->scope->sym) || !jl_is(symbol->entity, JL_ENTITY_FUNC)) {
        jl_parse_err(self->compiler, token.loc,
          "access of __func__ outside of function"
        );
      }
      jl_lexer_next(self->lexer);
      return jl_const_string(jl_lloc_end(lloc), symbol->entity.name);
    case '(':
      jl_lexer_next(self->lexer);
      r1 = expression(self, out);
      jl_lexer_consume(self->lexer, ')');
      return jl_unary(jl_lloc_end(lloc), JL_OP_EN, r1);
    default:
      jl_parse_err(self->compiler, token.loc,
        "Unexpected '%s', not a valid primary expression",
        token.value ? token.value : token.name
      );
  }
}

static jl_expr_t postfix_expression(jl_parser_t *self, jl_program_t *out) {
  jl_token_t token;
  jl_expr_t r1, r2;
  jl_lloc_t lloc;

  r1 = primary_expression(self, out);
  while (true) {
    lloc = jl_lloc_begin(self->lexer);
    switch ((token = jl_lexer_peek(self->lexer)).type) {
      case '[':
        if (!jl_type_is_ref(r1.type)) {
          jl_parse_err(self->compiler, token.loc,
            "Array access on non array element"
          );
        }
        jl_lexer_next(self->lexer);
        r2 = expression(self, out);
        jl_lexer_consume(self->lexer, ']');
        r1 = jl_array_read(r1, jl_lloc_end(lloc), r2);
        break;
      case '(': {
        unsigned i;
        jl_entity_t param;
        jl_func_t *func;
        jl_expr_r args = (jl_expr_r) {0};
        jl_type_t type = r1.type;

        if (jl_is(type, JL_TYPE_POINTER) && jl_type_is_func(*type.pointer.of)) {
          type = *type.pointer.of;
        } else if (!jl_type_is_func(type)) {
          jl_parse_err(self->compiler, token.loc,
            "Expression must have type pointer to function"
          );
        }
        func = &type.compound.entity->function;
        jl_lexer_next(self->lexer);
        for (i = 0; i < ds_size(func->params); ++i) {
          if (jl_lexer_peek(self->lexer).type == ')') {
            jl_parse_err(self->compiler, token.loc,
              "Too few arguments, expected %zu but got %d",
              ds_size(func->params), i
            );
          }
          param = ds_at(func->params, i);
          r2 = assignment_expression(self, out);
          if (!jl_type_equals(r2.type, param.type)) {
            r2 = jl_cast(r2.lloc, param.type, r2);
          }
          vec_push(args, r2);
          if (i < ds_size(func->params) - 1) {
            jl_lexer_consume(self->lexer, ',');
          }
        }
        jl_lexer_consume(self->lexer, ')');
        vec_push(args, jl_expr_undefined());
        r1 = jl_call(r1, jl_lloc_end(lloc), jl_exprs(args.data).exprs);
        break;
      }
      case '.': {
        jl_field_t *field;

        jl_lexer_next(self->lexer);
        token = jl_lexer_consume(self->lexer, C_TOK_IDENTIFIER);
        field = jl_field_lookup(r1.type, token.value);
        if (!field) {
          jl_parse_err(self->compiler, token.loc,
            "Invalid access, no member named '%s'",
            token.value
          );
        }
        r1 = jl_field_read(r1, jl_id(jl_lloc_end(lloc), field->name, field->type).id);
        break;
      }
      case C_TOK_PTR_OP: {
        jl_type_t type;
        jl_field_t *field;

        jl_lexer_next(self->lexer);
        if (!jl_is(type = r1.type, JL_TYPE_POINTER)) {
          jl_parse_err(self->compiler, token.loc,
            "Invalid access on non pointer element '%s'",
            token.value
          );
        }
        token = jl_lexer_consume(self->lexer, C_TOK_IDENTIFIER);
        field = jl_field_lookup(jl_type_deref(type), token.value);
        if (!field) {
          jl_parse_err(self->compiler, token.loc,
            "Invalid access, no member named '%s'",
            token.value
          );
        }
        r1 = jl_field_read(r1, jl_id(jl_lloc_end(lloc), field->name, field->type).id);
        break;
      }
      default:
        return r1;
    }
  }
}

static jl_expr_t unary_expression(jl_parser_t *self, jl_program_t *out) {
  jl_expr_t r1;
  enum jl_op_n op;
  jl_lloc_t lloc = jl_lloc_begin(self->lexer);

  switch (jl_lexer_peek(self->lexer).type) {
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
      return postfix_expression(self, out);
    self_assign:
      jl_lexer_next(self->lexer);
      lloc = jl_lloc_end(lloc);
      r1 = unary_expression(self, out);
      return jl_unary(lloc, JL_OP_EN, jl_binary(JL_OP_ASSIGN, r1, jl_binary(op, r1, jl_const_int(r1.lloc, 1))));
  }
  jl_lexer_next(self->lexer);
  return jl_unary(jl_lloc_end(lloc), op, cast_expression(self, out));
}

static jl_expr_t cast_expression(jl_parser_t *self, jl_program_t *out) {
  jl_type_t type;
  jl_token_t token;
  jl_lloc_t lloc = jl_lloc_begin(self->lexer);

  if ((token = jl_lexer_peek(self->lexer)).type == '(') {
    jl_lexer_next(self->lexer);
    if (!jl_defined(type = type_name(self, out))) {
      jl_lexer_undo(self->lexer, token);
      return unary_expression(self, out);
    }
    jl_lexer_consume(self->lexer, ')');
    return jl_cast(jl_lloc_end(lloc), type, cast_expression(self, out));
  }

  return unary_expression(self, out);
}

static jl_expr_t multiplicative_expression(jl_parser_t *self, jl_program_t *out) {
  jl_expr_t r1;

  r1 = cast_expression(self, out);
  while (true) {
    switch (jl_lexer_peek(self->lexer).type) {
      case '*':
        jl_lexer_next(self->lexer);
        r1 = jl_binary(JL_OP_MUL, r1, cast_expression(self, out));
        break;
      case '/':
        jl_lexer_next(self->lexer);
        r1 = jl_binary(JL_OP_DIV, r1, cast_expression(self, out));
        break;
      case '%':
        jl_lexer_next(self->lexer);
        r1 = jl_binary(JL_OP_MOD, r1, cast_expression(self, out));
        break;
      default:
        return r1;
    }
  }
}

static jl_expr_t additive_expression(jl_parser_t *self, jl_program_t *out) {
  jl_expr_t r1;

  r1 = multiplicative_expression(self, out);
  while (true) {
    switch (jl_lexer_peek(self->lexer).type) {
      case '+':
        jl_lexer_next(self->lexer);
        r1 = jl_binary(JL_OP_ADD, r1, multiplicative_expression(self, out));
        break;
      case '-':
        jl_lexer_next(self->lexer);
        r1 = jl_binary(JL_OP_SUB, r1, multiplicative_expression(self, out));
        break;
      default:
        return r1;
    }
  }
}

static jl_expr_t shift_expression(jl_parser_t *self, jl_program_t *out) {
  jl_expr_t r1;

  r1 = additive_expression(self, out);
  while (true) {
    switch (jl_lexer_peek(self->lexer).type) {
      case C_TOK_LEFT_OP:
        jl_lexer_next(self->lexer);
        r1 = jl_binary(JL_OP_SHL, r1, additive_expression(self, out));
        break;
      case C_TOK_RIGHT_OP:
        jl_lexer_next(self->lexer);
        r1 = jl_binary(JL_OP_SHR, r1, additive_expression(self, out));
        break;
      default:
        return r1;
    }
  }
}

static jl_expr_t relational_expression(jl_parser_t *self, jl_program_t *out) {
  jl_expr_t r1;

  r1 = shift_expression(self, out);
  while (true) {
    switch (jl_lexer_peek(self->lexer).type) {
      case '<':
        jl_lexer_next(self->lexer);
        r1 = jl_binary(JL_OP_LT, r1, shift_expression(self, out));
        break;
      case C_TOK_LE_OP:
        jl_lexer_next(self->lexer);
        r1 = jl_binary(JL_OP_LE, r1, shift_expression(self, out));
        break;
      case '>':
        jl_lexer_next(self->lexer);
        r1 = jl_binary(JL_OP_GT, r1, shift_expression(self, out));
        break;
      case C_TOK_GE_OP:
        jl_lexer_next(self->lexer);
        r1 = jl_binary(JL_OP_GE, r1, shift_expression(self, out));
        break;
      default:
        return r1;
    }
  }
}

static jl_expr_t equality_expression(jl_parser_t *self, jl_program_t *out) {
  jl_expr_t r1;

  r1 = relational_expression(self, out);
  while (true) {
    switch (jl_lexer_peek(self->lexer).type) {
      case C_TOK_EQ_OP:
        jl_lexer_next(self->lexer);
        r1 = jl_binary(JL_OP_EQ, r1, relational_expression(self, out));
        break;
      case C_TOK_NE_OP:
        jl_lexer_next(self->lexer);
        r1 = jl_binary(JL_OP_NE, r1, relational_expression(self, out));
        break;
      default:
        return r1;
    }
  }
}

static jl_expr_t and_expression(jl_parser_t *self, jl_program_t *out) {
  jl_expr_t r1;

  r1 = equality_expression(self, out);
  while (true) {
    if (jl_lexer_peek(self->lexer).type != '&') {
      return r1;
    }
    jl_lexer_next(self->lexer);
    r1 = jl_binary(JL_OP_AND, r1, equality_expression(self, out));
  }
}

static jl_expr_t exclusive_or_expression(jl_parser_t *self, jl_program_t *out) {
  jl_expr_t r1;

  r1 = and_expression(self, out);
  while (true) {
    if (jl_lexer_peek(self->lexer).type != '^') {
      return r1;
    }
    jl_lexer_next(self->lexer);
    r1 = jl_binary(JL_OP_XOR, r1, and_expression(self, out));
  }
}

static jl_expr_t inclusive_or_expression(jl_parser_t *self, jl_program_t *out) {
  jl_expr_t r1;

  r1 = exclusive_or_expression(self, out);
  while (true) {
    if (jl_lexer_peek(self->lexer).type != '|') {
      return r1;
    }
    jl_lexer_next(self->lexer);
    r1 = jl_binary(JL_OP_OR, r1, exclusive_or_expression(self, out));
  }
}

static jl_expr_t logical_and_expression(jl_parser_t *self, jl_program_t *out) {
  jl_expr_t r1;

  r1 = inclusive_or_expression(self, out);
  while (true) {
    if (jl_lexer_peek(self->lexer).type != C_TOK_LOGICAL_AND) {
      return r1;
    }
    jl_lexer_next(self->lexer);
    r1 = jl_binary(JL_OP_LAND, r1, inclusive_or_expression(self, out));
  }
}

static jl_expr_t logical_or_expression(jl_parser_t *self, jl_program_t *out) {
  jl_expr_t r1;

  r1 = logical_and_expression(self, out);
  while (true) {
    if (jl_lexer_peek(self->lexer).type != C_TOK_LOGICAL_OR) {
      return r1;
    }
    jl_lexer_next(self->lexer);
    r1 = jl_binary(JL_OP_LOR, r1, logical_and_expression(self, out));
  }
}

static jl_expr_t conditional_expression(jl_parser_t *self, jl_program_t *out) {
  jl_expr_t r1, r2;

  r1 = logical_or_expression(self, out);
  if (jl_lexer_peek(self->lexer).type != '?') {
    return r1;
  }
  jl_lexer_next(self->lexer);
  r2 = expression(self, out);
  jl_lexer_consume(self->lexer, ':');
  return jl_ternary(r1, r2, conditional_expression(self, out));
}

static jl_expr_t assignment_expression(jl_parser_t *self, jl_program_t *out) {
  jl_expr_t r1;
  enum jl_op_n op;

  r1 = conditional_expression(self, out);
  switch (jl_lexer_peek(self->lexer).type) {
    case C_TOK_ASSIGN:
      jl_lexer_next(self->lexer);
      op = JL_OP_ASSIGN;
      break;
    case C_TOK_MUL_ASSIGN:
      jl_lexer_next(self->lexer);
      op = JL_OP_MUL;
      break;
    case C_TOK_DIV_ASSIGN:
      jl_lexer_next(self->lexer);
      op = JL_OP_DIV;
      break;
    case C_TOK_MOD_ASSIGN:
      jl_lexer_next(self->lexer);
      op = JL_OP_MOD;
      break;
    case C_TOK_PLUS_ASSIGN:
      jl_lexer_next(self->lexer);
      op = JL_OP_ADD;
      break;
    case C_TOK_MINUS_ASSIGN:
      jl_lexer_next(self->lexer);
      op = JL_OP_SUB;
      break;
    case C_TOK_AND_ASSIGN:
      jl_lexer_next(self->lexer);
      op = JL_OP_AND;
      break;
    case C_TOK_OR_ASSIGN:
      jl_lexer_next(self->lexer);
      op = JL_OP_OR;
      break;
    case C_TOK_XOR_ASSIGN:
      jl_lexer_next(self->lexer);
      op = JL_OP_XOR;
      break;
    case C_TOK_RSHIFT_ASSIGN:
      jl_lexer_next(self->lexer);
      op = JL_OP_SHR;
      break;
    case C_TOK_LSHIFT_ASSIGN:
      jl_lexer_next(self->lexer);
      op = JL_OP_SHL;
      break;
    default:
      return r1;
  }
  if (jl_is(r1, JL_EXPR_BINARY)) {
    jl_parse_err(self->compiler, jl_lexer_peek(self->lexer).loc,
      "Unexpected left assign operand"
    );
  }
  return jl_binary(JL_OP_ASSIGN, r1, op != JL_OP_ASSIGN
    ? jl_binary(op, r1, assignment_expression(self, out))
    : assignment_expression(self, out)
  );
}

static jl_expr_t expression(jl_parser_t *self, jl_program_t *out) {
  jl_expr_t r1;

  r1 = assignment_expression(self, out);
  while (true) {
    if (jl_lexer_peek(self->lexer).type != ',') {
      return r1;
    }
    jl_lexer_next(self->lexer);
    if (!jl_is(r1, JL_EXPR_EXPRS)) {
      r1 = jl_exprs_start(r1);
    }
    vec_push(r1.exprs.vector, assignment_expression(self, out));
  }
}

static jl_expr_t constant_expression(jl_parser_t *self, jl_program_t *out) {
  jl_expr_t r1;

  r1 = conditional_expression(self, out);
  if (!jl_expr_is_constant(r1)) {
    jl_parse_err(self->compiler, jl_lexer_peek(self->lexer).loc,
      "Constant expression must be computable at compile time"
    );
  }
  return r1;
}

static jl_type_t declaration_specifiers(jl_parser_t *self, jl_program_t *out) {
  jl_token_t token;
  jl_type_t type = jl_type_undefined();

  while (true) {
    switch ((token = jl_lexer_peek(self->lexer)).type) {
      case C_TOK_TYPEDEF:
        jl_lexer_next(self->lexer);
        type.specifiers |= JL_TYPE_SPECIFIER_TYPEDEF;
        break;
      case C_TOK_EXTERN:
        jl_lexer_next(self->lexer);
        type.specifiers |= JL_TYPE_SPECIFIER_EXTERN;
        break;
      case C_TOK_STATIC:
        jl_lexer_next(self->lexer);
        type.specifiers |= JL_TYPE_SPECIFIER_STATIC;
        break;
      case C_TOK_THREAD_LOCAL:
        jl_lexer_next(self->lexer);
        type.specifiers |= JL_TYPE_SPECIFIER_THREAD_LOCAL;
        break;
      case C_TOK_AUTO:
        jl_lexer_next(self->lexer);
        type.specifiers |= JL_TYPE_SPECIFIER_AUTO;
        break;
      case C_TOK_REGISTER:
        jl_lexer_next(self->lexer);
        type.specifiers |= JL_TYPE_SPECIFIER_REGISTER;
        break;
      case C_TOK_INLINE:
        jl_lexer_next(self->lexer);
        type.specifiers |= JL_FUNC_SPECIFIER_INLINE;
        break;
      case C_TOK_NORETURN:
        jl_lexer_next(self->lexer);
        type.specifiers |= JL_FUNC_SPECIFIER_NORETURN;
        break;
      case C_TOK_CONST:
        jl_lexer_next(self->lexer);
        type.qualifiers |= JL_TYPE_QUALIFIER_CONST;
        break;
      case C_TOK_RESTRICT:
        jl_lexer_next(self->lexer);
        type.qualifiers |= JL_TYPE_QUALIFIER_RESTRICT;
        break;
      case C_TOK_VOLATILE:
        jl_lexer_next(self->lexer);
        type.qualifiers |= JL_TYPE_QUALIFIER_VOLATILE;
        break;
      case C_TOK_ATOMIC:
        jl_lexer_next(self->lexer);
        type.qualifiers |= JL_TYPE_QUALIFIER_ATOMIC;
        break;
      case C_TOK_ALIGNAS:
        jl_parse_err(self->compiler, token.loc,
          "_Alignas still unsupported"
        );
      default:
        return type;
    }
  }
}

static jl_type_t type_specifier(jl_parser_t *self, jl_program_t *out) {
  jl_token_t token;
  jl_type_t type = jl_type_undefined();
  //enum jl_type_n type_kind;

  while (true) {
    switch ((token = jl_lexer_peek(self->lexer)).type) {
      case C_TOK_VOID:
        //type_kind = JL_TYPE_VOID;
        goto dft_literal;
      case C_TOK_CHAR:
        //type_kind = JL_TYPE_CHAR;
        goto dft_literal;
      default:
        return type;
      dft_literal:
        if (jl_defined(type)) {
          jl_parse_err(self->compiler, token.loc,
            "Two or more data types in declaration specifiers"
          );
        }
        jl_lexer_next(self->lexer);
        break;
    }
  }
}

static jl_type_t type_name(jl_parser_t *self, jl_program_t *out) {
  return jl_type_undefined();
}
