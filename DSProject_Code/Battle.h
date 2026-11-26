
#pragma once

#include "Enemies/Enemy.h"
#include "Castle/Castle.h"

#include "Data_structure/LinkedQueue.h"
#include "Data_structure/LinkedList.h"
#include "Data_structure/PriorityQueue.h"

class GUI;

class Battle
{
private:
    Castle BCastle;

    // 1. Enemies that have not arrived yet
    LinkedQueue<Enemy*> InactiveEnemies;

    // 2. Active enemies: one LinkedList for each region
    LinkedList<Enemy*> ActiveEnemies[NoOfRegions];

    // 3. Killed enemies, sorted by priority
    PriorityQueue<Enemy*> KilledEnemies;

    // GUI drawing array
    Enemy* BEnemiesForDraw[MaxEnemyCount];
    int EnemyCount;

    // Simulation information
    int TotalEnemies;
    int KilledCount[NoOfRegions];
    int TimeStep;

    // Helper functions
    void MoveArrivedEnemies();
    int GetActiveCount() const;
    int CountActiveOfType(REGION region, enemyType type) const;
    void KillRandomActiveEnemies(int maximumToKill);
    void RefreshDrawingArray();
    void PrintStatus(GUI* pGUI) const;
    void ClearAllEnemies();

    Enemy* GetActiveEnemyByIndex(int n) const;
    void KillEnemy(Enemy* e);

public:
    Battle();
    ~Battle();

    bool AddInactiveEnemy(Enemy* Ptr, int arrivalTime = 0);
    void AddEnemy(Enemy* Ptr);

    void SetTowersData(double health, int maxEnemies, double towerPower);

    Castle* GetCastle();

    void RunSimulation();
};
