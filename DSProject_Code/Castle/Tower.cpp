#include "Tower.h"


Tower::Tower()
{
	SetHealth(TowerInitHealth);
	
	Power = 0;
	MaxEnemies = 0;
	FrozenTimeLeft = 0;
	IceAccumulated = 0;
	FreezeThreshold = 0;
	IceFires = 0;
	TotalFires = 0;
	TotalDamage = 0;

}


void Tower::SetHealth(double h)
{
	if(h > 0)
		Health = h;
	else
		Health = 0; // killed
}

double Tower::GetHealth() const
{
	return Health;
}