#ifndef OPTIMIZE_H
#define OPTIMIZE_H

#include "liveness.h"

#include <jayl/ir.h>

/* Set to non-zero to enable optimization. */
void push_optimization(int level);

/*
 * Do data flow analysis and perform optimizations on the intermediate
 * representation. Leaves the definition in a semantically equivalent,
 * and hopefully more consise, state.
 */
void optimize(struct definition *def);

/* Disable previously set optimization, cleaning up resources. */
void pop_optimization(void);

#endif
