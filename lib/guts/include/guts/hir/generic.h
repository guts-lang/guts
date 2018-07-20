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

/*!@brief
 * Try to parse a generic using #hir_generic_t syntax.
 * 
 * @param[out]    self   The generic to parse.
 * @param[in,out] parser The parser to use for parsing.
 * @return               `true` on success, `false` otherwise.
 *                       errors has been reported to `parser->codespan`.
 */
__api bool hir_generic_parse(hir_generic_t *self, hir_parser_t *parser);

/*!@brief
 * Parse a generic using #hir_generic_t syntax.
 * 
 * @param[out]    self   The generic to parse.
 * @param[in,out] parser The parser to use for parsing.
 * @return               `true` on success, `false` otherwise.
 *                       errors has been reported to `parser->codespan`.
 */
__api bool hir_generic_consume(hir_generic_t *self, hir_parser_t *parser);

/*!@brief
 * Destroy the given generic.
 *
 * @param[in,out] self The generic to destroy.
 */
__api void hir_generic_destroy(hir_generic_t *self);

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

/*!@brief
 * Try to parse a template using #hir_template_t syntax.
 * 
 * @param[out]    self   The template to parse.
 * @param[in,out] parser The parser to use for parsing.
 * @return               `true` on success, `false` otherwise.
 *                       errors has been reported to `parser->codespan`.
 */
__api bool hir_template_parse(hir_template_t *self, hir_parser_t *parser);

/*!@brief
 * Parse a template using #hir_template_t syntax.
 * 
 * @param[out]    self   The template to parse.
 * @param[in,out] parser The parser to use for parsing.
 * @return               `true` on success, `false` otherwise.
 *                       errors has been reported to `parser->codespan`.
 */
__api bool hir_template_consume(hir_template_t *self, hir_parser_t *parser);

/*!@brief
 * Destroy the given template.
 *
 * @param[in,out] self The template to destroy.
 */
__api void hir_template_destroy(hir_template_t *self);

/*!@struct hir_sym_t
 * @brief
 * High level representation of a symbol.
 * @code{.y}
 * sym
 * 	 : <IDENT>
 * 	 | <IDENT> '<' <type_list> '>'
 * 	 ;
 * @endcode
 */
typedef struct {

	/*! Symbol location on the origin source. */
	span_t span;

	/*! Symbol name. */
	hir_name_t name;

	/*! Symbol type template. */
	vecof(struct hir_ty) apply;

} hir_sym_t;

/*!@brief
 * Try to parse a sym using #hir_sym_t syntax.
 * 
 * @param[out]    self   The sym to parse.
 * @param[in,out] parser The parser to use for parsing.
 * @return               `true` on success, `false` otherwise.
 *                       errors has been reported to `parser->codespan`.
 */
__api bool hir_sym_parse(hir_sym_t *self, hir_parser_t *parser);

/*!@brief
 * Parse a sym using #hir_sym_t syntax.
 * 
 * @param[out]    self   The sym to parse.
 * @param[in,out] parser The parser to use for parsing.
 * @return               `true` on success, `false` otherwise.
 *                       errors has been reported to `parser->codespan`.
 */
__api bool hir_sym_consume(hir_sym_t *self, hir_parser_t *parser);

/*!@brief
 * Destroy the given sym.
 *
 * @param[in,out] self The sym to destroy.
 */
__api void hir_sym_destroy(hir_sym_t *self);

/*!@struct hir_path_t
 * @brief
 * High level representation of a path.
 * @code{.y}
 * path
 * 	 : <sym>
 * 	 | <path> '::' <sym>
 * 	 ;
 * @endcode
 */
typedef struct {

	/*! Path location on the origin source. */
	span_t span;

	/*! Path symbols. */
	vecof(hir_sym_t) elems;

} hir_path_t;

/*!@brief
 * Try to parse a path using #hir_path_t syntax.
 * 
 * @param[out]    self   The path to parse.
 * @param[in,out] parser The parser to use for parsing.
 * @return               `true` on success, `false` otherwise.
 *                       errors has been reported to `parser->codespan`.
 */
__api bool hir_path_parse(hir_path_t *self, hir_parser_t *parser);

/*!@brief
 * Parse a path using #hir_path_t syntax.
 * 
 * @param[out]    self   The path to parse.
 * @param[in,out] parser The parser to use for parsing.
 */
__api void hir_path_consume(hir_path_t *self, hir_parser_t *parser);

/*!@brief
 * Destroy the given path.
 *
 * @param[in,out] self The path to destroy.
 */
__api void hir_path_destroy(hir_path_t *self);

#endif /* !__GUTS_HIR_GENERIC_H */
/*!@} */
