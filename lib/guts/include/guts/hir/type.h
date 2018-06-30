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

/*!@file guts/hir/type.h
 * @author uael
 *
 * @addtogroup guts.hir @{
 */
#ifndef __GUTS_HIR_TYPE_H
# define __GUTS_HIR_TYPE_H

#include "parser.h"

struct hir_expr;

typedef enum {
	HIR_TY_IDENT = 0,
	HIR_TY_SLICE,
	HIR_TY_ARRAY,
	HIR_TY_PTR,
	HIR_TY_FN,
	HIR_TY_TUPLE,
	HIR_TY_INTEGER,
	HIR_TY_FLOATING,
	HIR_TY_CHAR,
	HIR_TY_BOOL,
	HIR_TY_VOID,
} hir_ty_kind_t;

typedef struct hir_ty {

	span_t span;

	hir_ty_kind_t kind: 8;

	union {
		hir_ident_t ident;

		struct {
			struct hir_ty *elem;
		} slice;

		struct {
			struct hir_ty *elem;
			struct hir_expr *len;
		} array;

		struct {
			struct hir_ty *elem;
		} ptr;

		struct {
			struct hir_ty *output;
			vecof(struct hir_ty *) inputs;
		} fn;

		struct {
			vecof(struct hir_ty *) elems;
		} tuple;

		struct {
			u8_t bytes;
			bool unsign;
		} integer;

		struct {
			u8_t bytes;
		} floating;
	};

} hir_ty_t;

__api hir_parse_t hir_ty_parse(hir_ty_t *ty, hir_parser_t *parser);
__api hir_parse_t hir_ty_consume(hir_ty_t *ty, hir_parser_t *parser);

#endif /* !__GUTS_HIR_TYPE_H */
/*!@} */
