#pragma once
#include "..\Defs.h"
class Tower
{
	double Health;
	double Power;
	int MaxEnemies;
	int FrozenTimeLeft;
	double IceAccumulated;
	double FreezeThreshold;
	int IceFires;
	int TotalFires;
	double TotalDamage;

	//
	// TODO: Add More Data Members As Needed
	//

public:

	Tower();
	void SetHealth(double h);
	double GetHealth() const;


	// set max enemies per time step
	void SetMaxEnemies(int n);

	// get total damage
	double GetTotalDamage() const;

	// check if tower is destroyed
	bool IsDestroyed() const;

};

