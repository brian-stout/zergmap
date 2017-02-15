#include "graph.h"

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
    newGraph->unitMatrix = malloc(newGraph->vertices * sizeof(zerg));

    printf("%d\n", vertices);

    struct listNode * cursor = listContainer->first;

    for(int i = 0; i < vertices; ++i)
    {
        newGraph->unitMatrix[i] = cursor->node;
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
    
    int testNumber = 0;

    //Run the edges here
    for(int i = 0; i < vertices; ++i)
    {
        for(int j = 0; j < vertices; ++j)
        {
            adj[i][j] = testNumber;
        }
    }

    return newGraph;
}
