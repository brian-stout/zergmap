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
    int deleted;
    zerg ** unitMatrix;
    int **adj;
};

bool are_doub_equal(double num1, double num2);
double TO_RAD(double number);
double haversine(zerg *unit1, zerg * unit2);
double find_distance(zerg *unit1, zerg * unit2);
int return_adj_num(struct graph * zergMap, int vertices);
int return_first_adj(struct graph * zergMap, int vert);
struct graph * delete_node(struct graph * zergMap, int vert);
struct graph * remove_leafs(struct graph * zergMap, int vert);
struct graph * cleanup_graph(struct graph * zergMap);
void print_matrix_table(struct graph *);

struct graph * create_graph(list * listContainer);

#endif
