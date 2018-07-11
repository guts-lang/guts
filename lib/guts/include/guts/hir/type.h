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
struct hir_ent;
struct hir_ty;

typedef struct hir_ty hir_ty_t;
typedef enum hir_ty_kind hir_ty_kind_t;

/*!@enum hir_ty_kind
 * @brief
 * Different kind of type.
 */
enum hir_ty_kind {
	HIR_TY_VOID = 0, /*!< See hir_ty.              */
	HIR_TY_CHAR,     /*!< See hir_ty.              */
	HIR_TY_BOOL,     /*!< See hir_ty.              */
	HIR_TY_INT,      /*!< See hir_ty::ty_int.      */
	HIR_TY_FLOAT,    /*!< See hir_ty::ty_float.    */
	HIR_TY_SYM,      /*!< See hir_ty::ty_sym.      */
	HIR_TY_TUPLE,    /*!< See hir_ty::ty_tuple.    */
	HIR_TY_LAMBDA,   /*!< See hir_lambda.          */
	HIR_TY_NULLABLE, /*!< See hir_ty::ty_nullable. */
	HIR_TY_PTR,      /*!< See hir_ty::ty_ptr.      */
	HIR_TY_SLICE,    /*!< See hir_ty::ty_slice.    */
	HIR_TY_ARRAY,    /*!< See hir_ty::ty_array.    */
	HIR_TY_STRUCT,   /*!< See hir_ty::ty_struct.   */
	HIR_TY_ENUM,     /*!< See hir_ty::ty_enum.     */
};

/*!@struct hir_ty
 * @brief
 * High level representation of a type.
 * @code{.y}
 * type
 * 	 : <CHAR>
 * 	 | <BOOL>
 * 	 | <ty_int>
 * 	 | <ty_float>
 * 	 | <ty_sym>
 * 	 | <ty_tuple>
 * 	 | <ty_lambda>
 * 	 | <ty_nullable>
 * 	 | <ty_ptr>
 * 	 | <ty_slice>
 * 	 | <ty_array>
 * 	 ;
 * @endcode
 */
struct hir_ty {

	/*! Type location on the origin source. */
	span_t span;

	/*! Type kind, See hir_ty_kind: HIR_TY_*. */
	hir_ty_kind_t kind: 8;

	/*! Data union of all type kind. */
	union {

		/*!@brief
		 * Integer type: `i32` or `u64`.
		 * @code{.y}
		 * ty_int
		 *   : ('i'|'u')<BYTES>
		 *   ;
		 * @endcode
		 * Where <BYTES> is '8', '16', '32' or '64'.
		 * Where 'i' is for signed integer type, 'u' for unsigned.
		 */
		struct {
			u8_t bytes;  /*!< Integer bytes, '8', '16', '32' or '64'. */
			bool unsign; /*!< Integer is unsigned or not. */
		} ty_int;

		/*!@brief
		 * Floating type: `f32` or `f64`.
		 * @code{.y}
		 * ty_float
		 *   : 'f'<BYTES>
		 *   ;
		 * @endcode
		 * Where <BYTES> is '32' or '64'.
		 */
		struct {
			u8_t bytes; /*!< Floating bytes, '32' or '64'. */
		} ty_float;

		/*!@brief
		 * Symbol type: `foo<bar, u8>`.
		 * @code{.y}
		 * ty_sym
		 *   : <IDENT>
		 *   | <IDENT> '<' <TYPES> '>'
		 *   ;
		 * @endcode
		 * Where <IDENT> is an unresolved symbol identifier.
		 * Where <TYPES> is a list of types.
		 */
		struct {
			hir_ident_t ident;               /*!< Symbol identifier. */
			vecof(struct hir_ty) template; /*!< Template type list. */
		} ty_sym;

		/*!@brief
		 * Tuple type: `<i8, i16>` or `<u8>`.
		 * @code{.y}
		 * ty_tuple
		 *   : '<' <TYPES> '>'
		 *   ;
		 * @endcode
		 * Where <TYPES> is a list of types.
		 */
		struct {
			vecof(struct hir_ty) elems; /*!< Tuple type elements. */
		} ty_tuple;

		/*!@struct hir_lambda
		 * @brief
		 * Lambda function type: `(u8, u16): u32`.
		 * @code{.y}
		 * ty_lambda
		 *   : '(' ')' '>'
		 *   | '(' ')' ':' <type>
		 *   | '(' <TYPES> ')'
		 *   | '(' <TYPES> ')' ':' <type>
		 *   ;
		 * @endcode
		 * Where <type> is the 'type' rule, represent the return type.
		 * Where <TYPES> represent the arguments types.
		 */
		struct {
			struct hir_ty *output;         /*!< Lambda return type. */
			vecof(struct hir_ty) inputs; /*!< Lambda arguments types. */
		} ty_lambda;

		/*!@brief
		 * Nullable pointer type: `? T` or `? const T`.
		 * Can be null (empty) or reference (pointer).
		 * A reference cannot be null.
		 * @code{.y}
		 * ty_nullable
		 *   : '?' <type>
		 *   | '?' <CONST> <type>
		 *   ;
		 * @endcode
		 * Where <type> is the 'type' rule.
		 * Where <CONST> is the 'const' keyword.
		 */
		struct {
			bool constant;
			struct hir_ty *elem;
		} ty_nullable;

		/*!@brief
		 * Raw pointer type: `* T` or `*const T`.
		 * Cannot be null.
		 * @code{.y}
		 * ty_ptr
		 *   : '*' <type>
		 *   | '*' <CONST> <type>
		 *   ;
		 * @endcode
		 * Where <type> is the 'type' rule.
		 * Where <CONST> is the 'const' keyword.
		 */
		struct {
			bool constant;
			struct hir_ty *elem;
		} ty_ptr;

		/*!@brief
		 * Dynamically sized slice type: `[T]`.
		 * @code{.y}
		 * ty_slice
		 *   : '[' <type> ']'
		 *   ;
		 * @endcode
		 * Where <type> is the 'type' rule.
		 */
		struct {
			struct hir_ty *elem;
		} ty_slice;

		/*!@brief
		 * Fixed size array type: `[T; n]`.
		 * @code{.y}
		 * ty_array
		 *   : '[' <type> ';' <expr> ']'
		 *   ;
		 * @endcode
		 * Where <type> is the 'type' rule, represent the item type.
		 * Where <expr> is the 'expr' rule, represent the array size.
		 */
		struct {
			struct hir_ty *elem;
			struct hir_expr *len;
		} ty_array;

		/*!@brief
		 * Anonymous structure type: `struct { foo: u8; }`.
		 * @code{.y}
		 * ty_struct
		 *   : <STRUCT> <struct_body>
		 *   ;
		 * @endcode
		 * Where <STRUCT> is the keyword `struct`.
		 * Where <struct_body> is the structure body.
		 */
		struct {
			vecof(struct hir_ent) fields;
		} ty_struct;

		/*!@brief
		 * Anonymous enum type: `enum { FOO, BAR, BAZ; }`.
		 * @code{.y}
		 * ty_enum
		 *   : <ENUM> <enum_body>
		 *   ;
		 * @endcode
		 * Where <ENUM> is the keyword `enum`.
		 * Where <enum_body> is the enum body.
		 */
		struct {
			vecof(struct hir_ent) fields;
		} ty_enum;
	};
};

/*!@brief
 * Try to parse a type using #hir_ty syntax.
 *
 * @param[out]    self   The type to parse.
 * @param[in,out] parser The parser to use for parsing.
 * @return               ::PARSE_OK on success, ::PARSE_NONE if nothing was
 *                       parsed or ::PARSE_ERROR on error, errors has been
 *                       reported to `parser->codespan`.
 */
__api hir_parse_t hir_ty_parse(hir_ty_t *self, hir_parser_t *parser);

/*!@brief
 * Parse a type using #hir_ty syntax.
 *
 * @param[out]    self   The type to parse.
 * @param[in,out] parser The parser to use for parsing.
 * @return               ::PARSE_OK on success or and ::PARSE_ERROR on error or
 *                       empty, errors has been reported to `parser->codespan`.
 */
__api hir_parse_t hir_ty_consume(hir_ty_t *self, hir_parser_t *parser);

/*!@brief
 * Destroy the given type.
 *
 * @param[in,out] self The type to destroy.
 */
__api void hir_ty_destroy(hir_ty_t *self);

#endif /* !__GUTS_HIR_TYPE_H */
/*!@} */
