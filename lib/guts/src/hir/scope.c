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

#include <guts/hir/scope.h>
#include <il/span.h>
#include "guts/hir/scope.h"
#include "guts/hir/entity.h"

#if 0
static bool __eq_name(u8_t const *a, u8_t const *b)
{
	hir_name_t *na, *nb;

	na = (hir_name_t *) a;
	nb = (hir_name_t *) b;
	return (bool)(na->len == nb->len && !memcmp(na->ident, nb->ident, na->len));
}

static u32_t __hash_name(u8_t const *key)
{
	hir_name_t *nkey;

	nkey = (hir_name_t *) key;
	return hash_strn((const u8_t *) nkey->ident, nkey->len);
}

FORCEINLINE
void hir_scope_init(hir_scope_t *self, struct hir_scope *parent,
					struct hir_entity *entity)
{
	self->parent = parent;
	self->entity = entity;
	mapinit(&self->childs, __eq_name, __hash_name);
}

hir_ent_t *hir_scope_add(hir_scope_t *self, hir_ent_t *e)
{
	assert(e);

	if (maphas(&self->childs, &e->name))
		return NULL;
	e = memcpy(malloc(sizeof(hir_ent_t)), e, sizeof(hir_ent_t));
	mapput(&self->childs, &e->name, e);
	return e;
}

hir_ent_t *hir_scope_find(hir_scope_t *self, hir_name_t *name)
{
	u32_t idx;

	if (!self)
		return NULL;
	if ((idx = mapget(&self->childs, name)) != U32_MAX)
		return self->childs.entries[idx].val;
	return hir_scope_find(self->parent, name);
}

FORCEINLINE
hir_ent_t *hir_scope_locate(hir_scope_t *self, hir_ident_t id,
							   span_t span)
{
	hir_name_t name;

	name.ident = id;
	name.len = span.length;
	return hir_scope_find(self, &name);
}
#endif
