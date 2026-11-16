#pragma once

#include "..\Defs.h"
#include "..\CMUgraphicsLib\CMUgraphics.h"

#include "Enemy.h"


class Healer : public Enemy {
public :
	Healer(color r_c, REGION r_region, int d = MaxDistance);
    // Movement
    void Move() override;

    // Heal nearby enemies
    void Act(Tower* tower, Enemy* enemies[],
        int count, int currentTime) override;
};