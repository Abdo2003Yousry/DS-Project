#include "Tower.h"
#include "..\Enemies\Enemy.h"
#include "..\Data_structure\LinkedList.h"
#include "..\Data_structure\PriorityQueue.h"
#include <cstdlib>
#include <cmath>

//// Initialization ////

//Sets the tower to default values (zero) until the real data arrives
Tower::Tower()
{
	Region = A_REG;			    // region this tower protects
	InitHealth = 0;		        // initial health
	Health = 0;				    // current health
	Power = 0;	    		    // TP from input file
    MaxEnemies = 0;				// Maximum number of enemies attacked per time step
	Frozen = false;			    // True while the tower is frozen
	IceAccumulated = 0;		    // Ice thrown on the tower 
	IceThreshold = 0;		    //Freeze when IceAccumulated reaches it
	TotalDamage = 0;			// From fighters 
	BulletShots = 0;			// Number of bullet fires
	IceShots = 0;				// Number of ice fires
	SetHealth(InitHealth);
}

//Receives the input file data (region, TH, N, TP) and stores it in the attributes. 
//Called from Castle::SetTowersData
void Tower::SetData(REGION region, double TH, int N, double TP)
{
    Region = region;
    MaxEnemies = N;
    Power = TP;
    InitHealth = TH;
    IceThreshold = 0.1 * TH;
    SetHealth(TH);
}

//// Getters & Setters ////

void Tower::SetRegion(REGION r)
{
    Region = r;
}
void Tower::SetMaxTargets(int n)
{
    MaxEnemies = n;
}
void Tower::SetFirePower(double p)
{
    Power = p;
}

//////////////////////////////////

//Sets the health. If the value is zero or less, health becomes 0 (the tower is killed)
void Tower::SetHealth(double h)
{
    if (h > 0)
        Health = h;
    else
        Health = 0;
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
    return Power;
}

//Returns N, the maximum number of enemies per time step
int Tower::GetMaxTargets() const
{
    return MaxEnemies;
}

//Returns the total damage taken, 
//used by the output file
double Tower::GetTotalDamage() const
{
    return TotalDamage;
}

//Returns true if health is zero or less
//// Health <= 0
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

//Decreases health and increases TotalDamage. Called by the Fighter
//// Damage (fighter -> tower) = (K / distance) * power
void Tower::ReceiveDamage(double d)
{
    SetHealth(Health - d);
    TotalDamage += d;
}

//Adds to the accumulated ice, and freezes the tower when the threshold is reached. 
//Called by the Freezer. Ignored if the tower is already frozen
void Tower::ReceiveIce(double ice)
{
    if (Frozen)
    {
        return;
    }

    IceAccumulated += ice;
    if (IceAccumulated >= IceThreshold)
    {
        Frozen = true;
        IceAccumulated = 0;
    }
}


//// Freeze ////

//Called at the start of each step to release the tower, 
//since freezing lasts one time step
void Tower::UpdateFreezeState()
{
    Frozen = false;
}


//// Fire ////

//Randomly decides between bullet and ice, 
//// with ice never exceeding 20% of all fires
//// At random time steps the tower fires ice
bool Tower::DecideFireType()
{
    int totalShots = BulletShots + IceShots;
    bool allowIce = false;
    if ((IceShots + 1) <= 0.2 * (totalShots + 1))
    {
        allowIce = true;
    }

    double randomShot = rand() % 100;
    bool chance = false;
    if (randomShot < 20)
    {
        chance = true;
    }

    return allowIce && chance;
}

//Computes the priority of one enemy using a formula
//with at least three factors (distance, power, type, health, state, last shot, remaining reload, ...)
//* priority = 10 * (1 / distance) + power + 2 * typeWeight - remainingReload
double Tower::CalcPriority(Enemy* e, int currentTime) const
{
    double typeWeight = 1;
    if (e->GetType() == Healer_Type)
    {
        typeWeight = 3;
    }
    else if (e->GetType() == Freezer_Type)
    {
        typeWeight = 2;
    }
    
    double distanceScore = 10.0 / e->GetDistance();
    double powerScore = e->GetPower();
    double typeScore = 2 * typeWeight;
    double reloadPenalty = e->GetRemainingReload(currentTime);
    double priority = distanceScore + powerScore + typeScore - reloadPenalty;
    return priority;
}

//Picks the N highest-priority enemies from the active list, 
//stores them in targets[], and returns their count
int Tower::SelectTargets(LinkedList<Enemy*>& active, Enemy* targets[], int currentTime)
{
    PriorityQueue<Enemy*> pq;

    Node<Enemy*>* current = active.GetHead();  
    while (current != NULL)                    
    {
        Enemy* e = current->GetItem();        
        double priority = CalcPriority(e, currentTime);  
        pq.Enqueue(e, priority);              
        current = current->GetNext();       
    }

    int count = 0;
    Enemy* target;
    while (count < MaxEnemies && pq.Dequeue(target))
    {
        targets[count] = target;
        count++;
    }
    return count;
}


//// Attacking ////

//The main function. Returns if the tower is destroyed or frozen; 
//otherwise selects the targets and fires a bullet or ice at each one
void Tower::Fire(LinkedList<Enemy*>& active, int currentTime)
{
    if (IsDestroyed() || IsFrozen() || MaxEnemies <= 0)
    {
        return;
    }

    Enemy** targets = new Enemy * [MaxEnemies];
    int count = SelectTargets(active, targets, currentTime);

    for (int i = 0; i < count; i++)
    {
        if (DecideFireType())
        {
            FireIce(targets[i], currentTime);
        }
        else
        {
            FireBullet(targets[i], currentTime);
        }
    }
    delete[] targets;
}

//Computes DTE and applies it to the enemy (K = 2 for healers), and records the first shot time
//// Damage (tower -> enemy) = (1 / distance) * TP * (1 / K)
void Tower::FireBullet(Enemy* e, int currentTime)
{
    double K = 1;
    if (e->GetType() == HEALER)
    {
        K = 2;
    }

    double damage = (1.0 / e->GetDistance()) * Power * (1.0 / K);

    e->SetFirstShot(currentTime); 
    e->TakeDamage(damage);
    BulletShots++;
}

//Computes the freeze duration, freezes the enemy, and records the first shot time
void Tower::FireIce(Enemy* e, int currentTime)
{
    int freezeSteps = (int)ceil(Power / e->GetDistance());
    if (freezeSteps < 1)
    {
        freezeSteps = 1;
    }
    if (freezeSteps > 5)
    {
        freezeSteps = 5;
    }

    e->SetFirstShot(currentTime); 
    e->Freeze(freezeSteps);
    IceShots++;
}