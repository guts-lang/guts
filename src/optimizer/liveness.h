#ifndef LIVENESS_H
#define LIVENESS_H

#include <jayl/ir.h>

/*
 * Compute liveness of each variable on every edge, before and after
 * every ir operation.
 */
int live_variable_analysis(struct block *block);

/*
 * Determine whether a variable may be read after a given statement.
 * Return zero iff it is definitely not accessed after this point.
 */
int is_live_after(const struct symbol *sym, const struct statement *st);

#endif
