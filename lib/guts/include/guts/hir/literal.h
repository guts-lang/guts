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

/*!@file guts/hir/literal.h
 * @author uael
 *
 * @addtogroup guts.hir @{
 */
#ifndef __GUTS_HIR_LITERAL_H
# define __GUTS_HIR_LITERAL_H

#include <ds/vector.h>
#include <il/diagnostic.h>

#include "guts/conf.h"

struct hir_tok;

/*!@brief
 * Integer number literal.
 * When base is set to `HIR_INTEGER_UNRESOLVED' the `number' field point to
 * the begin of the number in the source string.
 * Once resolved the `base', `size' and `number' fields are set.
 */
typedef struct {

	/*! Base of the integer literal. */
	enum {
		HIR_INTEGER_UNRESOLVED = 0, /*!< Must be resolved. */
		HIR_INTEGER_DECIMAL,        /*!< By default, no `0o' or `0x' prefix. */
		HIR_INTEGER_OCTAL,          /*!< `0o' prefix. */
		HIR_INTEGER_HEXADECIMAL,    /*!< `0x' prefix. */
	} base;

	/*! Size part of a integer literal suffix. */
	enum {
		HIR_INTEGER_INT = 0, /*!< By default, no `l' or `ll' suffix. */
		HIR_INTEGER_LONG,    /*!< `l' suffix. */
		HIR_INTEGER_LLONG,   /*!< `ll' suffix. */
	} size;

	/*! Integer literal has unsigned type, `u' suffix. */
	bool unsign;

	/*! Union type to store resolved or unresolved integer value. */
	union {

		/*! Unresolved value, contains prefix and suffix. */
		char __const *number;

		/*! Resolved value. */
		long long int value;
	};

} hir_integer_t;

/*!@brief
 * Floating point number literal.
 * Parsed as a `hir_integer_t' then converted to floating number on resolve.
 */
typedef struct {

	/*! Floating point number base. */
	enum {
		HIR_FLOAT_DECIMAL = 0,     /*!< By default, no prefix. */
		HIR_FLOAT_HEXADECIMAL, /*!< Hexadecimal prefix. */
	} base;

	/*! Floating point literal format specified by the suffix. */
	enum {
		HIR_FLOAT_DOUBLE = 0,  /*!< By default, no suffix. */
		HIR_FLOAT_FLOAT,   /*!< `f' suffix. */
		HIR_FLOAT_LDOUBLE, /*!< `l' suffix. */
	} size;

	/*! floating value. */
	long double value;

} hir_float_t;

/*!@brief
 * A UTF-8 string literal: `"foo"`.
 */
typedef vecof(char) hir_string_t;

/*!@brief
 * A character literal: `'a'`.
 */
typedef char hir_char_t;

/*!@brief
 * A boolean literal: `true` or `false`.
 */
typedef bool hir_bool_t;

/*!@brief
 * A literal such as a string or integer or boolean.
 */
typedef struct {
	enum {
		HIR_LIT_INTEGER = 0,
		HIR_LIT_FLOAT,
		HIR_LIT_STRING,
		HIR_LIT_CHAR,
		HIR_LIT_BOOL,
		HIR_LIT_NULL,
	} kind;
	union {
		hir_integer_t integer;
		hir_float_t floating;
		hir_string_t string;
		hir_char_t chr;
		hir_bool_t boolean;
	};
} hir_lit_t;

/*!@brief
 * TODO
 *
 * @param tok
 * @param diags
 * @return
 */
__api bool hir_number_resolve(struct hir_tok *tok, vecof(diag_t) *diags);

#endif /* !__GUTS_HIR_LITERAL_H */
/*!@} */
