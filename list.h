#ifndef LIST_H
#define LIST_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <stdbool.h>

#include "input.h"

typedef struct listContainer
{
    int numberOfMembers;
    struct listNode * first;
}   list;

struct listNode
{
    zerg * node;
    struct listNode * next;    
};

list * init_list(void);

list * add_zerg(list * listContainer, zerg * zergUnit);

void print_list(list * listContainer);

void destroy_list(list * listContainer);

double haversine(zerg *unit1, zerg * unit2);

void compare_list(list * listContainer);

#endif
