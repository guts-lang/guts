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
#include "generic.h"

struct hir_stmt;
struct hir_expr;

typedef struct hir_ent hir_ent_t;
typedef enum hir_ent_kind hir_ent_kind_t;

/*!@enum hir_ent_kind
 * @brief
 * Different kind of entity.
 */
enum hir_ent_kind {
	HIR_ENT_PARAM = 1 << 0,    /*!< See hir_ent::ent_param.    */
	HIR_ENT_VAR = 1 << 1,      /*!< See hir_ent::ent_var.      */
	HIR_ENT_FN = 1 << 2,       /*!< See hir_ent::ent_fn.       */
	HIR_ENT_CTOR = 1 << 3,     /*!< See hir_ent::ent_ctor.     */
	HIR_ENT_DTOR = 1 << 4,     /*!< See hir_ent::ent_dtor.     */
	HIR_ENT_STRUCT = 1 << 5,   /*!< See hir_ent::ent_struct.   */
	HIR_ENT_ENUM = 1 << 6,     /*!< See hir_ent::ent_enum.     */
	HIR_ENT_ENUM_VAR = 1 << 7, /*!< See hir_ent::ent_enum_var. */
	HIR_ENT_ENUM_TY = 1 << 8,  /*!< See hir_ent::ent_enum_ty.  */
	HIR_ENT_INCLUDE = 1 << 9,  /*!< See hir_ent::ent_include.  */
	HIR_ENT_USE = 1 << 10,     /*!< See hir_ent::ent_use.      */
	HIR_ENT_NS = 1 << 11,      /*!< See hir_ent::ent_ns.       */
};

/*!@struct hir_ent
 * @brief
 * High level representation of an entity.
 * @code{.y}
 * type
 * 	 : <ent_param>
 * 	 | <ent_var>
 * 	 | <ent_fn>
 * 	 | <ent_ctor>
 * 	 | <ent_dtor>
 * 	 | <ent_struct>
 * 	 | <ent_enum>
 * 	 | <ent_enum_var>
 * 	 | <ent_enum_ty>
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
	hir_ent_kind_t kind: 16;

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
		 * ent_fn
		 *   : <IDENT> '(' <params> ')'
		 *   | <IDENT> '(' <params> ')' : <type>
		 *   | <IDENT> '(' <params> ')' => <stmt>
		 *   | <IDENT> '(' <params> ')' : <type> => <stmt>
		 *   | <IDENT> '<' <template> '>' '(' <params> ')'
		 *   | <IDENT> '<' <template> '>' '(' <params> ')' : <type>
		 *   | <IDENT> '<' <template> '>' '(' <params> ')' => <stmt>
		 *   | <IDENT> '<' <template> '>' '(' <params> ')' : <type> => <stmt>
		 *   ;
		 * @endcode
		 * Where <IDENT> is the function name.
		 * Where <template> is the function template.
		 * Where <params> is a list of <ent_param>, the function parameters.
		 * Where <type> is the function return type.
		 * Where <stmt> is the function body.
		 */
		struct {
			hir_template_t *template;
			vecof(struct hir_ent) inputs;
			hir_ty_t *output;
			struct hir_stmt *block;
		} ent_fn;

		/*!@brief
		 * Constructor function entity: `self(a: u32)`.
		 * @code{.y}
		 * ent_dtor
		 *   : <SELF> '(' <params> ')'
		 *   | <SELF> '(' <params> ')' => <stmt>
		 *   ;
		 * @endcode
		 * Where <SELF> is the keyword `self`.
		 * Where <params> is a list of <ent_param>, the constructor parameters.
		 * Where <stmt> is the function body.
		 */
		struct {
			struct hir_stmt *block;
		} ent_ctor;

		/*!@brief
		 * Destructor function entity: `~foo(): u32 => 0`.
		 * @code{.y}
		 * ent_dtor
		 *   : '~' <SELF> '(' ')'
		 *   | '~' <SELF> '(' ')' => <stmt>
		 *   ;
		 * @endcode
		 * Where <SELF> is the keyword `self`.
		 * Where <stmt> is the function body.
		 */
		struct {
			struct hir_stmt *block;
		} ent_dtor;

		/*!@brief
		 * Structure entity: `struct Foo { foo: Bar; }`.
		 * @code{.y}
		 * ent_struct
		 *   : <STRUCT> <IDENT> '{' <fields> '}'
		 *   | <STRUCT> <IDENT> '<' <template> '>' '{' <fields> '}'
		 *   | <STRUCT> <IDENT> : <traits> '{' <fields> '}'
		 *   | <STRUCT> <IDENT> '<' <template> '>' : <traits> '{' <fields> '}'
		 *   ;
		 * @endcode
		 * Where <STRUCT> is the keyword `struct`.
		 * Where <IDENT> is the struct name.
		 * Where <template> is the struct template.
		 * Where <traits> is the struct traits.
		 * Where <fields> is the struct fields, a list of entities which each
		 *   item is ::HIR_ENT_VAR, ::HIR_ENT_FN, ::HIR_ENT_DTOR,
		 *   ::HIR_ENT_USE, ::HIR_ENT_STRUCT or ::HIR_ENT_ENUM.
		 */
		struct {
			hir_template_t *template;
			vecof(hir_ty_t) traits;
			vecof(struct hir_ent) fields;
		} ent_struct;

		/*!@brief
		 * Enum entity: `enum Foo { FOO(Bar), BAR(Baz), BAZ; }`.
		 * @code{.y}
		 * ent_enum
		 *   : <ENUM> <IDENT> '{' <fields> ';' '}'
		 *   : <ENUM> <IDENT> '{' <fields> ';' <methods> '}'
		 *   | <ENUM> <IDENT> '<' <template> '>' '{' <fields> ';' '}'
		 *   | <ENUM> <IDENT> '<' <template> '>' '{' <fields> ';' <methods> '}'
		 *   | <ENUM> <IDENT> : <traits> '{' <fields> ';' '}'
		 *   | <ENUM> <IDENT> : <traits> '{' <fields> ';' <methods> '}'
		 *   | <ENUM> <IDENT> '<' <template> '>' : <traits> '{' <fields> ';' '}'
		 *   | <ENUM> <IDENT> '<' <template> '>' : <traits> '{' <fields> ';'
		 *       <methods> '}'
		 *   ;
		 * @endcode
		 * Where <ENUM> is the keyword `enum`.
		 * Where <IDENT> is the enum name.
		 * Where <template> is the enum template.
		 * Where <traits> is the enum traits.
		 * Where <fields> is the enum fields, a list of entities separated
		 *   by commas which each item is ::HIR_ENT_ENUM_VAR or
		 *   ::HIR_ENT_ENUM_TY.
		 * Where <methods> is the enum methods, a list of entities which each
		 *   item is ::HIR_ENT_ENUM_FN or ::HIR_ENT_ENUM_DTOR.
		 */
		struct {
			hir_template_t *template;
			vecof(hir_ty_t) traits;
			vecof(struct hir_ent) fields;
		} ent_enum;

		/*!@brief
		 * Enum variable (or constant) entity: `FOO = 42`.
		 * @code{.y}
		 * ent_enum_var
		 *   : <IDENT>
		 *   | <IDENT> = <expr>
		 *   ;
		 * @endcode
		 * Where <IDENT> is the enum constant name.
		 * Where <expr> is the enum constant value.
		 */
		struct {
			struct hir_expr *value;
		} ent_enum_var;

		/*!@brief
		 * Enum type entity: `FOO(i32)`.
		 * @code{.y}
		 * ent_enum_ty
		 *   : <IDENT> '(' <type> ')'
		 *   ;
		 * @endcode
		 * Where <IDENT> is the enum type name.
		 * Where <type> is the enum union type.
		 */
		struct {
			hir_ty_t type;
		} ent_enum_ty;

		/*!@brief
		 * Include entity: `include <stdio.h> as io`.
		 * @code{.y}
		 * ent_include
		 *   : <INCLUDE> '<' <file> '>' as <IDENT>
		 *   | <INCLUDE> '"' <file> '"' as <IDENT>
		 *   ;
		 * @endcode
		 * Where <INCLUDE> is the keyword `include`.
		 * Where <IDENT> is the named scope for the includes symbols.
		 * Where <file> is a filename.
		 */
		struct {
			bool local;
			char filename[U8_MAX];
		} ent_include;

		/*!@brief
		 * Use entity: `use foo::Bar` or `use foo::Bar as Baz`.
		 * @code{.y}
		 * ent_use
		 *   : <USE> <path> '::' <IDENT>
		 *   | <USE> <path> '::' <IDENT> as <IDENT>
		 *   ;
		 * @endcode
		 * Where <USE> is the keyword `use`.
		 * Where <IDENT> is the ident to use.
		 * Where <path> is the path where to locate <IDENT>.
		 */
		struct {
			vecof(hir_name_t) path;
			hir_name_t *as;
		} ent_use;

		/*!@brief
		 * Namespace entity: `namespace foo { bar: u8 = 42; }`.
		 * @code{.y}
		 * ent_ns
		 *   : <NAMESPACE> <IDENT> '{' <fields> '}'
		 *   | <NAMESPACE> <path> '::' <IDENT> '{' <fields> '}'
		 *   ;
		 * @endcode
		 * Where <NAMESPACE> is the keyword `namespace`.
		 * Where <IDENT> is the namespace name.
		 * Where <path> is the namespace path.
		 * Where <fields> is the struct fields, a list of entities which each
		 *   item is ::HIR_ENT_VAR, ::HIR_ENT_FN, ::HIR_ENT_USE,
		 *   ::HIR_ENT_STRUCT, ::HIR_ENT_ENUM or ::HIR_ENT_NS.
		 */
		struct {
			vecof(hir_name_t) path;
			vecof(struct hir_ent) fields;
		} ent_ns;
	};
};

/*!@brief
 * Parse an entity using #hir_ent syntax.
 *
 * @note
 * If the parsed entity finish by a statement, the match for closing is omitted
 * and prohibbed.
 *
 * @param[out]    self     The entity to parse.
 * @param[in,out] parser   The parser to use for parsing.
 * @param[in]     allowed  Flags of allowed #hir_ent_kind kinds to parse.
 *                         Emit an error if the parsed entity kind isn't present
 *                         in the `allowed` flags.
 * @param[in]     closings The closings token to match at end of the entity.
 *                         Emit an error if end token isn't present in the
 *                         `closings` range.
 * @return                 The closing token, already skipped, on success,
 *                         peek otherwise.
 */
__api hir_tok_t *hir_ent_consume(hir_ent_t *self, hir_parser_t *parser,
								 u16_t allowed, char __const *closings);

/*!@brief
 * TODO
 *
 * @param fields
 * @return
 */
__api hir_tok_t *hir_struct_body(vecof(hir_ent_t) *fields);

/*!@brief
 * TODO
 *
 * @param fields
 * @return
 */
__api hir_tok_t *hir_enum_body(vecof(hir_ent_t) *fields);

/*!@brief
 * TODO
 *
 * @param self
 */
__api void hir_ent_destroy(hir_ent_t *self);

#endif /* !__GUTS_HIR_ENTITY_H */
/*!@} */
