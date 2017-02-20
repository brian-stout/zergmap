#include "list.h"

list * init_list(void)
{
    list * newList = (list *)malloc(sizeof(list));
    newList->numberOfMembers = 0;
    newList->first = NULL;

    return newList;
}

list * add_zerg(list * listContainer, zerg * zergUnit)
{
    struct listNode * newNode = (struct listNode*)malloc(sizeof(struct listNode));
    newNode->node = zergUnit;

    if(listContainer->first == NULL)
    {
        listContainer->first = newNode;
        newNode->next = NULL;
    }
    else
    {
        struct listNode * temp;
        temp = listContainer->first;

        listContainer->first = newNode;
        newNode->next = temp;
    }

    listContainer->numberOfMembers += 1;

    return listContainer;
}

void print_list(list * listContainer)
{
    if (listContainer == NULL)
    {
        return;
    }

    if (listContainer->first == NULL)
    {
        return;
    }

    struct listNode * cursor = listContainer->first;

    while(cursor)
    {
        printf("ZergID: %d \n", cursor->node->zergID);
        cursor = cursor->next;
    }
}

void destroy_list(list * listContainer)
{
    if(listContainer == NULL)
    {
        return;
    }

    if(listContainer->first == NULL)
    {
        free(listContainer);
        return;
    }
    
    struct listNode * cursor = listContainer->first;
    struct listNode * target;

    while(cursor)
    {
        target = cursor;
        cursor = cursor->next;

        if(target->node)
        {
            free(target->node);
        }
        if(target)
        {
            free(target);
        }
    }

    free(listContainer);
}

void compare_list(list * listContainer)
{
    if(listContainer == NULL)
    {
        return;
    }

    if(listContainer->first == NULL)
    {
        free(listContainer);
        return;
    }

    struct listNode * cursor = listContainer->first;
    struct listNode * target;

    while(cursor)
    {
        target = cursor->next;
        while(target)
        {
            printf("The distance between unit %d", cursor->node->zergID);
            printf(" and unit %d", target->node->zergID);
            printf(" is %lf\n", haversine(cursor->node, target->node));
            target = target->next;
        }

        cursor = cursor->next;
    }
}
