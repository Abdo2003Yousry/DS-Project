
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


    int FreezeEndTime;       // Time when freezing ends

    int FirstShotTime;       // First time enemy was hit
    int KilledTime;          // Time enemy was killed
    

public:

    // Constructor and destructor
    Enemy(color r_c, REGION r_region, int r_id, int r_arrivalTime,
        double r_health, double r_power, int r_reloadPeriod,
        enemyType r_type, int d = MaxDistance);
    virtual ~Enemy();

    // Existing functions
    color GetColor() const; //Done
    REGION GetRegion() const;//Done

    void DecrementDist();//Done

    void SetDistance(int d);//Done
    int GetDistance() const;//Done

    // Getters
    int GetID() const;//Done
    double GetHealth() const;//Done
    double GetOriginalHealth() const;//Done
    double GetPower() const;//Done

    int GetArrivalTime() const;//Done
    int GetReloadPeriod() const;//Done
    int GetNextActionTime() const;//Done
    enemyType GetType() const;
    enemyState GetState() const;//Done

    // Setters
    void SetID(int id);//Done
    void SetHealth(double health);//Done
    void SetOriginalHealth(double health);//Done
    void SetPower(double power);//Done

    void SetArrivalTime(int time);//Done
    void SetReloadPeriod(int period);//Done
    void SetState(enemyState state);//Done

  
    // Statistics
    void SetFirstShotTime(int time);//Done
    void SetKilledTime(int time);//Done

    int GetFD() const; //done
    int GetKD() const; //done
    int GetLT() const; //done

    // Movement
    void Move(int currentTime) ;//Done
    bool CanMove(int currentTime) const;//Done

    // Actions
    virtual void Act(Tower* tower, Enemy* enemies[],
        int count, int currentTime) = 0;

    bool CanAct(int currentTime) const;//Done
    void UpdateNextActionTime(int currentTime);//Done

    // Health
    void TakeDamage(double damage, int currentTime);//Done
    void Heal(double amount);//Done
    bool IsKilled() const;//Done

    // Freeze
    void Freeze(int duration, int currentTime);//Done
    bool IsFrozen(int currentTime) const;//Done
};
