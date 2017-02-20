#include "output.h"

void print_zerg_unit(zerg *zergUnit)
{
    printf("zerg ID: %d \n", zergUnit->zergID);
    printf("Current HP: %ld \n", zergUnit->HP);
    printf("Max HP: %ld\n", zergUnit->maxHP);
    printf("longitude: %lf\n", zergUnit->longitude);
    printf("latitude: %lf\n", zergUnit->latitude);
    printf("altitude: %f\n", zergUnit->altitude);
}

void print_zerg_removal(struct graph * zergGraph)
{
    int vertices = zergGraph->vertices;
    bool changesRequired = false;



    for(int i = 0; i < vertices; ++i)
    {
        if(zergGraph->adj[i][i] < 0)
        {
            if(changesRequired == false)
            {
                printf("Network Alterations:\n");
            }
            changesRequired = true;
            printf("Remove zerg#%d\n", zergGraph->unitMatrix[i]->zergID);
        }
    }
    if(changesRequired == false)
    {
        printf("All zerg are in position!\n");
    }
}

bool print_status(tree * zergTree, int healthPercentage, bool printOnce)
{
    if(zergTree->left)
    {
        printOnce = print_status(zergTree->left, healthPercentage, printOnce);
    }

    if(zergTree->node)
    {
        zerg * zergUnit = zergTree->node;
        if(zergUnit->maxHP > 0)
        {
            double percentOfHealth;

            percentOfHealth = (double)zergUnit->HP/zergUnit->maxHP;
            percentOfHealth *= 100;

            if(percentOfHealth < healthPercentage)
            {
                if(printOnce == false)
                {
                    printf("Low Health (%d%%)\n", healthPercentage);
                    printOnce = true;
                }
                printf("zerg# %d\n", zergUnit->zergID);
            } 
        }
    }

    if(zergTree->right)
    {
        printOnce = print_status(zergTree->right, healthPercentage, printOnce);
    }

    return printOnce;    
}
