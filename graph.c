#include "graph.h"

double TO_RAD(double number)
{
    number *=(3.1415926536 / 180);

    return number;
}

bool are_doub_equal(double num1, double num2)
{
    double epsilon = 0.000001;

    if(fabs(num1 - num2) < epsilon)
    {
        return true;
    }
    else
    {
        return false;
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

    //If the altitude is the same we only need to return the surface distance
    if(are_doub_equal(unit1->altitude, unit2->altitude))
    {

        return surfaceDistance;
    }
    else
    {
        double altitudeDelta = abs(unit1->altitude - unit2->altitude);
        //Convert fathoms to meters

        altitudeDelta *= 1.8288;
        if(surfaceDistance > 0)
        {
            //If both the altitude and surface distances are different
            //Calculate the hypotenuse to get the true distance
            return sqrt(pow(surfaceDistance, 2) + pow(altitudeDelta, 2));
        }
        else
        {
            //If the surface Distance is 0 then the distance is difference between the heights
            return altitudeDelta;
        }
    }
}

struct graph * create_graph(list * listContainer)
{
    //Mallocing memory
    struct graph * newGraph = (struct graph *)malloc(sizeof(struct graph));
    newGraph->vertices = 0;
    newGraph->deleted = 0;
    newGraph->unitMatrix = NULL;
    newGraph->adj = NULL;
    
    if(!newGraph)
    {
        printf("Memory Error\n");
        return NULL;
    }
    int vertices;

    //Runs through and graphs every node from the list and inserts it into the graph
    //A zerg unit array is used for quick access based off the adjacency array numbers
    newGraph->vertices = vertices = listContainer->numberOfMembers;
    zerg ** unitMatrix = malloc(newGraph->vertices * sizeof(zerg));

    struct listNode * cursor = listContainer->first;

    for(int i = 0; i < vertices; ++i)
    {
        unitMatrix[i] = cursor->node;
        cursor = cursor->next;     
    }

    //Malloc's memory for the adjacency matric
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
    
    double distance;

    for(int i = 0; i < vertices; ++i)
    {
        for(int j = 0; j < vertices; ++j)
        {
            distance = find_distance(unitMatrix[i], unitMatrix[j]);
            if(distance > 0)
            {
                //1.25 yards
                if(distance < 1.143)
                {
                    printf("Nodes are too close!\n");
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
                    printf("Nodes have the same coordinates\n");
                    //Set for deletion
                    //adj[i][i] = -1;
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

    //If the node is deleted return 0
    if(zergMap->adj[vert][vert] < 0)
    {
        return numOfAdj;
    }

    
    int vertices = zergMap->vertices;

    for(int i = 0; i < vertices; ++i)
    {
        //If there's an adjacent node increment the amount
        //Greater than 0 to make sure it doesn't count deleted nodes
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
        if(zergMap->adj[vert][i] > 0 && zergMap->adj[vert][i] < 15)
        {
            //Once and adjacent node has been found return it
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
        //Sets all the edge information to 0
        zergMap->adj[i][vert] = -1;
        zergMap->adj[vert][i] = -1;
    }

    zergMap->deleted++;
    return zergMap;
}

bool graph_solveable(struct graph * zergMap)
{
    if(zergMap->deleted > (zergMap->vertices/2))
    {
        return false;
    }
    else
    {
        return true;
    }
}

struct graph * remove_leafs(struct graph * zergMap, int vert)
{
    int adj_node;

    //If there's only one adjacent node
    if(1 == return_adj_num(zergMap, vert))
    {
        //Check is used to stop a two node solution from being dismantled
        if((zergMap->vertices - zergMap->deleted) > 2)
        {
            //Grabs the only adjacent node for further use
            adj_node = return_first_adj(zergMap, vert);
            //Deletes the adjacent node
            zergMap = delete_node(zergMap, vert);

            //Only runs if there is an adjacent node
            if(adj_node > 0)
            {
                //Recursively calls the function to see if deleting this node
                //Creates another leaf
                zergMap = remove_leafs(zergMap, adj_node);
            }
        }
    }

    return zergMap;
}

struct graph * cleanup_graph(struct graph * zergMap)
{
    int vertices = zergMap->vertices;

    for(int i = 0; i < vertices; ++i)
    {
        //If a function is set to delete '-1'
        if(zergMap->adj[i][i] < 0)
        {
            zergMap = delete_node(zergMap, i);
        }

        //If we've deleted too many nodes for it to be solveable just quit out
        if(!graph_solveable(zergMap))
        {
            return zergMap;
        }

        //Delete all the leaves in the graph
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

int check_for_weakness(struct graph * zergGraph)
{
    int vertices = zergGraph->vertices;

    //Checking every node in the graph
    for(int i = 0; i < vertices; ++i)
    {
        //If node has more than 2 adjacencies it can potentially be a choke point
        if(return_adj_num(zergGraph, i) > 2)
        {
            //If it is check to see if there is a route around that node
            for(int j = 0; j < vertices; ++j)
            {
                for(int k = 0; k < vertices; ++k)
                {
                    //If function returns false, there's no way around the specified node
                    //Thus it's a choke point
                    if(find_route(zergGraph, j, k, i) == false)
                    {
                        return i;
                    }
                }
            }
        }
    }

    return -1;
}

bool find_route(struct graph * zergGraph, int point1, int point2, int weakness)
{
    int vertices = zergGraph->vertices;
    int **adj = zergGraph->adj;
    //route * newRoute = NULL;
    int nextPoint = 0;

    //newRoute = add_node(newRoute, point1);

    while(true)
    {
        nextPoint = -1;
        int nextPointDistance = 9999;

        for(int i = 0; i < vertices; ++i)
        {
            if(nextPoint == point1)
            {
                adj[i][i] = 1;
                nextPoint = -1;
                continue;                
            }
            //Checks to see if node hasn't already been visited '1'
            //Checks to see if node has been deleted '-1'
            if(adj[i][i] == 1 && adj[i][i] < 0)
            {
                adj[i][i] = 1;
                continue;
            }
            //Checks to see if node is adjacent 'distance is less than 15' 
            //Checks to see if edge hasn't been deleted '-1'
            if(adj[point1][i] > 0 && adj[point1][i] < 15)
            {
                //Finds the distance to the goal, picks the next adjacent point that's closest
                // to the goal
                if(adj[point2][i] < nextPointDistance)
                {
                    nextPointDistance = adj[point2][i];

                    if(i != weakness)
                    {
                        nextPoint = i;
                    }
                    adj[i][i] = 1;
                }                    
            }
        }

        //newRoute = add_node(newRoute, nextPoint);

        //Can't find path
        if(nextPoint == -1)
        {
            for(int i = 0; i < vertices; ++i)
            {
                if(adj[i][i] > 0)
                {
                    adj[i][i] = 0;
                }
            }
            return false;
        }
        //Found the goal
        if(nextPoint == point2)
        {
            for(int i = 0; i < vertices; ++i)
            {
                if(adj[i][i] > 0)
                {
                    adj[i][i] = 0;
                }
            }
            return true;
        }  

        point1 = nextPoint; 
    }
}

struct graph * solve_weakness(struct graph * zergGraph, int weakness)
{
    int vertices = zergGraph->vertices;
    int **adj = zergGraph->adj;
    
    for(int i = 0; i < vertices; ++i)
    {
        if(adj[weakness][i] < 15 && adj[weakness][i] > 0)
        {
            //Sets one of the adjacent nodes to -1 one, if it's a choke point
            //This will cause a cascading effect from the cleanup_graph function
            adj[i][i] = -1;
            break;
        }
    }

    return zergGraph;
}



