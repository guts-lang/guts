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

#include "guts/hir/fe.h"

#include "test.h"

int main(void)
{
	source_t *src;
	codemap_t fe;
	hir_lexer_t lexer;
	hir_tok_t *tok;
	static char __const *SRC =
		"u8 main()\n"
		"{\n"
		"    return 0;\n"
		"}\n";

	codemap_init(&fe, NULL);
	src = codemap_src_push(&fe, SRC, true);

	hir_lexer_init(&lexer, src, &fe.diagnostics);

	ASSERT(tok = hir_lexer_next(&lexer));
	ASSERT_EQ(HIR_TOK_U8, tok->kind);
	ASSERT_EQ(2, tok->span.length);
	ASSERT_EQ(1, tok->span.start.col);
	ASSERT_EQ(1, tok->span.start.raw);
	ASSERT_EQ(0, tok->span.start.off);

	ASSERT(tok = hir_lexer_next(&lexer));
	ASSERT_EQ(HIR_TOK_IDENT, tok->kind);
	ASSERT_EQ(4, tok->span.length);
	ASSERT_EQ(4, tok->span.start.col);
	ASSERT_EQ(1, tok->span.start.raw);
	ASSERT_EQ(3, tok->span.start.off);
	ASSERT_EQ(0, memcmp("main", tok->ident, tok->span.length));

	ASSERT(tok = hir_lexer_next(&lexer));
	ASSERT_EQ(HIR_TOK_LPAR, tok->kind);
	ASSERT_EQ(1, tok->span.length);
	ASSERT_EQ(8, tok->span.start.col);
	ASSERT_EQ(1, tok->span.start.raw);
	ASSERT_EQ(7, tok->span.start.off);

	ASSERT(tok = hir_lexer_next(&lexer));
	ASSERT_EQ(HIR_TOK_RPAR, tok->kind);
	ASSERT_EQ(1, tok->span.length);
	ASSERT_EQ(9, tok->span.start.col);
	ASSERT_EQ(1, tok->span.start.raw);
	ASSERT_EQ(8, tok->span.start.off);

	ASSERT(tok = hir_lexer_next(&lexer));
	ASSERT_EQ(HIR_TOK_LCUR, tok->kind);
	ASSERT_EQ(1, tok->span.length);
	ASSERT_EQ(1, tok->span.start.col);
	ASSERT_EQ(2, tok->span.start.raw);
	ASSERT_EQ(10, tok->span.start.off);

	ASSERT(tok = hir_lexer_next(&lexer));
	ASSERT_EQ(HIR_TOK_RETURN, tok->kind);
	ASSERT_EQ(6, tok->span.length);
	ASSERT_EQ(5, tok->span.start.col);
	ASSERT_EQ(3, tok->span.start.raw);
	ASSERT_EQ(16, tok->span.start.off);

	ASSERT(tok = hir_lexer_next(&lexer));
	ASSERT_EQ(HIR_TOK_LIT_INTEGER, tok->kind);
	ASSERT_EQ(1, tok->span.length);
	ASSERT_EQ(12, tok->span.start.col);
	ASSERT_EQ(3, tok->span.start.raw);
	ASSERT_EQ(23, tok->span.start.off);
	ASSERT_EQ(0, memcmp("0", tok->lit_integer.number, tok->span.length));

	ASSERT(tok = hir_lexer_next(&lexer));
	ASSERT_EQ(HIR_TOK_SEMICOLON, tok->kind);
	ASSERT_EQ(1, tok->span.length);
	ASSERT_EQ(13, tok->span.start.col);
	ASSERT_EQ(3, tok->span.start.raw);
	ASSERT_EQ(24, tok->span.start.off);

	ASSERT(tok = hir_lexer_next(&lexer));
	ASSERT_EQ(HIR_TOK_RCUR, tok->kind);
	ASSERT_EQ(1, tok->span.length);
	ASSERT_EQ(1, tok->span.start.col);
	ASSERT_EQ(4, tok->span.start.raw);
	ASSERT_EQ(26, tok->span.start.off);

	ASSERT(tok = hir_lexer_next(&lexer));
	ASSERT_EQ(HIR_TOK_EOF, tok->kind);
	ASSERT_EQ(1, tok->span.length);
	ASSERT_EQ(1, tok->span.start.col);
	ASSERT_EQ(5, tok->span.start.raw);
	ASSERT_EQ(28, tok->span.start.off);

	ASSERT_NULL(hir_lexer_next(&lexer));

	hir_lexer_dtor(&lexer);

	codemap_emit(&fe, stdout);
	codemap_dtor(&fe);
	hir_lexer_dtor(&lexer);
	return 0;
}
