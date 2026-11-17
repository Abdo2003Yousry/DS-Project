#pragma once
#include "..\Defs.h"
#include "..\DS\LinkedList.h"
class Enemy;
class Tower
{
private:
	REGION Region;			// region this tower protects
	double Health;			// current health
	double FirePower;		// TP from input file
	int	MaxTargets;			// Maximum number of enemies attacked per time step
	bool Frozen;			// True while the tower is frozen
	double IceAccumulated;	// Ice thrown on the tower 
	double IceThreshold;	//Freeze when IceAccumulated reaches it
	double TotalDamage;		// From fighters 
	int BulletShots;		// Number of bullet fires
	int IceShots;			// Number of ice fires

public:

	//// Initialization ////
	Tower();
	void SetData(REGION reg, double towerHealth, int numberEnemies, double towerPower);
	// Getters & Setters
	REGION GetRegion() const;
	double GetHealth() const;
	double GetFirePower() const;
	int GetMaxTargets() const;
	double GetTotalDamage() const;
	bool IsDestroyed() const;
	bool isFrozen() const;

	//// Attacks ////
	void ReceiveDamage(double damg);
	void ReceiveIce(double ice);

	//// Freeze ////
	void UpdateFreezeState();

	//// Fire ////
	bool DecideFireType();
	double CalcPriority(Enemy* e) const;
	void SelectTargets(LinkedList<Enemy*>& active, Enemy* targets[], int& count);

	//// Attacking ////
	void Fire(LinkedList<Enemy*>& active, int currentTime);
	void FireBullet(Enemy* e, int currentTime);
	void FireIce(Enemy* e, int currentTime);
};
