#include "Healer.h"

#include "Healer.h"

Healer::Healer(color r_c, REGION r_region, int d)
    : Enemy(r_c, r_region, d)
{
}
void Healer::Move()
{
}

void Healer::Act(Tower* tower, Enemy* enemies[],
    int count, int currentTime)
{
}