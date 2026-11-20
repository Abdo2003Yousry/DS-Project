#include "Fighter.h"



Fighter::Fighter(color r_c, REGION r_region, int r_id,
    int r_arrivalTime, double r_health, double r_power,
    int r_reloadPeriod, int d)
    : Enemy(r_c, r_region, r_id, r_arrivalTime,
        r_health, r_power, r_reloadPeriod, Fighter_Type, d)
{
}

void Fighter::Act(Tower* tower, Enemy* enemies[],
    int count, int currentTime)
{
    if (GetState() == Active && CanAct(currentTime))
    {
        double K = 1;

        if (GetHealth() < GetOriginalHealth() / 2)
            K = 0.5;

        double damage = (K * GetPower()) / GetDistance();

        // Send damage to tower
        //tower->TakeDamage(damage); wait to coorect name from somaya

        UpdateNextActionTime(currentTime);
    }
}



