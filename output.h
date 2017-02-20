#ifndef OUTPUT_H
#define OUTPUT_H

#include <stdio.h>
#include <stdbool.h>
#include <math.h>

#include "input.h"
#include "graph.h"
#include "tree.h"

void print_zerg_unit(zerg *zergUnit);
void print_zerg_removal(struct graph * zergGraph);
bool print_status(tree * zergTree, int healthPercentage, bool printOnce);

#endif
