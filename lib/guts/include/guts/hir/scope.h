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

/*!@file guts/hir/scope.h
 * @author uael
 *
 * @addtogroup guts.hir @{
 */
#ifndef __GUTS_HIR_SCOPE_H
# define __GUTS_HIR_SCOPE_H

#include <ds/map.h>

#include "token.h"

typedef struct {
	hir_ident_t ident;
	u16_t len;
} hir_name_t;

__api void hir_name_init(hir_name_t *self, hir_tok_t const *tok);

struct hir_entity;

typedef struct hir_scope {
	struct hir_scope *parent;
	struct hir_entity *entity;
	mapof(hir_name_t *, struct hir_entity *) childs;
} hir_scope_t;

__api void hir_scope_init(hir_scope_t *self, struct hir_scope *parent,
						  struct hir_entity *entity);
__api struct hir_entity *hir_scope_add(hir_scope_t *self, struct hir_entity *e);
__api struct hir_entity *hir_scope_find(hir_scope_t *self, hir_name_t *name);
__api struct hir_entity *hir_scope_locate(hir_scope_t *self, hir_ident_t id,
										  span_t span);

#endif /* !__GUTS_HIR_SCOPE_H */
/*!@} */
