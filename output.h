#ifndef OUTPUT_H
#define OUTPUT_H

#include <stdio.h>
#include <stdbool.h>

#include "input.h"
#include "graph.h"

void print_zerg_unit(zerg *zerg_unit);
void print_zerg_removal(struct graph * zergGraph);

#endif
