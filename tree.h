#ifndef TREE_H
#define TREE_H

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"

typedef struct tree
{

    zerg * node;
    struct tree *left;
    struct tree *right;
    int height;
    bool visited;
}   tree;

/** insert() is the main function that handles creating nodes and sorting them based on there symbol
*       name.  The reason it sorts by symbol name is because the key never gets updated, and is the
*       searched for the most frequently.  Insert() also handles most of the AVL tree logic and
*       balances itself on updates.
*/
struct tree *insert(tree * root, zerg * zergUnit);

/** get_balance() is a function called by insert and is used to determine if a tree need balancing
*       or not.  It does this by checking the heights of the left and right sub trees.
*/
int get_balance(tree * root);

/** left_rotate() shifts the nodes in an unbalanced tree by rotating it counter clockwise.
*       used when a tree is imbalanced.
*/
tree * left_rotate(tree * root);

/** right_rotate() is the same as left_rotate() but it does the opposite
*/
tree * right_rotate(tree * root);

/** max() returns the largest value of two.  Quick implementation to avoid importing math.h.
*/
int max(int a, int b);

/** height() returns the height field in the tree structure
*/
int height(tree * root);

int id_compare(int num1, int num2);

#endif
