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

/*!@file guts/hir/entity.h
 * @author uael
 *
 * @addtogroup guts.hir @{
 */
#ifndef __GUTS_HIR_ENTITY_H
# define __GUTS_HIR_ENTITY_H

#include "stmt.h"

typedef struct hir_entity {

	enum {
		HIR_ENTITY_INCLUDE = 1 << 0,
		HIR_ENTITY_USE = 1 << 1,
		HIR_ENTITY_NAMESPACE = 1 << 2,
		HIR_ENTITY_FUNCTION = 1 << 3,
		HIR_ENTITY_STRUCTURE = 1 << 4,
		HIR_ENTITY_FIELD = 1 << 5,
		HIR_ENTITY_VARIABLE = 1 << 6,
	} kind;

	span_t span;
	hir_name_t name;
	struct hir_entity *parent;

	union {
		struct {
			hir_scope_t scope;
		} namespace;

		struct {
			hir_scope_t params;
			hir_ty_t ret_ty;
			hir_stmt_t stmt;
		} function;

		struct {
			hir_scope_t members;
		} structure;

		struct {
			hir_ty_t ty;
			u16_t slot;
		} field;

		struct {
			hir_ty_t ty;
		} variable;
	};

} hir_entity_t;

#define HIR_ENTITY_SCOPED \
	(HIR_ENTITY_ROOT | HIR_ENTITY_NAMESPACE | HIR_ENTITY_FUNCTION | \
		HIR_ENTITY_STRUCTURE)
#define HIR_ENTITY_TYPED \
	(HIR_ENTITY_FUNCTION | HIR_ENTITY_FIELD | HIR_ENTITY_VARIABLE)

#endif /* !__GUTS_HIR_ENTITY_H */
/*!@} */
