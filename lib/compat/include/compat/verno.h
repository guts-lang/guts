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

/*!@file compat/verno.h
 * @author uael
 *
 * @addtogroup compat @{
 */
#ifndef __COMPAT_VERNO_H
# define __COMPAT_VERNO_H

#include "compat/conf.h"

#define VERNO_GT(verno, major, minor) ((verno) > VERNO(major, minor, 0))
#define VERNO_GE(verno, major, minor) ((verno) >= VERNO(major, minor, 0))
#define VERNO_EQ(verno, major, minor) ((verno) == VERNO(major, minor, 0))
#define VERNO_LT(verno, major, minor) ((verno) < VERNO(major, minor, 0))
#define VERNO_LE(verno, major, minor) ((verno) <= VERNO(major, minor, 0))

/*!@private
 *
 * @def VERNO(major, minor, patch)
 *
 * @brief
 * Compute a valid semantic version into a unsigned long value.
 *
 * @param[in] major The major version.
 * @param[in] minor The minor version.
 * @param[in] patch The patch version.
 * @return An unsigned long.
 */
#define VERNO(major, minor, patch) ( \
	(((major) % 100) * 10000000) \
	+ (((minor) % 100) * 100000) \
	+ ((patch) % 100000) \
)

/*!@private
 *
 * @def VERNO_MAJOR(verno)
 *
 * @brief
 * Retrieve the major version from a computed verno.
 *
 * @param[in] verno The computed verno.
 * @return The major version.
 */
#define VERNO_MAJOR(verno) ( \
	((verno) / 10000000) % 100 \
)

/*!@private
 *
 * @def VERNO_MINOR(verno)
 *
 * @brief
 * Retrieve the minor version from a computed verno.
 *
 * @param[in] verno The computed verno.
 * @return The minor version.
 */
#define VERNO_MINOR(verno) ( \
	((verno) / 100000) % 100 \
)

/*!@private
 *
 * @def VERNO_PATCH(verno)
 *
 * @brief
 * Retrieve the patch version from a computed verno.
 *
 * @param[in] verno The computed verno.
 * @return The patch version.
 */
#define VERNO_PATCH(verno) ( \
	(verno)  % 100000 \
)

#define VERNO_0X_VRP(V) \
	VERNO(((V)&0xF00)>>8,((V)&0xF0)>>4,((V)&0xF))

#define VERNO_0X_VVRP(V)  \
	VERNO(((V)&0xFF00)>>8,((V)&0xF0)>>4,((V)&0xF))

#define VERNO_0X_VRPP(V)  \
	VERNO(((V)&0xF000)>>12,((V)&0xF00)>>8,((V)&0xFF))

#define VERNO_0X_VVRR(V)  \
	VERNO(((V)&0xFF00)>>8,((V)&0xFF),0)

#define VERNO_0X_VRRPPPP(V)  \
	VERNO(((V)&0xF000000)>>24,((V)&0xFF0000)>>16,((V)&0xFFFF))

#define VERNO_0X_VVRRP(V)  \
	VERNO(((V)&0xFF000)>>12,((V)&0xFF0)>>4,((V)&0xF))

#define VERNO_0X_VRRPP000(V)  \
	VERNO(((V)&0xF0000000)>>28,((V)&0xFF00000)>>20,((V)&0xFF000)>>12)

#define VERNO_0X_VVRRPP(V)  \
	VERNO(((V)&0xFF0000)>>16,((V)&0xFF00)>>8,((V)&0xFF))

#define VERNO_10_VPPP(V)  \
	VERNO(((V)/1000)%10,0,(V)%1000)

#define VERNO_10_VRP(V)  \
	VERNO(((V)/100)%10,((V)/10)%10,(V)%10)

#define VERNO_10_VRP000(V)  \
	VERNO(((V)/100000)%10,((V)/10000)%10,((V)/1000)%10)

#define VERNO_10_VRPP(V)  \
	VERNO(((V)/1000)%10,((V)/100)%10,(V)%100)

#define VERNO_10_VRR(V)  \
	VERNO(((V)/100)%10,(V)%100,0)

#define VERNO_10_VRRPP(V)  \
	VERNO(((V)/10000)%10,((V)/100)%100,(V)%100)

#define VERNO_10_VRR000(V)  \
	VERNO(((V)/100000)%10,((V)/1000)%100,0)

#define VERNO_10_VV00(V)  \
	VERNO(((V)/100)%100,0,0)

#define VERNO_10_VVRR(V)  \
	VERNO(((V)/100)%100,(V)%100,0)

#define VERNO_10_VVRRPP(V)  \
	VERNO(((V)/10000)%100,((V)/100)%100,(V)%100)

#define VERNO_10_VVRR0PP00(V)  \
	VERNO(((V)/10000000)%100,((V)/100000)%100,((V)/100)%100)

#define VERNO_10_VVRR0PPPP(V)  \
	VERNO(((V)/10000000)%100,((V)/100000)%100,(V)%10000)

#define VERNO_10_VVRR00PP00(V)  \
	VERNO(((V)/100000000)%100,((V)/1000000)%100,((V)/100)%100)

#endif /* !__COMPAT_VERNO_H */
/*!@} */
