#pragma once
#include "..\Defs.h"

class Enemy;

template <class T>
class LinkedList;

class Tower
{
private:
	REGION Region;			// region this tower protects
    double InitHealth; // initial health
    double Health;			// current health
    double Power;   		// TP from input file
    int MaxEnemies;			// Maximum number of enemies attacked per time step
	bool Frozen;			// True while the tower is frozen
	double IceAccumulated;	// Ice thrown on the tower 
	double IceThreshold;	//Freeze when IceAccumulated reaches it
	double TotalDamage;		// From fighters 
	int BulletShots;		// Number of bullet fires
	int IceShots;			// Number of ice fires
    int NextIceTime;        // time step when the next ice fire is allowed

    /*bool DecideFireType();
    double CalcPriority(Enemy* e, int currentTime) const;
    int SelectTargets(LinkedList<Enemy*>& active, Enemy* targets[], int currentTime);*/

public:

    //// Initialization ////
    Tower();
    void SetData(REGION reg, double towerHealth, int numberEnemies, double towerPower);

    void SetRegion(REGION r);
    void SetMaxTargets(int n);
    void SetFirePower(double p);

    //// Getters & Setters ////
    void SetHealth(double h);
    double GetHealth() const;
    REGION GetRegion() const;
    double GetFirePower() const;
    int GetMaxTargets() const;
    double GetTotalDamage() const;
    bool IsDestroyed() const;
    bool IsFrozen() const;

    //// Attacks ////
    void ReceiveDamage(double d);
    void ReceiveIce(double ice);

    //// Freeze ////
    void UpdateFreezeState();

    //// Attacking ////
    void Fire(LinkedList<Enemy*>& active, int currentTime);
    void FireBullet(Enemy* e, int currentTime);
    void FireIce(Enemy* e, int currentTime);
    // add missing fun by abdelrhman
    // Fire
    bool DecideFireType(int currentTime);
    double CalcPriority(Enemy* e, int currentTime) const;
    int SelectTargets(LinkedList<Enemy*>& active, Enemy* targets[], int currentTime);
};
