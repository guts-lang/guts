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

#include "guts/hir/scope.h"
#include "guts/hir/entity.h"

#include "test.h"

int main(void)
{
	codemap_t codemap;
	hir_parser_t parser;
	hir_tok_t *tok;
	hir_scope_t scope;
	hir_entity_t entity, *entities[10], *foo, *bar, *a, *baz, *blu, *b, *fla,
		*olk, *c, *lob;
	u8_t idx = 0;
	static char __const *SRC =
		"foo bar {a baz } blu {b fla olk {c lob } }\n";

	codemap_init(&codemap, NULL);
	codemap_src_push(&codemap, SRC, true);
	hir_parser_init(&parser, &codemap, &scope);

	while ((tok = hir_parser_next(&parser)) && tok->kind != HIR_TOK_EOF) {
		if (tok->kind == HIR_TOK_LCUR) {
			tok = hir_parser_consume(&parser, HIR_TOK_IDENT);
			bzero(&entity, sizeof(hir_entity_t));
			entity.span = tok->span;
			entity.name.len = tok->span.length;
			entity.name.ident = tok->ident;
			entity.parent = parser.scope->entity;
			ASSERT(entities[idx++] = hir_scope_add(parser.scope, &entity));
			hir_scope_init(&entities[idx - 1]->namespace.scope, parser.scope,
				entities[idx - 1]);
			parser.scope = &entities[idx - 1]->namespace.scope;
		} else if (tok->kind == HIR_TOK_RCUR) {
			parser.scope = parser.scope->parent;
		} else if (tok->kind == HIR_TOK_IDENT) {
			bzero(&entity, sizeof(hir_entity_t));
			entity.span = tok->span;
			entity.name.len = tok->span.length;
			entity.name.ident = tok->ident;
			entity.parent = parser.scope->entity;
			ASSERT(entities[idx++] = hir_scope_add(parser.scope, &entity));
		}
	}

	ASSERT(foo = hir_scope_find(&scope, &(hir_name_t){ "foo", 3 }));
	ASSERT(bar = hir_scope_find(&scope, &(hir_name_t){ "bar", 3 }));
	ASSERT(blu = hir_scope_find(&scope, &(hir_name_t){ "blu", 3 }));
	ASSERT(a = hir_scope_find(&scope, &(hir_name_t){ "a", 1 }));
	ASSERT(b = hir_scope_find(&scope, &(hir_name_t){ "b", 1 }));

	ASSERT_NULL(hir_scope_find(&scope, &(hir_name_t){ "baz", 3 }));
	ASSERT_NULL(hir_scope_find(&scope, &(hir_name_t){ "fla", 3 }));
	ASSERT_NULL(hir_scope_find(&scope, &(hir_name_t){ "olk", 3 }));
	ASSERT_NULL(hir_scope_find(&scope, &(hir_name_t){ "lob", 3 }));
	ASSERT_NULL(hir_scope_find(&scope, &(hir_name_t){ "c", 1 }));

	ASSERT(baz = hir_scope_find(&a->namespace.scope,
		&(hir_name_t){ "baz", 3 }));
	ASSERT(fla = hir_scope_find(&b->namespace.scope,
		&(hir_name_t){ "fla", 3 }));
	ASSERT(olk = hir_scope_find(&b->namespace.scope,
		&(hir_name_t){ "olk", 3 }));
	ASSERT(c = hir_scope_find(&b->namespace.scope,
		&(hir_name_t){ "c", 1 }));
	ASSERT(lob = hir_scope_find(&c->namespace.scope,
		&(hir_name_t){ "lob", 3 }));
	ASSERT(hir_scope_find(&c->namespace.scope, &(hir_name_t){ "fla", 3 }));
	ASSERT(hir_scope_find(&c->namespace.scope, &(hir_name_t){ "blu", 3 }));

	//TODO: cleanup..

	codemap_emit(&codemap, stdout);
	codemap_dtor(&codemap);
	hir_parser_dtor(&parser);
	return 0;
}
