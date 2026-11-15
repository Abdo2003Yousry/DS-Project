#pragma once

#include "..\Defs.h"
#include "..\CMUgraphicsLib\CMUgraphics.h"

#include "Enemy.h"


class Freezer : public Enemy {
public:
	Freezer(color r_c, REGION r_region, int d = MaxDistance);
	void Move() override;	//All enemies can move
	void Act() override;	//Acting means fighting or healing
};