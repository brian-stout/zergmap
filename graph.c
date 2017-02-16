#include "graph.h"

double TO_RAD(double number)
{
    number *=(3.1415926536 / 180);

    return number;
}

bool are_doub_equal(double num1, double num2)
{
    double epsilon = 0.000001;

    if(abs(num1 - num2) < epsilon)
    {
        return false;
    }
    else
    {
        return true;
    }
}

//http://gis.stackexchange.com/a/4909
double haversine(zerg *unit1, zerg * unit2)
{
    if(are_doub_equal(unit1->latitude, unit2->latitude))
    {
        if(are_doub_equal(unit1->longitude, unit2->longitude))
        {
            return 0;
        }
    }

    double lat1 = TO_RAD(unit1->latitude);
    double lon1 = TO_RAD(unit1->longitude);
    double lat2 = TO_RAD(unit2->latitude);
    double lon2 = TO_RAD(unit2->longitude);


    double dLat = (lat2-lat1);
    double dLon = (lon2-lon1);
    double a;
    a = sin(dLat/2) * sin(dLat/2) + cos(lat1) * cos(lat2) * sin(dLon/2) * sin(dLon/2);
    double distance = 6371 * 2 * atan2(sqrt(a), sqrt(1-a));

    //Convert to meters
    distance *= 1000;

    return distance;
}


double find_distance(zerg *unit1, zerg * unit2)
{
    double surfaceDistance;

    surfaceDistance = haversine(unit1, unit2);

    if(are_doub_equal(unit1->altitude, unit2->altitude))
    {
        return surfaceDistance;
    }
    else
    {
        double altitudeDelta = abs(unit1->altitude - unit2->altitude);
        //Convert fathoms to meters
        altitudeDelta *= 1.8288;
        return sqrt(pow(surfaceDistance, 2) + pow(altitudeDelta, 2));
    }
}

struct graph * create_graph(list * listContainer)
{
    struct graph * newGraph = (struct graph *)malloc(sizeof(struct graph));
    
    if(!newGraph)
    {
        printf("Memory Error\n");
        return NULL;
    }
    int vertices;

    newGraph->vertices = vertices = listContainer->numberOfMembers;
    zerg ** unitMatrix = malloc(newGraph->vertices * sizeof(zerg));

    printf("%d\n", vertices);

    struct listNode * cursor = listContainer->first;

    for(int i = 0; i < vertices; ++i)
    {
        unitMatrix[i] = cursor->node;
        cursor = cursor->next;     
    }

    int **adj = (int **)malloc(newGraph->vertices * sizeof(int *));
    if(!adj)
    {
        printf("Memory error for matrix adj table! (1)\n");
        return NULL;
    }
    else
    {

        for(int i = 0; i < vertices; ++i)
        {
            adj[i] = (int *)malloc(vertices * sizeof(int));
            if(!adj[i])
            {
                printf("Memory error for matrix adj table! (2)(%d)\n", i);
            }
        }
    }
    
    //Run the edges here

    double distance;


    for(int i = 0; i < vertices; ++i)
    {
        for(int j = 0; j < vertices; ++j)
        {
            distance = find_distance(unitMatrix[i], unitMatrix[j]);
            //1.25 yards
            if(distance < 1.143)
            {
                adj[i][j] = -1;
            }
            else
            {
                adj[i][j] = (int)floor(distance);
            }
        }
    }
    newGraph->unitMatrix = unitMatrix;
    newGraph->adj = adj;

    return newGraph;
}

void print_matrix_table(struct graph * zergMap)
{
    int vertices = zergMap->vertices;
    printf("     ");
    for(int i = 0; i < vertices; ++i)
    {
        printf("%d  ", zergMap->unitMatrix[i]->zergID);
    }
    printf("\n");

    for(int i = 0; i < vertices; ++i)
    {
        printf("%d:", zergMap->unitMatrix[i]->zergID);
        for(int j = 0; j < vertices; ++j)
        {
            printf("  %d", zergMap->adj[i][j]);
        }
        printf("\n");
    }
}
