#ifndef GRAPH_H
#define GRAPH_H

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <math.h>
#include <stdbool.h>

#include "list.h"

struct graph {
    int vertices;
    int edges;
    zerg ** unitMatrix;
    int **adj;
};

bool are_doub_equal(double num1, double num2);
double TO_RAD(double number);
double haversine(zerg *unit1, zerg * unit2);
double find_distance(zerg *unit1, zerg * unit2);
void print_matrix_table(struct graph *);

struct graph * create_graph(list * listContainer);

#endif
