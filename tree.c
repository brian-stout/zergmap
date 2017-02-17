#include "tree.h"

int id_compare(int num1, int num2)
{
        return num1 - num2;
}

int height(tree * root)
{
    //Function just grabs the height and returns it
    //Used in the AVL portion of the tree
    if (root != NULL)
    {
        return root->height;
    }
    else
    {
        return 0;
    }
}

int max(int a, int b)
{
    //Don't want to import math.h just for this
    if (a > b) { return a; }
    else { return b; }
}

tree * right_rotate(tree * root)
{
    //Grabbing the left to move it
    tree * leftChild = root->left;

    //Storing the right of the left to move it
    tree * subTree = leftChild->right;

    //Doing the rotation
    leftChild->right = root;
    root->left = subTree;

    //Updating the heights
    root->height = max(height(root->left), height(root->right)) + 1;
    leftChild->height = max(height(leftChild->left), height(leftChild->right)) + 1;

    //Returning the new root
    return leftChild;
}

tree * left_rotate(tree * root)
{
    //Exactly the the same as up except we grab the right child and it's subTree
    tree * rightChild = root->right;
    tree * subTree = rightChild->left;

    rightChild->left = root;
    root->left = subTree;

    root->height = max(height(root->left), height(root->right)) + 1;
    rightChild->height = max(height(rightChild->left), height(rightChild->right)) + 1;

    return rightChild;
}

int get_balance(tree * root)
{
    //Used to determine if the tree or subtree is unbalanced by comparing the max height of
    //  the left and right branches

    if (root != NULL)
    {
        return height(root->left) - height(root->right);
    }
    else
    {
        return 0;
    }
}

struct tree *insert(tree * root, zerg * zergUnit)
{
    //If we found a spot, create the node here
    if (root == NULL)
    {
        root = (tree *) malloc(sizeof(tree));

        if (root == NULL)
        {
            printf("Memory Error\n");
            return NULL;
        }
        else
        {
            root->node = zergUnit;
            root->left = root->right = NULL;
            root->visited = false;
            root->height = 1;
        }
    }
    else
    {
        int cmpValue = 0;
        cmpValue = id_compare(zergUnit->zergID, root->node->zergID);

        //If the symbol is the same, we just add the cents and update the name if a 
        //  name was provided
        if (cmpValue == 0)
        {
            //TODO: Seperate the status updates from the GPS updates

            if(!(root->node->HP > 0))
            {
                root->node->HP = zergUnit->HP; 
            }
            if(!(root->node->maxHP > 0))
            {
                root->node->maxHP = zergUnit->maxHP; 
            }
            if(!(root->node->longitude > 0))
            {
                root->node->longitude = zergUnit->longitude; 
            }
            if(!(root->node->latitude > 0))
            {
                root->node->latitude = zergUnit->latitude; 
            }
            if(!(root->node->altitude > 0))
            {
                root->node->altitude = zergUnit->altitude; 
            }
        }
        //Name is smaller than root so go to the left
        else if (cmpValue < 0)
        {
            root->left = insert(root->left, zergUnit);
        }
        //Name is bigger than root so go to the right
        else
        {
            root->right = insert(root->right, zergUnit);
        }
    }
    //See's if the left or right branches are balanced or not
    int balance = get_balance(root);

    //Checks for the types of imbalances and fixes them if they are

    //Zig Zig
    if (balance > 1 && (id_compare(zergUnit->zergID, root->left->node->zergID) < 0))
    {
        return right_rotate(root);
    }
    //Zag Zag
    if (balance < -1 && (id_compare(zergUnit->zergID, root->right->node->zergID) > 0))
    {
        return left_rotate(root);
    }
    //Zig Zag
    if (balance > 1 && (id_compare(zergUnit->zergID, root->left->node->zergID) > 0))
    {
        root->left = left_rotate(root->left);
        return right_rotate(root);
    }
    //Zag Zig
    if (balance < -1 && (id_compare(zergUnit->zergID, root->right->node->zergID) < 0))
    {
        root->right = right_rotate(root->right);
        return left_rotate(root);
    }

    //Return the new node
    return root;
}
