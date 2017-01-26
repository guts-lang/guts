#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <jayl/ir.h>

/*
 * Optimization pass which joins together sequential assignments to the
 * same variable into a single statement.
 *
 *   .t1 = a + 1
 *   b = .t1
 *
 * If .t1 is not live after the second line, the sequence can be
 * converted to the following:
 *
 *   b = a + 1
 *
 */
int merge_chained_assignment(struct block *block);

/*
 * Remove assignments to variables that are never read, as determined by
 * liveness analysis.
 */
int dead_store_elimination(struct block *block);

#endif
