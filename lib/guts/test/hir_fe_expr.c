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

#include <il/codemap.h>
#include <guts/hir/expr.h>
#include <il/span.h>

#include "guts/hir/expr.h"

#include "test.h"

int main(void)
{
	source_t *src;
	codemap_t fe;
	hir_lexer_t lexer;
	static char __const *SRC =
		"[la, lol, foo, bar]\n";

	codemap_init(&fe, NULL);
	src = codemap_src_push(&fe, SRC, true);

	hir_lexer_init(&lexer, src, &fe.diagnostics);

	hir_expr_t *expr = hir_expr_parse(&lexer);
	ASSERT(expr);
	ASSERT_EQ(HIR_EXPR_ARRAY, expr->kind);
	ASSERT_EQ(4, veclen(expr->array.elems));

	ASSERT_EQ(HIR_EXPR_IDENT, expr->array.elems[0]->kind);
	ASSERT_EQ(0, memcmp(expr->array.elems[0]->ident, "la",
		expr->array.elems[0]->span.length));

	ASSERT_EQ(HIR_EXPR_IDENT, expr->array.elems[1]->kind);
	ASSERT_EQ(0, memcmp(expr->array.elems[1]->ident, "lol",
		expr->array.elems[1]->span.length));

	ASSERT_EQ(HIR_EXPR_IDENT, expr->array.elems[2]->kind);
	ASSERT_EQ(0, memcmp(expr->array.elems[2]->ident, "foo",
		expr->array.elems[2]->span.length));

	ASSERT_EQ(HIR_EXPR_IDENT, expr->array.elems[3]->kind);
	ASSERT_EQ(0, memcmp(expr->array.elems[3]->ident, "bar",
		expr->array.elems[3]->span.length));

	//TODO: dtor expr

	hir_lexer_dtor(&lexer);

	codemap_emit(&fe, stdout);
	codemap_dtor(&fe);
	hir_lexer_dtor(&lexer);
	return 0;
}
