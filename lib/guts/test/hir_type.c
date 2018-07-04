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
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, typeESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "guts/hir/type.h"

#include "test.h"

int main(void)
{
	codemap_t codemap;
	hir_parser_t parser;
	hir_ty_t type;

	codemap_init(&codemap, NULL);
	codemap_src_push(&codemap,
		"() : (u8, <bool, char, [?const u8; 5]>): [*u8]\n", true);
	hir_parser_init(&parser, &codemap, NULL);

	ASSERT_EQ(PARSE_OK, hir_ty_consume(&type, &parser));
	ASSERT_EQ(HIR_TY_LAMBDA, type.kind);
	ASSERT_EQ(0, veclen(type.ty_lambda.inputs));
	ASSERT_EQ(HIR_TY_LAMBDA, type.ty_lambda.output->kind);
	ASSERT_EQ(2, veclen(type.ty_lambda.output->ty_lambda.inputs));

	hir_ty_destroy(&type);
	codemap_emit(&codemap, stdout);
	codemap_dtor(&codemap);
	hir_parser_dtor(&parser);
	return 0;
}
