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

#include "type.h"

struct hir_stmt;
struct hir_expr;

typedef struct hir_ent hir_ent_t;
typedef enum hir_ent_kind hir_ent_kind_t;

/*!@enum hir_ent_kind
 * @brief
 * Different kind of entity.
 */
enum hir_ent_kind {
	HIR_ENT_PARAM = 0, /*!< See hir_ent::ent_param.   */
	HIR_ENT_VAR,       /*!< See hir_ent::ent_var.     */
	HIR_ENT_FN,        /*!< See hir_ent::ent_fn.      */
	HIR_ENT_STRUCT,    /*!< See hir_ent::ent_struct.  */
	HIR_ENT_ENUM,      /*!< See hir_ent::ent_enum.    */
	HIR_ENT_INCLUDE,   /*!< See hir_ent::ent_include. */
	HIR_ENT_USE,       /*!< See hir_ent::ent_use.     */
	HIR_ENT_NS,        /*!< See hir_ent::ent_ns.      */
};

/*!@struct hir_ent
 * @brief
 * High level representation of an entity.
 * @code{.y}
 * type
 * 	 : <ent_param>
 * 	 | <ent_var>
 * 	 | <ent_fn>
 * 	 | <ent_struct>
 * 	 | <ent_enum>
 * 	 | <ent_include>
 * 	 | <ent_use>
 * 	 | <ent_ns>
 * 	 ;
 * @endcode
 */
struct hir_ent {

	/*! Entity location on the origin source. */
	span_t span;

	/*! Entity kind, See hir_ent_kind: HIR_ENT_*. */
	hir_ty_kind_t kind: 8;

	/*! Entity name. */
	hir_name_t name;

	/*! Entity parent, only the root entity got a zeroed parent. */
	struct hir_ent *parent;

	/*! Data union of all entity kind. */
	union {

		/*!@brief
		 * Parameter entity: `foo: u32 = 0`.
		 * @code{.y}
		 * ent_param
		 *   : <IDENT> : <type>
		 *   | <IDENT> : <type> = <expr>
		 *   ;
		 * @endcode
		 * Where <IDENT> is the parameter name.
		 * Where <type> is the parameter type.
		 * Where <expr> is the parameter value.
		 */
		struct {
			hir_ty_t type;
			struct hir_expr *value;
		} ent_param;

		/*!@brief
		 * Variable entity: `foo: u32 = 0`.
		 * @code{.y}
		 * ent_var
		 *   : <IDENT> : <type>
		 *   | <IDENT> : <type> = <expr>
		 *   ;
		 * @endcode
		 * Where <IDENT> is the variable name.
		 * Where <type> is the variable type.
		 * Where <expr> is the variable value.
		 */
		struct {
			hir_ty_t type;
			struct hir_expr *value;
		} ent_var;

		/*!@brief
		 * Function entity: `foo(): u32 => 0`.
		 * @code{.y}
		 * ent_var
		 *   : <IDENT> <ty_lambda>
		 *   | <IDENT> <ty_lambda> => <stmt>
		 *   ;
		 * @endcode
		 * Where <IDENT> is the function name.
		 * Where <ty_lambda> is the function lambda type.
		 * Where <stmt> is the function body.
		 */
		struct {
			hir_lambda_t lambda;
			struct hir_stmt *block;
		} ent_fn;

		/*!@brief
		 * Structure entity: `foo(): u32 => 0`.
		 * @code{.y}
		 * ent_var
		 *   : <IDENT> <ty_lambda>
		 *   | <IDENT> <ty_lambda> => <stmt>
		 *   ;
		 * @endcode
		 *
		 */
		struct {

		} ent_struct;
	};

};

__api hir_parse_t hir_param_parse(hir_ent_t *self, hir_parser_t *parser);
__api hir_parse_t hir_var_parse(hir_ent_t *self, hir_parser_t *parser);

#endif /* !__GUTS_HIR_ENTITY_H */
/*!@} */
