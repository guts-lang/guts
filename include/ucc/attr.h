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

/*!@file ucc/attr.h
 * @author uael
 */
#ifndef __UCC_ATTR_H
# define __UCC_ATTR_H

#include "kw.h"

#if !defined DEPRECATED
# if __has_declspec_attribute__(deprecated)
#   define DEPRECATED __declspec(deprecated)
# elif __has_attribute__(deprecated)
#   define DEPRECATED __attribute__((deprecated))
# else
#   define DEPRECATED
# endif
#endif

#if !defined NOSANITIZE
# if __has_feature__(address_sanitizer) || defined __SANITIZE_ADDRESS__ || \
     __has_attribute__(no_sanitize_address)
#   define NOSANITIZE __attribute__((no_sanitize_address))
# else
#   define NOSANITIZE
# endif
#endif

#if !defined FORCEINLINE
# if defined U_DEBUG
#   define FORCEINLINE __inline
# else
#   define FORCEINLINE __always_inline __inline
# endif
#endif

#if !defined NORETURN
# if defined U_DEBUG
#   define NORETURN
# else
#   define NORETURN __noreturn
# endif
#endif

#if !defined PURE
# if __has_attribute__(pure)
#   define PURE __attribute__((__pure__))
# elif defined __pure
#   define PURE __pure
# else
#   define PURE
# endif
#endif

#if !defined CONST
# if __has_attribute__(__const)
#   define CONST __attribute__((__const__))
# elif defined __pure2
#   define CONST __pure2
# else
#   define CONST
# endif
#endif

#if !defined UNUSED
# if __has_attribute__(unused)
#   define UNUSED __attribute__((unused))
# elif defined __LCLINT__
#   define UNUSED /*@unused@*/
# elif defined __unused
#   define UNUSED __unused
# else
#   define UNUSED
# endif
#endif

#endif /* !__UCC_ATTR_H */
