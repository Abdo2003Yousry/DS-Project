
#include "Tower.h"
#include "..\Enemies\Enemy.h"
#include "..\Data_structure\LinkedList.h"
#include <cstdlib>
#include <cmath>

//// Initialization ////

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

void Tower::SetData(REGION reg, double towerHealth, int numberEnemies, double towerPower)
{
    Region = reg;
    SetHealth(towerHealth);
    MaxEnemies = numberEnemies;
    Power = towerPower;
}

//// Getters & Setters ////

void Tower::SetHealth(double h)
{
    if (h > 0)
        Health = h;
    else
        Health = 0;
}

double Tower::GetHealth() const
{
    return Health;
}

REGION Tower::GetRegion() const
{
    return Region;
}

double Tower::GetFirePower() const
{
    return Power;
}

int Tower::GetMaxTargets() const
{
    return MaxEnemies;
}

double Tower::GetTotalDamage() const
{
    return TotalDamage;
}

bool Tower::IsDestroyed() const
{
    return Health <= 0;
}

bool Tower::IsFrozen() const
{
    return FrozenTimeLeft > 0;
}

//// Attacks ////

void Tower::ReceiveDamage(double d)
{
}

void Tower::ReceiveIce(double ice)
{
}

//// Freeze ////

void Tower::UpdateFreezeState()
{
}

//// Fire ////

bool Tower::DecideFireType()
{
    return false;
}

double Tower::CalcPriority(Enemy* e, int currentTime) const
{
    return 0;
}

int Tower::SelectTargets(LinkedList<Enemy*>& active, Enemy* targets[], int currentTime)
{
    return 0;
}

//// Attacking ////

void Tower::Fire(LinkedList<Enemy*>& active, int currentTime)
{
}

void Tower::FireBullet(Enemy* e, int currentTime)
{
}

void Tower::FireIce(Enemy* e, int currentTime)
{
}
