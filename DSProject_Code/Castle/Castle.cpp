#include "Castle.h"

#include "..\GUI\GUI.h"

// Assigns each tower its region
Castle::Castle()
{
	for (int i = 0; i < NoOfRegions; i++)
		Towers[i].SetRegion((REGION)i);
}


// Gives every tower the same TH ,N and TP 
void Castle::SetTowersData(double TH, int N, double TP)
{
	for (int i = 0; i < NoOfRegions; i++)
	{
		Towers[i].SetMaxTargets(N);    // max enemies the tower can attack per time step
		Towers[i].SetFirePower(TP);    // tower power
		Towers[i].SetHealth(TH);        
	}
}

// get the tower of this region
Tower* Castle::GetTower(REGION r)
{
	return &Towers[r];
}

// get health of the tower
double Castle::GetTowerHealth(REGION r) const
{
	return Towers[r].GetHealth();
}

// check if this tower is destroyed
bool Castle::IsTowerDestroyed(REGION r) const
{
	return Towers[r].IsDestroyed();   // Health <= 0
}


// check if all towers are destroyed
bool Castle::AreAllTowersDestroyed() const
{
	return GetNumberOfAliveTowers() == 0;
}



// count towers that are still alive
int Castle::GetNumberOfAliveTowers() const
{
	int count = 0;   // number of alive towers

	for (int i = 0; i < NoOfRegions; i++)
		if (!Towers[i].IsDestroyed())   // tower is still alive
			count++;

	return count;
}


// Get the next region (A -> B -> C -> D -> A)
REGION Castle::GetNextRegion(REGION r) const
{
	return (REGION)((r + 1) % NoOfRegions);
}

// first alive region after r (A->B->C->D->A)
//Skips destroyed towers; returns r itself if all towers are destroyed
REGION Castle::GetNextAliveRegion(REGION r) const
{
REGION next = GetNextRegion(r);  

for (int i = 0; i < NoOfRegions; i++)
{
	if (!Towers[next].IsDestroyed())   
		return next;
	else
       next = GetNextRegion(next);        
}

return r;  //no alive tower
}

// get total damage of the tower
double Castle::GetTotalDamage(REGION r) const
{
	return Towers[r].GetTotalDamage();
}


