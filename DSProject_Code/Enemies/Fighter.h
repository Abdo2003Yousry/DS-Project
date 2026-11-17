#pragma once

#include "..\Defs.h"
#include "..\CMUgraphicsLib\CMUgraphics.h"

#include "Enemy.h"


class Fighter : public Enemy {
public:
	Fighter(color r_c, REGION r_region, int d = MaxDistance);
    // Movement
    void Move() override;

    // Attack the tower
    void Act(Tower* tower, Enemy* enemies[],
        int count, int currentTime) override;
};