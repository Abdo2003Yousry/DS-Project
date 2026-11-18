#pragma once

#include "..\Defs.h"
#include "..\CMUgraphicsLib\CMUgraphics.h"
#include "..\GUI\GUI.h"

#include "Tower.h"

class Castle
{
	Tower Towers[NoOfRegions];

	// next region (A->B->C->D->A) even if it is destroyed
	REGION GetNextRegion(REGION r) const;
	

public:
	    
		Castle();
		void SetTowerHealth(REGION r, double h);

		// set N, TP for all towers
		void SetTowersData(int N, double TP);

		// get the tower of this region
		Tower* GetTower(REGION r);

		// get health of the tower
		double GetTowerHealth(REGION r) const;

		// check if this tower is destroyed
		bool IsTowerDestroyed(REGION r) const;

		// check if all towers are destroyed
		bool AreAllTowersDestroyed() const;

		// count towers that are still alive
		int GetNumberOfAliveTowers() const;

		// first alive region after r (A->B->C->D->A), returns r if all are destroyed
		REGION GetNextAliveRegion(REGION r) const;

		// get total damage of the tower
		double GetTotalDamage(REGION r) const;

		
};
