#include "output.h"

void print_zerg_unit(zerg *zerg_unit)
{
    printf("zerg ID: %d \n", zerg_unit->zergID);
    printf("Current HP: %ld \n", zerg_unit->HP);
    printf("Max HP: %ld\n", zerg_unit->maxHP);
    printf("longitude: %lf\n", zerg_unit->longitude);
    printf("latitude: %lf\n", zerg_unit->latitude);
    printf("altitude: %f\n", zerg_unit->altitude);
}

void print_zerg_removal(struct graph * zergGraph)
{
    int vertices = zergGraph->vertices;
    bool changesRequired = false;

    printf("Network Alterations:\n");

    for(int i = 0; i < vertices; ++i)
    {
        if(zergGraph->adj[i][i] < 0)
        {
            changesRequired = true;
            printf("Remove zerg#%d\n", zergGraph->unitMatrix[i]->zergID);
        }
    }
    if(changesRequired)
    {
        printf("All zerg are in position!\n");
    }
}
