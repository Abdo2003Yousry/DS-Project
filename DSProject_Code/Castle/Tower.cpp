#include "Tower.h"
#include "..\Enemies\Enemy.h"
#include "..\DS\PriorityQueue.h" 
#include <cstdlib>
#include <cmath>

//// Initialization ////

//Sets the tower to default values (zero) until the real data arrives
Tower::Tower()
{
	SetHealth(TowerInitHealth);
}

//Receives the input file data (region, TH, N, TP) and stores it in the attributes. 
//Called from Castle::SetTowersData
void Tower::SetData(REGION reg, double towerHealth, int numberEnemies, double towerPower)
{
	
}
////remove it and set the parameters in the constructor////


//// Getters & Setters ////

//Sets the health. If the value is zero or less, health becomes 0 (the tower is killed)
void Tower::SetHealth(double h)
{
	if(h > 0)
		Health = h;
	else
		Health = 0; // killed
}

//Returns the current health, used by the status bar
double Tower::GetHealth() const
{
	return Health;
}

//Returns the region this tower protects
REGION Tower::GetRegion() const
{
	return Region;
}

//Returns the tower power
double Tower::GetFirePower() const
{
	return FirePower;
}

//Returns N, the maximum number of enemies per time step
int Tower::GetMaxTargets() const
{
	return MaxTargets;
}

//Returns the total damage taken, 
//used by the output file
double Tower::GetTotalDamage() const
{
	return TotalDamage;
}

//Returns true if health is zero or less
bool Tower::IsDestroyed() const
{
	return Health <= 0;
}

//Returns true if the tower is frozen
bool Tower::IsFrozen() const
{
	return Frozen;
}


//// Attacks ////

//Decreases health and increases TotalDamage. 
//Called by the Fighter
void Tower::ReceiveDamage(double d)
{
	 
}
////called by enemy////

//Adds to the accumulated ice, and freezes the tower when the threshold is reached. 
//Called by the Freezer. Ignored if the tower is already frozen
void Tower::ReceiveIce(double ice)
{
	 
}
////called by enemy////
////check pers. if it is enough or not to freeze and freezed for how long////


//// Freeze ////

//Called at the start of each step to release the tower, 
//since freezing lasts one time step
void Tower::UpdateFreezeState()
{
	 
}


//// Fire ////

//Randomly decides between bullet and ice, 
//with ice never exceeding 20% of all fires
bool Tower::DecideFireType()
{
	
}

//Computes the priority of one enemy using a formula
//with at least three factors (distance, power, type, health, state, last shot, remaining reload, ...)
double Tower::CalcPriority(Enemy* e, int currentTime) const
{
	 
}
////create fomula based on distance////
////return N of list to fire so it can attacks////

//icks the N highest-priority enemies from the active list, 
//stores them in targets[], and returns their count
int Tower::SelectTargets(LinkedList<Enemy*>& active, Enemy* targets[], int currentTime)
{
	 
}
////


//// Attacking ////

//The main function. Returns if the tower is destroyed or frozen; 
//otherwise selects the targets and fires a bullet or ice at each one
void Tower::Fire(LinkedList<Enemy*>& active, int currentTime)
{
	 
}
////pick which type based on the %////

//Computes DTE and applies it to the enemy (K = 2 for healers), and records the first shot time
void Tower::FireBullet(Enemy* e, int currentTime)
{
	 
}
//// power * k /d ////

//Computes the freeze duration, freezes the enemy, and records the first shot time
void Tower::FireIce(Enemy* e, int currentTime)
{
	 
}
////e required