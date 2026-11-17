#pragma once

#include "..\Defs.h"
#include "..\CMUgraphicsLib\CMUgraphics.h"
#include "..\GUI\GUI.h"

#include "Tower.h"

class Castle
{
	Tower Towers[NoOfRegions];

	//
	// TODO: Add More Data Members As Needed
	//
	
public:

	Castle();
	void SetTowerHealth(REGION TowerRegion, double h);

	// set  N, TP for all towers
	void SetTowersData( int N, double TP);

	// get the tower of this region
	Tower* GetTower(REGION r);

	// get health of the tower
	double GetTowerHealth(REGION r);

	// check if this tower is destroyed
	bool IsTowerDestroyed(REGION r);

	// check if all towers are destroyed
	bool AreAllTowersDestroyed();
	
	// get next region (A->B->C->D->A)
	REGION GetNextRegion(REGION r);

	// get total damage of the tower
	double GetTotalDamage(REGION r);

	// count towers that are still alive
	int GetNumberOfAliveTowers();
};
