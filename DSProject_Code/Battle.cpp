#include "Battle.h"
#include "FileManager.h"
#include "GUI/GUI.h"

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <sstream>

Battle::Battle() : EnemyCount(0), TotalEnemies(0), TimeStep(0)
{
    for (int i = 0; i < NoOfRegions; ++i)
        KilledEnemies[i] = 0;
    for (int j = 0; j < MaxEnemyCount; ++j)
        BEnemiesForDraw[j] = 0;
}

Battle::~Battle()
{
    ClearAllEnemies();
}

void Battle::ClearAllEnemies()
{
    InactiveEnemyRecord record;
    while (InactiveEnemies.dequeue(record))
        delete record.Ptr;

    for (int region = A_REG; region <= D_REG; ++region)
    {
        Enemy* enemy = 0;
        while (ActiveEnemies[region].removeFirst(enemy))
            delete enemy;
    }
    EnemyCount = 0;
}

bool Battle::AddInactiveEnemy(Enemy* Ptr, int arrivalTime)
{
    if (Ptr == 0 || arrivalTime < 0 || TotalEnemies >= MaxEnemyCount)
        return false;

    InactiveEnemies.enqueue(InactiveEnemyRecord(Ptr, arrivalTime));
    ++TotalEnemies;
    return true;
}

void Battle::AddEnemy(Enemy* Ptr)
{
    if (Ptr != 0)
        AddInactiveEnemy(Ptr, Ptr->GetArrivalTime());
}

void Battle::SetTowersData(double health, int maxEnemies, int towerPower)
{
    BCastle.SetTowersData(maxEnemies, towerPower);
    for (int region = A_REG; region <= D_REG; ++region)
        BCastle.SetTowerHealth(static_cast<REGION>(region), health);
}

Castle* Battle::GetCastle()
{
    return &BCastle;
}

void Battle::MoveArrivedEnemies()
{
    InactiveEnemyRecord record;
    while (InactiveEnemies.peek(record) && record.ArrivalTime <= TimeStep)
    {
        InactiveEnemies.dequeue(record);
        Enemy* enemy = record.Ptr;
        REGION region = enemy->GetRegion();

        // Keep the GUI's vertical stack within its supported 15 slots.
        int distance = MaxDistance;
        for (; distance >= MinDistance; --distance)
        {
            int atDistance = 0;
            Node<Enemy*>* current = ActiveEnemies[region].getHead();
            while (current != 0)
            {
                if (current->getItem()->GetDistance() == distance)
                    ++atDistance;
                current = current->getNext();
            }
            if (atDistance < MaxVerticalEnemies)
                break;
        }
        if (distance < MinDistance)
            distance = MinDistance;

        enemy->SetDistance(distance);
        enemy->SetState(Active);
        ActiveEnemies[region].insertEnd(enemy);
    }
}

int Battle::GetActiveCount() const
{
    int total = 0;
    for (int region = A_REG; region <= D_REG; ++region)
        total += ActiveEnemies[region].getCount();
    return total;
}

void Battle::KillRandomActiveEnemies(int maximumToKill)
{
    for (int killed = 0; killed < maximumToKill; ++killed)
    {
        Enemy* candidates[MaxEnemyCount];
        int candidateCount = 0;

        for (int region = A_REG; region <= D_REG; ++region)
        {
            Node<Enemy*>* current = ActiveEnemies[region].getHead();
            while (current != 0 && candidateCount < MaxEnemyCount)
            {
                candidates[candidateCount++] = current->getItem();
                current = current->getNext();
            }
        }

        if (candidateCount == 0)
            return;

        Enemy* selected = candidates[std::rand() % candidateCount];
        REGION region = selected->GetRegion();
        if (ActiveEnemies[region].remove(selected))
        {
            selected->SetState(Killed);
            ++KilledEnemies[region];
            delete selected;
        }
    }
}

void Battle::RefreshDrawingArray()
{
    EnemyCount = 0;
    for (int region = A_REG; region <= D_REG; ++region)
    {
        Node<Enemy*>* current = ActiveEnemies[region].getHead();
        while (current != 0 && EnemyCount < MaxEnemyCount)
        {
            BEnemiesForDraw[EnemyCount++] = current->getItem();
            current = current->getNext();
        }
    }
}

void Battle::PrintStatus(GUI* pGUI) const
{
    std::ostringstream status;
    status << "Step: " << TimeStep;
    for (int region = A_REG; region <= D_REG; ++region)
    {
        status << " | " << static_cast<char>('A' + region)
            << " HP: " << BCastle.GetTowerHealth(static_cast<REGION>(region));
    }
    // Add each region's active and killed counts, as required by Phase 1.
    for (int region = A_REG; region <= D_REG; ++region)
    {
        status << " | " << static_cast<char>('A' + region)
            << " Active: " << ActiveEnemies[region].getCount()
            << " Killed: " << KilledEnemies[region];
    }
    status << " | Click to advance";
    pGUI->PrintMessage(status.str());
}

void Battle::RunSimulation()
{
    std::cout << "===== Castle Battle - Phase 1 =====\n";
    std::cout << "Enter input file name (example: input.txt): ";
    std::string filename;
    std::cin >> filename;

    FileManager fileManager;
    if (!fileManager.LoadInputFile(filename, *this))
    {
        std::cout << "Input loading failed. Simulation stopped.\n";
        return;
    }

    if (TotalEnemies == 0)
    {
        std::cout << "The input file contains no enemies.\n";
        return;
    }

    std::srand(static_cast<unsigned int>(std::time(0)));
    GUI* pGUI = new GUI;
    Point click;

    pGUI->PrintMessage("Phase 1 ready. Click to start.");
    pGUI->GetPointClicked(click);

    // Time starts at zero so enemies whose arrival time is zero are handled.
    while (!InactiveEnemies.isEmpty() || GetActiveCount() > 0)
    {
        MoveArrivedEnemies();
        RefreshDrawingArray();
        pGUI->DrawBattle(BEnemiesForDraw, EnemyCount);
        PrintStatus(pGUI);

        pGUI->GetPointClicked(click);
        KillRandomActiveEnemies(4);
        ++TimeStep;
    }

    RefreshDrawingArray();
    pGUI->DrawBattle(BEnemiesForDraw, EnemyCount);
    std::ostringstream finalStatus;
    int totalKilled = 0;
    for (int region = A_REG; region <= D_REG; ++region)
        totalKilled += KilledEnemies[region];

    finalStatus << "Phase 1 complete | Total: " << TotalEnemies
        << " | Killed: " << totalKilled;
    pGUI->PrintMessage(finalStatus.str());
    std::cout << "Phase 1 completed. Time steps: " << TimeStep << "\n";

    delete pGUI;
}
