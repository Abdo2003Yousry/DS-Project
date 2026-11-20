#pragma once

#include "..\Defs.h"
#include "..\CMUgraphicsLib\CMUgraphics.h"

#include "Enemy.h"


class Fighter : public Enemy {
public:
    Fighter(color r_c, REGION r_region, int r_id,
        int r_arrivalTime, double r_health, double r_power,
        int r_reloadPeriod, int d = MaxDistance);

    // Attack the tower
    void Act(Tower* tower, Enemy* enemies[],
        int count, int currentTime) override;
};