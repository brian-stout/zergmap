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

        free(target->node);
        free(target);
    }

    free(listContainer);
}

double TO_RAD(double number)
{
    number *=(3.1415926536 / 180);

    return number;
}

double haversine(zerg *unit1, zerg * unit2)
{
    double lat1 = TO_RAD(unit1->latitude);
    double lon1 = TO_RAD(unit1->longitude);
    double lat2 = TO_RAD(unit2->latitude);
    double lon2 = TO_RAD(unit2->longitude);


    double dLat = (lat2-lat1);
    double dLon = (lon2-lon1);
    double a;
    a = sin(dLat/2) * sin(dLat/2) + cos(lat1) * cos(lat2) * sin(dLon/2) * sin(dLon/2);
    double distance = 6371 * 2 * atan2(sqrt(a), sqrt(1-a));

    return distance;
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
            printf(" is %lf\n", haversine(cursor->node, target->node)*1000);
            target = target->next;
        }

        cursor = cursor->next;
    }
}
