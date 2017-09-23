/*
 * MIT License
 *
 * Copyright (c) 2016-2017 Abel Lucas <www.github.com/uael>
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

/*!@file il/ty/const/kind.h
 * @author uael
 */
#ifndef __IL_TY_CONST_KIND_H
# define __IL_TY_CONST_KIND_H

enum il_ty_const_kind {
  IL_TY_CONST_I8,
  IL_TY_CONST_I16,
  IL_TY_CONST_I32,
  IL_TY_CONST_I64,
  IL_TY_CONST_U8,
  IL_TY_CONST_U16,
  IL_TY_CONST_U32,
  IL_TY_CONST_U64,
  IL_TY_CONST_F32,
  IL_TY_CONST_F64,
  IL_TY_CONST_STR,
};

typedef enum il_ty_const_kind il_ty_const_kind_t;

#endif /* !__IL_TY_CONST_KIND_H */
