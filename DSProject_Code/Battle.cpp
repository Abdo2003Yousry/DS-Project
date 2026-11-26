#include "Battle.h"
#include "Enemies/Fighter.h"
#include "Enemies/Healer.h"
#include "Enemies/Freezer.h"
#include "FileManager.h"
#include "GUI/GUI.h"
#include <string>
#include <iostream>
#include <cstdlib>   // rand, srand
#include <ctime>     // time
#include <sstream>

Battle::Battle() : EnemyCount(0), TotalEnemies(0), TimeStep(0)
{
    for (int i = 0; i < NoOfRegions; i++)
        KilledCount[i] = 0;          // no killed enemies at the start

    for (int j = 0; j < MaxEnemyCount; j++)
        BEnemiesForDraw[j] = nullptr;   // no enemy to draw at the start
}

Battle::~Battle()
{
    ClearAllEnemies();
}


bool Battle::AddInactiveEnemy(Enemy* Ptr, int arrivalTime)
{
    if (Ptr == 0 || arrivalTime < 0 || TotalEnemies >= MaxEnemyCount)
        return false;

    InactiveEnemies.enqueue(Ptr);
    ++TotalEnemies;
    return true;
}

void Battle::AddEnemy(Enemy* Ptr)
{
    if (Ptr != 0)
        AddInactiveEnemy(Ptr, Ptr->GetArrivalTime());
}

void Battle::SetTowersData(double health, int maxEnemies, double towerPower)
{
    BCastle.SetTowersData(health, maxEnemies, towerPower);
}

Castle* Battle::GetCastle()
{
    return &BCastle;
}


int Battle::GetActiveCount() const
{
    int total = 0;
    for (int region = A_REG; region <= D_REG; ++region)
        total += ActiveEnemies[region].getCount();
    return total;
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

// number of active enemies of one type inside one region
int Battle::CountActiveOfType(REGION region, enemyType type) const
{
    int total = 0;
    for (Node<Enemy*>* p = ActiveEnemies[region].getHead(); p != 0; p = p->getNext())
        if (p->getItem()->GetType() == type)
            ++total;
    return total;
}

// time step, then one line per region:
// tower health, active enemies (fighters / healers / freezers), killed so far
void Battle::PrintStatus(GUI* pGUI) const
{
    std::string lines[NoOfRegions + 1];

    std::ostringstream header;
    header << "Time Step: " << TimeStep << "   (click to go to the next step)";
    lines[0] = header.str();

    for (int region = A_REG; region <= D_REG; ++region)
    {
        REGION r = static_cast<REGION>(region);
        std::ostringstream line;
        line << "Region " << static_cast<char>('A' + region)
            << " | Tower Health: " << BCastle.GetTowerHealth(r)
            << " | Active: " << ActiveEnemies[region].getCount()
            << " (Fighters: " << CountActiveOfType(r, Fighter_Type)
            << ", Healers: " << CountActiveOfType(r, Healer_Type)
            << ", Freezers: " << CountActiveOfType(r, Freezer_Type)
            << ") | Killed: " << KilledCount[region];
        lines[region + 1] = line.str();
    }

    pGUI->PrintStatusLines(lines, NoOfRegions + 1);
}
void Battle::RunSimulation()
{
    std::cout << "===== Castle Battle - Phase 1 =====\n";
    std::cout << "Enter input file name (example: input.txt): ";
    std::string filename;
    std::getline(std::cin >> std::ws, filename);

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
        RefreshDrawingArray();
        pGUI->DrawBattle(BEnemiesForDraw, EnemyCount);
        ++TimeStep;
    }

    RefreshDrawingArray();
    pGUI->DrawBattle(BEnemiesForDraw, EnemyCount);
    std::ostringstream finalStatus;
    int totalKilled = 0;
    for (int region = A_REG; region <= D_REG; ++region)
        totalKilled += KilledCount[region];

    finalStatus << "Phase 1 complete | Total: " << TotalEnemies
        << " | Killed: " << totalKilled;
    pGUI->PrintMessage(finalStatus.str());
    std::cout << "Phase 1 completed. Time steps: " << TimeStep << "\n";

    delete pGUI;
}


// move enemies whose arrival time came from Inactive to Active
void Battle::MoveArrivedEnemies()
{
    Enemy* e;

    // peek first, so we do not remove an enemy that did not arrive
    while (InactiveEnemies.peek(e) && e->GetArrivalTime() <= TimeStep)
    {
        InactiveEnemies.dequeue(e);

        e->SetState(Active);                    // now it is active
        ActiveEnemies[e->GetRegion()].insertEnd(e);   // list of its region
    }
}


// get the n active enemy, counting through the 4 regions
Enemy* Battle::GetActiveEnemyByIndex(int n) const
{
    for (int r = 0; r < NoOfRegions; r++)
    {
        int count = ActiveEnemies[r].getCount();

        if (n < count)    // the enemy is inside this region list
        {
            Node<Enemy*>* p = ActiveEnemies[r].getHead();
            for (int i = 0; i < n; i++)
                p = p->getNext();
            return p->getItem();
        }

        n -= count;       // skip this region, go to the next one
    }
    return nullptr;
}



// move one active enemy from its region list to the killed list
void Battle::KillEnemy(Enemy* e)
{
    ActiveEnemies[e->GetRegion()].remove(e);   // remove from active list

    e->SetState(Killed);
    e->SetKilledTime(TimeStep);

    // smallest kill time leaves first 
    KilledEnemies.insert(e, -(double)TimeStep * 100000);

    KilledCount[e->GetRegion()]++;             // per region
}




// kill at most maximumToKill random active enemies
void Battle::KillRandomActiveEnemies(int maximumToKill)
{
    for (int i = 0; i < maximumToKill; i++)
    {
        int activeCount = GetActiveCount();
        if (activeCount == 0)
            return;                          // nobody left to kill

        int n = rand() % activeCount;        // random index
        Enemy* e = GetActiveEnemyByIndex(n);
        KillEnemy(e);
    }
}



// delete every enemy that Battle still owns 
void Battle::ClearAllEnemies()
{
    Enemy* e;

    // 1) enemies that never arrived
    while (InactiveEnemies.dequeue(e))
        delete e;

    // 2) active enemies, list of each region
    for (int r = 0; r < NoOfRegions; r++)
        while (ActiveEnemies[r].removeFirst(e))
            delete e;

    // 3) killed enemies
    while (KilledEnemies.extractMax(e))
        delete e;

    EnemyCount = 0;
}




