#pragma once


#include "Enemies/Enemy.h"
#include "Castle/Castle.h"
#include "Data_structure/LinkedQueue.h"
#include "Data_structure/LinkedList.h"

class GUI;

class Battle
{
private:
    Castle BCastle;

    // The input is sorted by arrival time, so this queue is efficient:
    // enqueue/dequeue/peek are O(1).
    struct InactiveEnemyRecord
    {
        Enemy* Ptr;
        int ArrivalTime;
        InactiveEnemyRecord() : Ptr(0), ArrivalTime(0) {}
        InactiveEnemyRecord(Enemy* enemy, int arrival)
            : Ptr(enemy), ArrivalTime(arrival) {}
    };

    LinkedQueue<InactiveEnemyRecord> InactiveEnemies;
    LinkedList<Enemy*> ActiveEnemies[NoOfRegions];

    // The GUI drawing interface requires one array containing all active enemies.
    Enemy* BEnemiesForDraw[MaxEnemyCount];
    int EnemyCount;

    int TotalEnemies;
    int KilledEnemies[NoOfRegions];
    int TimeStep;

    void MoveArrivedEnemies();
    int GetActiveCount() const;
    void KillRandomActiveEnemies(int maximumToKill);
    void RefreshDrawingArray();
    void PrintStatus(GUI* pGUI) const;
    void ClearAllEnemies();

public:
    Battle();
    ~Battle();

    bool AddInactiveEnemy(Enemy* Ptr, int arrivalTime = 0);
    void AddEnemy(Enemy* Ptr);
    void SetTowersData(double health, int maxEnemies, int towerPower);

    Castle* GetCastle();
    void RunSimulation();
};
