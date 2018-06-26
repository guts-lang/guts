/*
 * MIT License
 *
 * Copyright (c) 2016-2018 Abel Lucas <www.github.com/uael>
 * Copyright (c) 2016-2018 Guts <www.github.com/guts-lang>
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

#include "guts/hir/parser.h"

#include "test.h"

int main(void)
{
	codemap_t codemap;
	hir_parser_t parser;
	hir_tok_t *tok;
	static char __const *SRC =
		"begin\n"
  		"u8 main()\n"
		"{\n"
		"    return 0;\n"
		"}\n";
	static char __const *SRC1 =
		"u8 lol();\n";
	static char __const *SRC2 =
		"lol();\n";

	codemap_init(&codemap, NULL);
	codemap_src_push(&codemap, SRC, true);
	hir_parser_init(&parser, &codemap, NULL);

	ASSERT(hir_parser_consume(&parser, HIR_TOK_IDENT));
	hir_parser_include(&parser, SRC1, true);
	ASSERT(hir_parser_consume(&parser, HIR_TOK_U8));
	ASSERT(hir_parser_consume(&parser, HIR_TOK_IDENT));
	ASSERT(hir_parser_consume(&parser, HIR_TOK_LPAR));
	ASSERT(hir_parser_consume(&parser, HIR_TOK_RPAR));
	ASSERT(hir_parser_consume(&parser, HIR_TOK_SEMICOLON));
	ASSERT(hir_parser_consume(&parser, HIR_TOK_U8));
	ASSERT(hir_parser_consume(&parser, HIR_TOK_IDENT));
	ASSERT(hir_parser_consume(&parser, HIR_TOK_LPAR));
	ASSERT(hir_parser_consume(&parser, HIR_TOK_RPAR));
	ASSERT(hir_parser_consume(&parser, HIR_TOK_LCUR));
	ASSERT((tok = hir_parser_peekn(&parser, 2)));
	ASSERT_EQ(HIR_TOK_SEMICOLON, tok->kind);
	hir_parser_include(&parser, SRC2, true);
	ASSERT((tok = hir_parser_peekn(&parser, 2)));
	ASSERT_EQ(HIR_TOK_RPAR, tok->kind);
	ASSERT((tok = hir_parser_peekn(&parser, 5)));
	ASSERT_EQ(HIR_TOK_LIT_NUMBER, tok->kind);
	ASSERT(hir_parser_consume(&parser, HIR_TOK_IDENT));
	ASSERT(hir_parser_consume(&parser, HIR_TOK_LPAR));
	ASSERT(hir_parser_consume(&parser, HIR_TOK_RPAR));
	ASSERT(hir_parser_consume(&parser, HIR_TOK_SEMICOLON));
	ASSERT(hir_parser_consume(&parser, HIR_TOK_RETURN));
	ASSERT(hir_parser_consume(&parser, HIR_TOK_LIT_NUMBER));
	ASSERT(hir_parser_consume(&parser, HIR_TOK_SEMICOLON));
	ASSERT(hir_parser_consume(&parser, HIR_TOK_RCUR));
	ASSERT(hir_parser_consume(&parser, HIR_TOK_EOF));

	hir_parser_include(&parser, SRC, true);
	ASSERT_NULL(hir_parser_consume(&parser, HIR_TOK_U8));

	codemap_emit(&codemap, stdout);
	codemap_dtor(&codemap);
	hir_parser_dtor(&parser);
	return 0;
}
