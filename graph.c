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
    //TODO: PROBLEMS:
    //1. This function does not handle different nodes with the same coordinates
    //2. This function does not handle nodes to close
    //3. The problem doesn't handle duplicate Zerg IDs (This needs to be fixed)
    //      probably with a tree, cause yolo.

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
            //TODO:Rework this to be more efficient and not as messy
            if(distance > 0)
            {
                //1.25 yards
                if(distance < 1.143)
                {
                    //Set for adjacency check
                    adj[i][i] = -1;
                    //Does the same thing as same longitude and latitude because
                    //  I have no way to resolve which one to delete yet
                }
                else
                {
                    adj[i][j] = (int)floor(distance);
                }
            }
            else
            {
                //Checks for nodes compared to other nodes
                //If it's zero distance and they're different nodes
                //  One needs to be deleted.
                if(i == j)
                {
                    //[i][i] is the field for that node's metadata
                    adj[i][i] = 0;
                }
                else
                {
                    //Set for deletion
                    adj[i][i] = -1;
                    adj[j][i] = 0; //no path
                }
            }
        }
    }
    newGraph->unitMatrix = unitMatrix;
    newGraph->adj = adj;

    return newGraph;
}

int return_adj_num(struct graph * zergMap, int vert)
{
    int numOfAdj = 0;

    if(zergMap->adj[vert][vert] < 0)
    {
        return numOfAdj;
    }

    int vertices = zergMap->vertices;

    for(int i = 0; i < vertices; ++i)
    {
        if(zergMap->adj[vert][i] > 0 && zergMap->adj[vert][i] < 15)
        {
            numOfAdj++;
        }        
    }

    return numOfAdj;
}

int return_first_adj(struct graph * zergMap, int vert)
{
    int vertices = zergMap->vertices;

    for(int i = 0; i < vertices; ++i)
    {
        if(zergMap->adj[vert][i] > 0)
        {
            return i;
        }
    }
    return 0;
}
/*
struct * graph remove_interfering_units(struct graph * zergMap)
{
    int vertices = zergMap->vertices;

    for(int i = 0; i < vertices; ++i)
    {
        
    }
}
*/


struct graph * delete_node(struct graph * zergMap, int vert)
{
    int vertices = zergMap->vertices;

    for(int i = 0; i < vertices; ++i)
    {
        zergMap->adj[i][vert] = -1;
        zergMap->adj[vert][i] = -1;
    }

    return zergMap;
}

struct graph * remove_leafs(struct graph * zergMap, int vert)
{
    int adj_node;

    if(1 == return_adj_num(zergMap, vert))
    {
        adj_node = return_first_adj(zergMap, vert);
        zergMap = delete_node(zergMap, vert);
        if(adj_node > 0)
        {
            printf("Yes there's an adjacent node!\n");
            zergMap = remove_leafs(zergMap, vert);
        }
    }

    return zergMap;
}


struct graph * cleanup_graph(struct graph * zergMap)
{
    int vertices = zergMap->vertices;

    for(int i = 0; i < vertices; ++i)
    {
        if(zergMap->adj[i][i] < 0)
        {
            zergMap = delete_node(zergMap, i);
        }

        zergMap = remove_leafs(zergMap, i);
    }   

    return zergMap;
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

    for(int i = 0; i < vertices; ++i)
    {
        printf("ZergID: %d", zergMap->unitMatrix[i]->zergID);
        printf(" has %d adjacenies\n", return_adj_num(zergMap, i));
    }
}
