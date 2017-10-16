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


#include <lex.h>
#include <lex/tok.h>
#include <ds.h>

enum {
  C_TOK_END = '\0', C_TOK_AUTO = C_TOK_END + 1, C_TOK_BREAK, C_TOK_CASE,
  C_TOK_CHAR, C_TOK_CONST, C_TOK_CONTINUE, C_TOK_DEFAULT, C_TOK_DO,
  C_TOK_DOUBLE, C_TOK_EOL = '\n', C_TOK_ELSE = C_TOK_EOL + 1, C_TOK_ENUM,
  C_TOK_EXTERN, C_TOK_FLOAT, C_TOK_FOR, C_TOK_GOTO, C_TOK_IF, C_TOK_INLINE,
  C_TOK_INT, C_TOK_LONG, C_TOK_REGISTER, C_TOK_RESTRICT, C_TOK_RETURN,
  C_TOK_SHORT, C_TOK_SIGNED, C_TOK_SIZEOF, C_TOK_STATIC, C_TOK_STRUCT,
  C_TOK_SWITCH, C_TOK_TYPEDEF, C_TOK_UNION, C_TOK_UNSIGNED,

  C_TOK_NOT = '!', C_TOK_VOID = C_TOK_NOT + 1, C_TOK_HASH = '#',
  C_TOK_VOLATILE = C_TOK_HASH + 1, C_TOK_MODULO = '%', C_TOK_AND = '&',
  C_TOK_WHILE = C_TOK_AND + 1,

  C_TOK_OPEN_PAREN = '(', C_TOK_CLOSE_PAREN = ')', C_TOK_STAR = '*',
  C_TOK_PLUS = '+', C_TOK_COMMA = ',', C_TOK_MINUS = '-', C_TOK_DOT = '.',
  C_TOK_SLASH = '/',

  C_TOK_ALIGNAS = C_TOK_SLASH + 1, C_TOK_ALIGNOF, C_TOK_ATOMIC, C_TOK_BOOL,
  C_TOK_COMPLEX, C_TOK_GENERIC, C_TOK_IMAGINARY, C_TOK_NORETURN,
  C_TOK_STATIC_ASSERT, C_TOK_THREAD_LOCAL,

  C_TOK_COLON = ':', C_TOK_SEMICOLON = ';', C_TOK_LT = '<', C_TOK_ASSIGN = '=',
  C_TOK_GT = '>', C_TOK_QUESTION = '?', C_TOK_DOTS = C_TOK_QUESTION + 1,
  C_TOK_LOGICAL_OR, C_TOK_LOGICAL_AND, C_TOK_LE_OP, C_TOK_GE_OP, C_TOK_EQ_OP,
  C_TOK_NE_OP, C_TOK_PTR_OP, C_TOK_INCREMENT, C_TOK_DECREMENT, C_TOK_LEFT_OP,
  C_TOK_RIGHT_OP, C_TOK_MUL_ASSIGN, C_TOK_DIV_ASSIGN, C_TOK_MOD_ASSIGN,
  C_TOK_PLUS_ASSIGN, C_TOK_MINUS_ASSIGN, C_TOK_LSHIFT_ASSIGN,
  C_TOK_RSHIFT_ASSIGN, C_TOK_AND_ASSIGN, C_TOK_XOR_ASSIGN, C_TOK_OR_ASSIGN,
  C_TOK_TOKEN_PASTE,

  C_TOK_FUNC_NAME,

  C_TOK_OPEN_BRACKET = '[', C_TOK_ANTI_SLASH = '\\', C_TOK_CLOSE_BRACKET = ']',
  C_TOK_XOR = '^', C_TOK_OPEN_CURLY = '{', C_TOK_OR = '|',
  C_TOK_CLOSE_CURLY = '}', C_TOK_NEG = '~',

  C_TOK_NUMBER = 116, C_TOK_IDENTIFIER = C_TOK_NUMBER + 1, C_TOK_STRING
};

#define __M(n, i, c) s[i + n] == (c)
#define __M1(n, a) __M(n, 0, a)
#define __M2(n, a, b) __M1(n, a) && __M(n, 1, b)
#define __M3(n, a, b, c) __M2(n, a, b) && __M(n, 2, c)
#define __M4(n, a, b, c, d) __M3(n, a, b, c) && __M(n, 3, d)
#define __M5(n, a, b, c, d, e) __M4(n, a, b, c, d) && __M(n, 4, e)
#define __M6(n, a, b, c, d, e, f) __M5(n, a, b, c, d, e) && __M(n, 5, f)
#define __M7(n, a, b, c, d, e, f, g) __M6(n, a, b, c, d, e, f) && __M(n, 6, g)
#define __M8(n, a, b, c, d, e, f, g, h) __M7(n, a, b, c, d, e, f, g) && __M(n, 7, h)
#define M1(n, a) i == (1+n) && __M1(n, a)
#define M2(n, a, b) i == (2+n) && __M2(n, a, b)
#define M3(n, a, b, c) i == (3+n) && __M3(n, a, b, c)
#define M4(n, a, b, c, d) i == (4+n) && __M4(n, a, b, c, d)
#define M5(n, a, b, c, d, e) i == (5+n) && __M5(n, a, b, c, d, e)
#define M6(n, a, b, c, d, e, f) i == (6+n) && __M6(n, a, b, c, d, e, f)
#define M7(n, a, b, c, d, e, f, g) i == (7+n) && __M7(n, a, b, c, d, e, f, g)
#define M8(n, a, b, c, d, e, f, g, h) i == (8+n) && __M8(n, a, b, c, d, e, f, g, h)

bool_t
lex(lexer_t *self)
{
  src_t *src;
  char_t peek;
  usize_t i;
  tok_t tok;

  if (srcs_size(&self->srcs) == 0) {
    return false;
  }
  src = srcs_offset(&self->srcs, 0);
  peek = src_peek(src, 0);
  init(&tok, tok_t);
  while (peek == ' ' || peek == '\t') {
    ++tok.lws;
    peek = src_next(src);
  }
  if (peek == '_' || isalpha(peek)) {
    char s[256];

    tok.loc = src->loc;
    i = 0;
    do {
      s[i++] = peek;
      peek = src_next(src);
    } while (peek == '_' || isalpha(peek));
    s[i] = '\0';
    tok.id = C_TOK_IDENTIFIER;
    switch (s[0]) {
      case 'a': if (M3(1, 'u', 't', 'o')) tok.id = C_TOK_AUTO;
        break;
      case 'b': if (M4(1, 'r', 'e', 'a', 'k')) tok.id = C_TOK_BREAK;
        break;
      case 'c':
        if (M3(1, 'a', 's', 'e')) tok.id = C_TOK_CASE;
        else if (M3(1, 'h', 'a', 'r')) tok.id = C_TOK_CHAR;
        else if (i >= 4 && s[1] == 'o' && s[2] == 'n') {
          if (M2(3, 's', 't')) tok.id = C_TOK_CONST;
          else if (M5(3, 't', 'i', 'n', 'u', 'e')) tok.id = C_TOK_CONTINUE;
        }
        break;
      case 'd':
        if (M6(1, 'e', 'f', 'a', 'u', 'l', 't')) tok.id = C_TOK_DEFAULT;
        else if (i > 1 && s[1] == 'o') {
          if (i == 2) tok.id = C_TOK_DO;
          else if (M4(2, 'u', 'b', 'l', 'e')) tok.id = C_TOK_DOUBLE;
        }
        break;
      case 'e':
        if (M3(1, 'l', 's', 'e')) tok.id = C_TOK_ELSE;
        else if (M3(1, 'n', 'u', 'm')) tok.id = C_TOK_ENUM;
        else if (M5(1, 'x', 't', 'e', 'r', 'n')) tok.id = C_TOK_EXTERN;
        break;
      case 'f':
        if (M4(1, 'l', 'o', 'a', 't')) tok.id = C_TOK_FLOAT;
        else if (M2(1, 'o', 'r')) tok.id = C_TOK_FOR;
        break;
      case 'g':if (M3(1, 'o', 't', 'o')) tok.id = C_TOK_GOTO;
        break;
      case 'i':
        if (M1(1, 'f')) tok.id = C_TOK_IF;
        else if (M5(1, 'n', 'l', 'i', 'n', 'e')) tok.id = C_TOK_INLINE;
        else if (M2(1, 'n', 't')) tok.id = C_TOK_INT;
        break;
      case 'l':if (M3(1, 'o', 'n', 'g')) tok.id = C_TOK_LONG;
        break;
      case 'r':
        if (i > 4 && s[1] == 'e') {
          if (M6(2, 'g', 'i', 's', 't', 'e', 'r'))
            tok.id = C_TOK_REGISTER;
          else if (M6(2, 's', 't', 'r', 'i', 'c', 't'))
            tok.id = C_TOK_RESTRICT;
          else if (M4(2, 't', 'u', 'r', 'n')) tok.id = C_TOK_RETURN;
        }
        break;
      case 's':
        if (M4(1, 'h', 'o', 'r', 't')) tok.id = C_TOK_SHORT;
        else if (i == 6) {
          switch (s[1]) {
            case 'i':
              if (__M4(2, 'g', 'n', 'e', 'd')) tok.id = C_TOK_SIGNED;
              else if (__M4(2, 'z', 'e', 'o', 'f')) tok.id = C_TOK_SIZEOF;
              break;
            case 't':
              if (__M4(2, 'a', 't', 'i', 'c')) tok.id = C_TOK_STATIC;
              else if (__M4(2, 'r', 'u', 'c', 't')) tok.id = C_TOK_STRUCT;
              break;
            case 'w':
              if (__M4(2, 'i', 't', 'c', 'h'))
                tok.id = C_TOK_SWITCH;
              break;
            default:break;
          }
        }
        break;
      case 't':
        if (M6(1, 'y', 'p', 'e', 'd', 'e', 'f')) tok.id = C_TOK_TYPEDEF;
        break;
      case 'u':
        if (i > 4 && s[1] == 'n') {
          if (M3(2, 'i', 'o', 'n')) tok.id = C_TOK_UNION;
          else if (M6(2, 's', 'i', 'g', 'n', 'e', 'd'))
            tok.id = C_TOK_UNSIGNED;
        }
        break;
      case 'v':
        if (i > 3 && s[1] == 'o') {
          if (M2(2, 'i', 'd')) tok.id = C_TOK_VOID;
          else if (M6(2, 'l', 'a', 't', 'i', 'l', 'e'))
            tok.id = C_TOK_VOLATILE;
        }
        break;
      case 'w':if (M4(1, 'h', 'i', 'l', 'e')) tok.id = C_TOK_WHILE;
        break;
      case '_':
        switch (s[1]) {
          case 'A':
            if (i == 8 && __M4(2, 'l', 'i', 'g', 'n')) {
              if (__M2(6, 'a', 's')) tok.id = C_TOK_ALIGNAS;
              else if (__M2(6, 'o', 'f')) tok.id = C_TOK_ALIGNOF;
            } else if (M5(2, 't', 'o', 'm', 'i', 'c'))
              tok.id = C_TOK_ATOMIC;
            break;
          case 'B':if (M3(2, 'o', 'o', 'l')) tok.id = C_TOK_BOOL;
            break;
          case 'C':
            if (M6(2, 'o', 'm', 'p', 'l', 'e', 'x'))
              tok.id = C_TOK_COMPLEX;
            break;
          case 'G':
            if (M6(2, 'e', 'n', 'e', 'r', 'i', 'c'))
              tok.id = C_TOK_GENERIC;
            break;
          case 'I':
            if (M8(2, 'm', 'a', 'g', 'i', 'n', 'a', 'r', 'y'))
              tok.id = C_TOK_IMAGINARY;
            break;
          case 'N':
            if (M7(2, 'o', 'r', 'e', 't', 'u', 'r', 'n'))
              tok.id = C_TOK_NORETURN;
            break;
          case 'S':
            if (i == 14 && __M6(2, 't', 'a', 't', 'i', 'c', '_')
              && __M6(8, 'a', 's', 's', 'e', 'r', 't'))
              tok.id = C_TOK_STATIC_ASSERT;
            break;
          case 'T':
            if (i == 13 && __M6(2, 'h', 'r', 'e', 'a', 'd', '_')
              && __M5(8, 'l', 'o', 'c', 'a', 'l'))
              tok.id = C_TOK_THREAD_LOCAL;
            break;
          case '_':
            if (M6(2, 'f', 'u', 'n', 'c', '_', '_'))
              tok.id = C_TOK_FUNC_NAME;
          default: break;
        }
      default: break;
    }
    if (tok.id == C_TOK_IDENTIFIER) {
      val_t val;

      val_init_ident(&val, s);
      tok.kind = TOK_VALUE;
      tok.id = self->vals.len;
      vals_push(&self->vals, val);
    } else {
      tok.kind = TOK_KEYWORD;
    }
    toks_push(&self->toks, tok);
    return true;
  }
  return false;
}

i32_t
main(void)
{
  lexer_t lexer;

  init(&lexer, lexer_t);
  lexer_init_str(&lexer, " auto int double foo bar        float");
  while (lex(&lexer)) {
    tok_t tok;

    if (toks_shift(&lexer.toks, &tok)) {
      switch (tok.kind) {
        case TOK_KEYWORD:
          switch (tok.id) {
            case C_TOK_AUTO: puts("auto");
              break;
            case C_TOK_INT: puts("int");
              break;
            case C_TOK_DOUBLE: puts("double");
              break;
            case C_TOK_FLOAT: puts("float");
              break;
            default: break;
          }
          break;
        case TOK_VALUE: {
          val_t *val;

          val = vals_offset(&lexer.vals, tok.id);
          if (val->kind == VAL_IDENT) {
            puts(val->val.ident.buf);
          }
          break;
        }
        default: break;
      }
    }
  }
  if (lexer.errs.len) {
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
