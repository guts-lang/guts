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

#include <ds/deque.h>
#include <il/codemap.h>

#include "guts/conf.h"

struct hir_tok;

typedef enum {
	HIR_INTEGER_UNRESOLVED = 0,
	HIR_INTEGER_DECIMAL,
	HIR_INTEGER_OCTAL,
	HIR_INTEGER_HEXADECIMAL,
} hit_integer_base_t;

typedef enum {
	HIR_INTEGER_INT,
	HIR_INTEGER_LONG,
	HIR_INTEGER_LLONG,
} hit_integer_size_t;

typedef struct {
	hit_integer_base_t base;
	hit_integer_size_t size;
	bool unsign;
	char __const * number;
} hir_integer_t;

typedef enum {
	HIR_FLOAT_UNRESOLVED = 0,
	HIR_FLOAT_DECIMAL,
	HIR_FLOAT_HEXADECIMAL,
} hit_float_base_t;

typedef enum {
	HIR_FLOAT_FLOAT,
	HIR_FLOAT_DOUBLE,
	HIR_FLOAT_LDOUBLE
} hit_float_size_t;

typedef struct {
	hit_float_base_t base;
	hit_float_size_t size;
	char __const * number;
} hir_float_t;

/*!@brief
 * TODO
 *
 * @param tok
 * @param diags
 * @return
 */
__api bool hit_number_resolve(struct hir_tok *tok, vecof(diag_t) *diags);

typedef vecof(char) hir_string_t;

typedef char __const *hir_ident_t;

#endif /* !__GUTS_HIR_LITERAL_H */
/*!@} */
