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

/*!@file guts/hir/generic.h
 * @author uael
 *
 * @addtogroup guts.hir @{
 */
#ifndef __GUTS_HIR_GENERIC_H
# define __GUTS_HIR_GENERIC_H

#include "parser.h"

struct hir_ty;

/*!@struct hir_generic_t
 * @brief
 * High level representation of a template item.
 * @code{.y}
 * generic
 * 	 : <IDENT>
 * 	 | <IDENT> ':' <type>
 * 	 ;
 * @endcode
 */
typedef struct {

	/*! Generic location on the origin source. */
	span_t span;

	/*! Generic name. */
	hir_name_t name;

	/*! Generic clause. */
	struct hir_ty *clause;

} hir_generic_t;

/*!@struct hir_template_t
 * @brief
 * High level representation of a template.
 * @code{.y}
 * template
 * 	 : <generic>
 * 	 | <template> ',' <generic>
 * 	 ;
 * @endcode
 */
typedef struct {

	/*! Template location on the origin source. */
	span_t span;

	/*! Template items. */
	vecof(hir_generic_t) elems;

} hir_template_t;

#endif /* !__GUTS_HIR_GENERIC_H */
/*!@} */
