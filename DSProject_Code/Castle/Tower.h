
#pragma once
#include "..\Defs.h"

class Enemy;

template <class T>
class LinkedList;

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
    REGION Region;

public:

    //// Initialization ////
    Tower();
    void SetData(REGION reg, double towerHealth, int numberEnemies, double towerPower);

    //new
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

	//
	// TODO: Add More Member Functions As Needed
	//

    //// Attacking ////
    void Fire(LinkedList<Enemy*>& active, int currentTime);
    void FireBullet(Enemy* e, int currentTime);
    void FireIce(Enemy* e, int currentTime);
};
