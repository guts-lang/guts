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

/*!@file termcolor/term.h
 * @author uael
 *
 * @addtogroup termcolor @{
 */
#ifndef __TERMCOLOR_TERM_H
# define __TERMCOLOR_TERM_H

#include "termcolor/color.h"

#if defined(CC_MSVC)
# include <io.h>
#elif defined(HAS_UNISTD_H)
# include <unistd.h>
#else
# error "Unable to impl termcolor"
#endif

#include <stdio.h>

/*!@brief
 * TODO
 *
 * @param stream
 * @param spec
 * @return
 */
__api FILE *termcolor_set(FILE *stream, color_spec_t spec);

/*!@brief
 * TODO
 *
 * @param stream
 * @param color
 * @return
 */
__api FILE *termcolor_setfg(FILE *stream, color_t color);

/*!@brief
 * TODO
 *
 * @param stream
 * @param color
 * @return
 */
__api FILE *termcolor_setbg(FILE *stream, color_t color);

/*!@brief
 * TODO
 *
 * @param stream
 * @param flags
 * @return
 */
__api FILE *termcolor_setfl(FILE *stream, u8_t flags);

/*!@brief
 * TODO
 *
 * @param stream
 * @return
 */
__api FILE *termcolor_reset(FILE *stream);

#endif /* !__TERMCOLOR_TERM_H */
/*!@} */
