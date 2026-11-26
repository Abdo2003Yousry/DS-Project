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
    NextIceTime = 0;         
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
    SetHealth(TH);
    IceThreshold = 0.1 * TH; 
    //freeze limit = 10% of initial health
    //tower freezes when accumulated ice reaches 10% of its initial health
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
    if (Frozen)     return;

    //the freezing threshold of a tower can never be reached by a single ice shot
    if (ice > 0.5 * IceThreshold)
    {
        ice = 0.5 * IceThreshold;
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
////set freeze time = 3s


//// Fire ////

//Randomly decides between bullet and ice, 
//// with ice never exceeding 20% of all fires
//// At random time steps the tower fires ice
bool Tower::DecideFireType(int currentTime)
{
    //ice never exceed 20% of all fires
    int totalShots = BulletShots + IceShots;
    bool allowIce = false;
    if ((IceShots + 1) <= 0.2 * (totalShots + 1))
    {
        allowIce = true;
    }

    //time for the next ice fire
    bool timeForIce = currentTime >= NextIceTime;
    if (allowIce && timeForIce)
    {
        int wait = 1 + rand() % MaxIceWait;
        NextIceTime = currentTime + wait;
        return true;
    }

    return false;
}

//Computes the priority of one enemy using a formula
//with at least three factors (distance, power, type, remaining reload, ...)
//* priority = 10.0 / distance + power + typeWeight - remainingReload
double Tower::CalcPriority(Enemy* e, int currentTime) const
{
    double weight = 1; //Fighter_Type
    if (e->GetType() == Healer_Type)
    {
        weight = 2;
    }
    else if (e->GetType() == Freezer_Type)
    {
        weight = 3;
    }
    double closeness = 10.0 / e->GetDistance();
    double power = e->GetPower();
    double remaining = e->GetNextActionTime() - currentTime;
    if (remaining < 0)  remaining = 0;
    double priority = closeness + power + weight - remaining;
    return priority;
}

//Picks the N highest-priority enemies from the active list, 
//stores them in targets[], and returns their count
int Tower::SelectTargets(LinkedList<Enemy*>& active, Enemy* targets[], int currentTime)
{
    PriorityQueue<Enemy*> PriQueue;
    Node<Enemy*>* current = active.getHead();

    //loop over the enemies list nodes
    //set them and their priority into PQ
    while (current != nullptr)
    {
        Enemy* e = current->getItem();
        double priority = CalcPriority(e, currentTime);
        PriQueue.insert(e, priority);

        current = current->getNext();
    }

    int count = 0;
    Enemy* target;
    //get the highest priority items
    while (count < MaxEnemies && PriQueue.extractMax(target))
    {
        targets[count] = target; // for Fire()
        count++;
    }
    return count;
}


//// Attacking ////

//The main function. Returns if the tower is destroyed or frozen; 
//otherwise selects the targets and fires a bullet or ice at each one
void Tower::Fire(LinkedList<Enemy*>& active, int currentTime)
{
    if (IsDestroyed() || IsFrozen() || MaxEnemies <= 0)     return;

    Enemy** targets = new Enemy * [MaxEnemies]; //from input file

    int count = SelectTargets(active, targets, currentTime);

    for (int i = 0; i < count; i++)
    {
        if (DecideFireType(currentTime))
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

//Computes DTE and applies it to the enemy (K = 2 for healers), 
//and records the first shot time
//// Damage (tower -> enemy) = (1 / distance) * TP * (1 / K)
void Tower::FireBullet(Enemy* e, int currentTime)
{
    double k = 1;   //Other types
    if (e->GetType() == Healer_Type)    k = 2;

    double damage = (1.0 / e->GetDistance()) * Power * (1.0 / k);

    e->SetFirstShotTime(currentTime);
    e->TakeDamage(damage, currentTime);
    BulletShots++;
}

//Computes the freeze duration, freezes the enemy, and records the first shot time
//* freezeSteps = 3 if distance <= 10, otherwise 1 
void Tower::FireIce(Enemy* e, int currentTime)
{
    int freezeSteps = 1;
    if (e->GetDistance() <= 10)     freezeSteps = 3;

    e->SetFirstShotTime(currentTime);
    e->Freeze(freezeSteps, currentTime);
    IceShots++;
}