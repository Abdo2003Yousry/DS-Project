
#pragma once

#include "..\Defs.h"
#include "..\CMUgraphicsLib\CMUgraphics.h"

// Enemies colors
const color FIGHTER_CLR = DARKBLUE;
const color HEALER_CLR = ORANGERED;

// Forward declaration
class Tower;

// Enemy is the base class of each type of enemy
class Enemy
{
protected:

    int ID;                  // Unique enemy ID
    color Clr;               // Enemy color
    REGION Region;           // Enemy region
    int Distance;            // Distance from tower (2 to 60)

    double Health;           // Current health
    double OriginalHealth;   // Original health
    double Power;            // Enemy power

    enemyState State;        // Current enemy state
    enemyType Type;          // Enemy type

    int ArrivalTime;         // Enemy arrival time
    int ReloadPeriod;        // Time between actions
    int LastActionTime;      // Last time enemy performed an action

    int MovementSteps;       // Controls movement speed

    int FreezeEndTime;       // Time when freezing ends

    int FirstShotTime;       // First time enemy was hit
    int KilledTime;          // Time enemy was killed

public:

    // Constructor and destructor
    Enemy(color r_c, REGION r_region, int d = MaxDistance);
    virtual ~Enemy();

    // Existing functions
    color GetColor() const;
    REGION GetRegion() const;

    void DecrementDist();

    void SetDistance(int d);
    int GetDistance() const;

    // Getters
    int GetID() const;
    double GetHealth() const;
    double GetOriginalHealth() const;
    double GetPower() const;

    int GetArrivalTime() const;
    int GetReloadPeriod() const;
    int GetNextActionTime() const;

    enemyState GetState() const;

    // Setters
    void SetID(int id);
    void SetHealth(double health);
    void SetOriginalHealth(double health);
    void SetPower(double power);

    void SetArrivalTime(int time);
    void SetReloadPeriod(int period);
    void SetState(enemyState state);

    // Movement
    virtual void Move() = 0;
    bool CanMove(int currentTime) const;

    // Actions
    virtual void Act(Tower* tower, Enemy* enemies[],
        int count, int currentTime) = 0;

    bool CanAct(int currentTime) const;
    void UpdateNextActionTime(int currentTime);

    // Health
    void TakeDamage(double damage);
    void Heal(double amount);
    bool IsKilled() const;

    // Freeze
    void Freeze(int duration, int currentTime);
    bool IsFrozen(int currentTime) const;

    // Statistics
    void SetFirstShotTime(int time);
    void SetKilledTime(int time);

    int GetFD() const;
    int GetKD() const;
    int GetLT() const;
};
