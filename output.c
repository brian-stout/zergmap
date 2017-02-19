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
