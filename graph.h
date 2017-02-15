#ifndef GRAPH_H
#define GRAPH_H

#include <stdio.h>
#include "list.h"

struct graph {
    int vertices;
    int edges;
    zerg ** unitMatrix;
    int **adj;
};

struct graph * create_graph(list * listContainer);

#endif
