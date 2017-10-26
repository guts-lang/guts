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

bool_t
c_tok_parse_ident(tok_t *self, char_t peek, val_t *val, src_t *src)
{
  char_t str[256];
  u8_t len;

  if (peek == '_' || isalpha(peek)) {
    len = 0;
    do {
      str[len++] = peek;
      peek = src_next(src);
    } while (peek == '_' || isalnum(peek));
    str[len] = '\0';
    switch (str[0]) {
      case 'a':
        if (MSTRN(str, len, 1, 'u', 't', 'o'))
          self->id = C_TOK_AUTO;
        break;
      case 'b':
        if (MSTRN(str, len, 1, 'r', 'e', 'a', 'k'))
          self->id = C_TOK_BREAK;
        break;
      case 'c':
        if (MSTRN(str, len, 1, 'a', 's', 'e'))
          self->id = C_TOK_CASE;
        else if (MSTRN(str, len, 1, 'h', 'a', 'r'))
          self->id = C_TOK_CHAR;
        else if (len >= 4 && str[1] == 'o' && str[2] == 'n') {
          if (MSTRN(str, len, 3, 's', 't'))
            self->id = C_TOK_CONST;
          else if (MSTRN(str, len, 3, 't', 'i', 'n', 'u', 'e'))
            self->id = C_TOK_CONTINUE;
        }
        break;
      case 'd':
        if (MSTRN(str, len, 1, 'e', 'f', 'a', 'u', 'l', 't'))
          self->id = C_TOK_DEFAULT;
        else if (len > 1 && str[1] == 'o') {
          if (len == 2)
            self->id = C_TOK_DO;
          else if (MSTRN(str, len, 2, 'u', 'b', 'l', 'e'))
            self->id = C_TOK_DOUBLE;
        }
        break;
      case 'e':
        if (MSTRN(str, len, 1, 'l', 's', 'e'))
          self->id = C_TOK_ELSE;
        else if (MSTRN(str, len, 1, 'n', 'u', 'm'))
          self->id = C_TOK_ENUM;
        else if (MSTRN(str, len, 1, 'x', 't', 'e', 'r', 'n'))
          self->id = C_TOK_EXTERN;
        break;
      case 'f':
        if (MSTRN(str, len, 1, 'l', 'o', 'a', 't'))
          self->id = C_TOK_FLOAT;
        else if (MSTRN(str, len, 1, 'o', 'r'))
          self->id = C_TOK_FOR;
        break;
      case 'g':if (MSTRN(str, len, 1, 'o', 't', 'o'))
          self->id = C_TOK_GOTO;
        break;
      case 'i':
        if (MSTRN(str, len, 1, 'f'))
          self->id = C_TOK_IF;
        else if (MSTRN(str, len, 1, 'n', 'l', 'i', 'n', 'e'))
          self->id = C_TOK_INLINE;
        else if (MSTRN(str, len, 1, 'n', 't'))
          self->id = C_TOK_INT;
        break;
      case 'l':if (MSTRN(str, len, 1, 'o', 'n', 'g'))
          self->id = C_TOK_LONG;
        break;
      case 'r':
        if (len > 4 && str[1] == 'e') {
          if (MSTRN(str, len, 2, 'g', 'i', 's', 't', 'e', 'r'))
            self->id = C_TOK_REGISTER;
          else if (MSTRN(str, len, 2, 's', 't', 'r', 'i', 'c', 't'))
            self->id = C_TOK_RESTRICT;
          else if (MSTRN(str, len, 2, 't', 'u', 'r', 'n'))
            self->id = C_TOK_RETURN;
        }
        break;
      case 's':
        if (MSTRN(str, len, 1, 'h', 'o', 'r', 't'))
          self->id = C_TOK_SHORT;
        else if (len == 6) {
          switch (str[1]) {
            case 'i':
              if (MSTR(str, 2, 'g', 'n', 'e', 'd'))
                self->id = C_TOK_SIGNED;
              else if (MSTR(str, 2, 'z', 'e', 'o', 'f'))
                self->id = C_TOK_SIZEOF;
              break;
            case 't':
              if (MSTR(str, 2, 'a', 't', 'i', 'c'))
                self->id = C_TOK_STATIC;
              else if (MSTR(str, 2, 'r', 'u', 'c', 't'))
                self->id = C_TOK_STRUCT;
              break;
            case 'w':
              if (MSTR(str, 2, 'i', 't', 'c', 'h'))
                self->id = C_TOK_SWITCH;
              break;
            default:break;
          }
        }
        break;
      case 't':
        if (MSTRN(str, len, 1, 'y', 'p', 'e', 'd', 'e', 'f'))
          self->id = C_TOK_TYPEDEF;
        break;
      case 'u':
        if (len > 4 && str[1] == 'n') {
          if (MSTRN(str, len, 2, 'i', 'o', 'n'))
            self->id = C_TOK_UNION;
          else if (MSTRN(str, len, 2, 's', 'i', 'g', 'n', 'e', 'd'))
            self->id = C_TOK_UNSIGNED;
        }
        break;
      case 'v':
        if (len > 3 && str[1] == 'o') {
          if (MSTRN(str, len, 2, 'i', 'd'))
            self->id = C_TOK_VOID;
          else if (MSTRN(str, len, 2, 'l', 'a', 't', 'i', 'l', 'e'))
            self->id = C_TOK_VOLATILE;
        }
        break;
      case 'w':if (MSTRN(str, len, 1, 'h', 'i', 'l', 'e'))
          self->id = C_TOK_WHILE;
        break;
      case '_':
        switch (str[1]) {
          case 'A':
            if (len == 8 && MSTR(str, 2, 'l', 'i', 'g', 'n')) {
              if (MSTR(str, 6, 'a', 's'))
                self->id = C_TOK_ALIGNAS;
              else if (MSTR(str, 6, 'o', 'f'))
                self->id = C_TOK_ALIGNOF;
            } else if (MSTRN(str, len, 2, 't', 'o', 'm', 'i', 'c'))
              self->id = C_TOK_ATOMIC;
            break;
          case 'B':if (MSTRN(str, len, 2, 'o', 'o', 'l'))
              self->id = C_TOK_BOOL;
            break;
          case 'C':
            if (MSTRN(str, len, 2, 'o', 'm', 'p', 'l', 'e', 'x'))
              self->id = C_TOK_COMPLEX;
            break;
          case 'G':
            if (MSTRN(str, len, 2, 'e', 'n', 'e', 'r', 'i', 'c'))
              self->id = C_TOK_GENERIC;
            break;
          case 'I':
            if (MSTRN(str, len, 2, 'm', 'a', 'g', 'i', 'n', 'a', 'r', 'y'))
              self->id = C_TOK_IMAGINARY;
            break;
          case 'N':
            if (MSTRN(str, len, 2, 'o', 'r', 'e', 't', 'u', 'r', 'n'))
              self->id = C_TOK_NORETURN;
            break;
          case 'S':
            if (len == 14 && MSTR(str, 2, 't', 'a', 't', 'i', 'c', '_')
              && MSTR(str, 8, 'a', 's', 's', 'e', 'r', 't'))
              self->id = C_TOK_STATIC_ASSERT;
            break;
          case 'T':
            if (len == 13 && MSTR(str, 2, 'h', 'r', 'e', 'a', 'd', '_')
              && MSTR(str, 8, 'l', 'o', 'c', 'a', 'l'))
              self->id = C_TOK_THREAD_LOCAL;
            break;
          case '_':
            if (MSTRN(str, len, 2, 'f', 'u', 'n', 'c', '_', '_'))
              self->id = C_TOK_FUNC_NAME;
          default: break;
        }
        break;
      default: break;
    }
    if (self->id == 0) {
      val_init_ident(val, str);
      self->kind = TOK_VALUE;
    } else {
      self->kind = TOK_KEYWORD;
    }
    return true;
  }
  return false;
}

bool_t
c_tok_parse_number(tok_t *self, char_t peek, val_t *val, src_t *src)
{
  char_t str[256];
  u8_t len;
  bool_t floating;

  if (isdigit(peek) || (peek == '.' && isdigit(src_peek(src, 1)))) {
    floating = peek == '.';
    str[len = 0] = peek;
    while (true) {
      if (isdigit(peek = src_next(src))) {
        str[++len] = peek;
      } else if (!floating && peek == '.') {
        floating = true;
        str[++len] = peek;
      } else {
        str[++len] = '\0';
        break;
      }
    }
    self->kind = TOK_VALUE;
    if (floating) {
      val_init_f64(val, atof(str));
    } else {
      val_init_i64(val, atoll(str));
    }
    return true;
  }
  return false;
}

bool_t
c_tok_parse_syn(tok_t *self, char_t peek, val_t *val, src_t *src)
{
  (void) val;
  switch (peek) {
    case '!':
      if (src_peek(src, 1) == '=') {
        self->id = C_TOK_NE_OP;
        self->kind = TOK_PONCT;
        break;
      }
      self->id = '!';
      break;
    case '#':
      if (src_peek(src, 1) == '#') {
        self->id = C_TOK_TOKEN_PASTE;
        self->kind = TOK_PONCT;
        break;
      }
      self->id = '#';
      break;
    case '%':
      if (src_peek(src, 1) == '=') {
        self->id = C_TOK_MOD_ASSIGN;
        self->kind = TOK_PONCT;
        break;
      }
      self->id = '%';
      break;
    case '&':
      if (src_peek(src, 1) == '&') {
        self->id = C_TOK_LOGICAL_AND;
        self->kind = TOK_PONCT;
        break;
      }
      if (src_peek(src, 1) == '=') {
        self->id = C_TOK_AND_ASSIGN;
        self->kind = TOK_PONCT;
        break;
      }
      self->id = '&';
      break;
    case '(':
      self->id = '(';
      break;
    case ')':
      self->id = ')';
      break;
    case '*':
      if (src_peek(src, 1) == '=') {
        self->id = C_TOK_MUL_ASSIGN;
        self->kind = TOK_PONCT;
        break;
      }
      self->id = '*';
      break;
    case '+':
      if (src_peek(src, 1) == '=') {
        self->id = C_TOK_PLUS_ASSIGN;
        self->kind = TOK_PONCT;
        break;
      }
      if (src_peek(src, 1) == '+') {
        self->id = C_TOK_INCREMENT;
        self->kind = TOK_PONCT;
        break;
      }
      self->id = '+';
      break;
    case ',':
      self->id = ',';
      break;
    case '-':
      if (src_peek(src, 1) == '=') {
        self->id = C_TOK_MINUS_ASSIGN;
        self->kind = TOK_PONCT;
        break;
      }
      if (src_peek(src, 1) == '-') {
        self->id = C_TOK_DECREMENT;
        self->kind = TOK_PONCT;
        break;
      }
      if (src_peek(src, 1) == '>') {
        self->id = C_TOK_PTR_OP;
        self->kind = TOK_PONCT;
        break;
      }
      self->id = '-';
      break;
    case '.':
      if (src_peek(src, 1) == '.' && src_peek(src, 2) == '.') {
        self->id = C_TOK_DOTS;
        self->kind = TOK_PONCT;
        break;
      }
      self->id = '.';
      break;
    case '/':
      if (src_peek(src, 1) == '*') {
        while ((peek = src_next(src)) != 0) {
          if (peek == '*') {
            while ((peek = src_next(src)) == '*');
            if (peek == '/') {
              src_next(src);
              break;
            }
          } else if (src_peek(src, 1) == 0) {
            exit(1);
          }
        }
        src_next(src);
        break;
      }
      if (src_peek(src, 1) == '=') {
        self->id = C_TOK_DIV_ASSIGN;
        self->kind = TOK_PONCT;
        break;
      }
      self->id = '/';
      break;
    case ':':
      self->id = ':';
      break;
    case ';':
      self->id = ';';
      break;
    case '<':
      if (src_peek(src, 1) == '<') {
        if (src_peek(src, 2) == '=') {
          self->id = C_TOK_LSHIFT_ASSIGN;
          self->kind = TOK_PONCT;
          break;
        }
        self->id = C_TOK_LEFT_OP;
        self->kind = TOK_PONCT;
        break;
      }
      if (src_peek(src, 1) == '=') {
        self->id = C_TOK_LE_OP;
        self->kind = TOK_PONCT;
        break;
      }
      self->id = '<';
      break;
    case '=':
      if (src_peek(src, 1) == '=') {
        self->id = C_TOK_EQ_OP;
        self->kind = TOK_PONCT;
        break;
      }
      self->id = '=';
      break;
    case '>':
      if (src_peek(src, 1) == '>') {
        if (src_peek(src, 2) == '=') {
          self->id = C_TOK_RSHIFT_ASSIGN;
          self->kind = TOK_PONCT;
          break;
        }
        self->id = C_TOK_RIGHT_OP;
        self->kind = TOK_PONCT;
        break;
      }
      if (src_peek(src, 1) == '=') {
        self->id = C_TOK_GE_OP;
        self->kind = TOK_PONCT;
        break;
      }
      self->id = '>';
      break;
    case '?':
      self->id = '?';
      break;
    case '[':
      self->id = '[';
      break;
    case '\\':
      self->id = '\\';
      break;
    case ']':
      self->id = ']';
      break;
    case '^':
      if (src_peek(src, 1) == '=') {
        self->id = C_TOK_XOR_ASSIGN;
        self->kind = TOK_PONCT;
        break;
      }
      self->id = '^';
      break;
    case '{':
      self->id = '{';
      break;
    case '|':
      if (src_peek(src, 1) == '|') {
        self->id = C_TOK_LOGICAL_OR;
        self->kind = TOK_PONCT;
        break;
      }
      if (src_peek(src, 1) == '=') {
        self->id = C_TOK_OR_ASSIGN;
        self->kind = TOK_PONCT;
        break;
      }
      self->id = '|';
      break;
    case '}':
      self->id = '}';
      break;
    case '~':
      self->id = '~';
      break;
    default:
      src_next(src);
      break;
  }
  if (self->id) {
    src_next(src);
    return true;
  }
  return false;
}

char_t __const *
c_tok_ident_str(tok_t *self)
{
  static char_t __const *idents[] = {
    [C_TOK_AUTO] = "auto",
    [C_TOK_BREAK] = "break",
    [C_TOK_CASE] = "case",
    [C_TOK_CHAR] = "char",
    [C_TOK_CONST] = "const",
    [C_TOK_CONTINUE] = "continue",
    [C_TOK_DEFAULT] = "default",
    [C_TOK_DO] = "do",
    [C_TOK_DOUBLE] = "double",
    [C_TOK_ELSE] = "else",
    [C_TOK_ENUM] = "enum",
    [C_TOK_EXTERN] = "extern",
    [C_TOK_FLOAT] = "float",
    [C_TOK_FOR] = "for",
    [C_TOK_GOTO] = "goto",
    [C_TOK_IF] = "if",
    [C_TOK_INLINE] = "inline",
    [C_TOK_INT] = "int",
    [C_TOK_LONG] = "long",
    [C_TOK_REGISTER] = "register",
    [C_TOK_RESTRICT] = "restrict",
    [C_TOK_RETURN] = "return",
    [C_TOK_SHORT] = "short",
    [C_TOK_SIGNED] = "signed",
    [C_TOK_SIZEOF] = "sizeof",
    [C_TOK_STATIC] = "static",
    [C_TOK_STRUCT] = "struct",
    [C_TOK_SWITCH] = "switch",
    [C_TOK_TYPEDEF] = "typedef",
    [C_TOK_UNION] = "union",
    [C_TOK_UNSIGNED] = "unsigned",
    [C_TOK_VOID] = "void",
    [C_TOK_VOLATILE] = "volatile",
    [C_TOK_WHILE] = "while",
    [C_TOK_ALIGNAS] = "_Alignas",
    [C_TOK_ALIGNOF] = "_Alignof",
    [C_TOK_ATOMIC] = "_Atomic",
    [C_TOK_BOOL] = "_Bool",
    [C_TOK_COMPLEX] = "_Complex",
    [C_TOK_GENERIC] = "_Generic",
    [C_TOK_IMAGINARY] = "_Imaginary",
    [C_TOK_NORETURN] = "_Noreturn",
    [C_TOK_STATIC_ASSERT] = "_Static_assert",
    [C_TOK_THREAD_LOCAL] = "_Thread_local",
    [C_TOK_FUNC_NAME] = "__func__",
    [C_TOK_NE_OP] = "!=",
    [C_TOK_TOKEN_PASTE] = "##",
    [C_TOK_MOD_ASSIGN] = "%=",
    [C_TOK_LOGICAL_AND] = "&&",
    [C_TOK_AND_ASSIGN] = "&=",
    [C_TOK_MUL_ASSIGN] = "*=",
    [C_TOK_PLUS_ASSIGN] = "+=",
    [C_TOK_INCREMENT] = "++",
    [C_TOK_MINUS_ASSIGN] = "-=",
    [C_TOK_DECREMENT] = "--",
    [C_TOK_PTR_OP] = "->",
    [C_TOK_DOTS] = "...",
    [C_TOK_DIV_ASSIGN] = "/=",
    [C_TOK_LSHIFT_ASSIGN] = "<<=",
    [C_TOK_LEFT_OP] = "<<",
    [C_TOK_LE_OP] = "<=",
    [C_TOK_EQ_OP] = "==",
    [C_TOK_RSHIFT_ASSIGN] = ">>=",
    [C_TOK_RIGHT_OP] = ">>",
    [C_TOK_GE_OP] = ">=",
    [C_TOK_XOR_ASSIGN] = "^=",
    [C_TOK_LOGICAL_OR] = "||",
    [C_TOK_OR_ASSIGN] = "|=",
  };

  return idents[self->id];
}

i32_t
main(void)
{
  lexer_t lexer;

  init(&lexer, lexer_t);
  lexer_init_str(&lexer,
    "i32_t\n"
    "main(void)\n"
    "{\n"
    "  return EXIT_SUCCESS;\n"
    "}"
  );
  lrules_push(&lexer.rules, c_tok_parse_ident);
  lrules_push(&lexer.rules, c_tok_parse_number);
  lrules_push(&lexer.rules, c_tok_parse_syn);
  lexer.tok_str = c_tok_ident_str;
  while (lexer_scan(&lexer, 1)) {
    tok_t tok;

    while (toks_shift(&lexer.toks, &tok)) {
      lexer_tok_dump(&lexer, &tok, cout);
      cout_putc(' ');
    }
  }
  lexer_ctor(&lexer);
  cout_flush();
  return EXIT_SUCCESS;
}
