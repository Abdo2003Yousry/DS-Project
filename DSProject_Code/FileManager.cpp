#include "FileManager.h"
#include "Battle.h"
#include "Enemies/Fighter.h"
#include "Enemies/Healer.h"
#include "Enemies/Freezer.h"

#include <fstream>
#include <iostream>
#include <cctype>

int FileManager::CharToRegion(char regionChar) const
{
    switch (static_cast<char>(std::toupper(static_cast<unsigned char>(regionChar))))
    {
    case 'A': return A_REG;
    case 'B': return B_REG;
    case 'C': return C_REG;
    case 'D': return D_REG;
    default:  return -1;
    }
}

bool FileManager::LoadInputFile(const std::string& filename, Battle& battle)
{
    std::ifstream input(filename.c_str());
    if (!input.is_open())
    {
        std::cerr << "Cannot open input file: " << filename << "\n";
        return false;
    }

    double towerHealth;
    int maxEnemies, towerPower;
    if (!(input >> towerHealth >> maxEnemies >> towerPower) ||
        towerHealth <= 0 || maxEnemies <= 0 || towerPower <= 0)
    {
        std::cerr << "Invalid header. Expected: TH N TP (all positive).\n";
        return false;
    }

    LinkedList<int> usedIDs;
    int lastArrival = -1;
    int id, type, arrivalTime, power, reloadPeriod;
    double health;
    char regionChar;
    bool markerFound = false;

    while (input >> id)
    {
        if (id == -1)
        {
            markerFound = true;
            break;
        }

        if (!(input >> type >> arrivalTime >> health >> power >> reloadPeriod >> regionChar))
        {
            std::cerr << "Malformed enemy record for ID " << id << ".\n";
            return false;
        }

        const int region = CharToRegion(regionChar);
        if (id < 0 || type < 1 || type > 3 || arrivalTime < 0 ||
            health <= 0 || power < 0 || reloadPeriod < 0 || region < 0)
        {
            std::cerr << "Invalid data in enemy record ID " << id << ".\n";
            return false;
        }
        if (arrivalTime < lastArrival)
        {
            std::cerr << "Enemy records must be sorted by arrival time.\n";
            return false;
        }

        Node<int>* used = usedIDs.getHead();
        while (used != 0 && used->getItem() != id)
            used = used->getNext();
        if (used != 0)
        {
            std::cerr << "Duplicate enemy ID: " << id << "\n";
            return false;
        }
        if (usedIDs.getCount() >= MaxEnemyCount)
        {
            std::cerr << "Input exceeds MaxEnemyCount (" << MaxEnemyCount << ").\n";
            return false;
        }

        REGION enemyRegion = static_cast<REGION>(region);
        color enemyColor = FIGHTER_CLR;
        Enemy* enemy = 0;

        if (type == 1)
            enemy = new Fighter(enemyColor, enemyRegion, id, arrivalTime,
                health, power, reloadPeriod);
        else if (type == 2)
        {
            enemyColor = HEALER_CLR;
            enemy = new Healer(enemyColor, enemyRegion, id, arrivalTime,
                health, power, reloadPeriod);
        }
        else
        {
            enemyColor = GREEN;
            enemy = new Freezer(enemyColor, enemyRegion, id, arrivalTime,
                health, power, reloadPeriod);
        }

        enemy->SetState(Inactive);
        if (!battle.AddInactiveEnemy(enemy, arrivalTime))
        {
            delete enemy;
            std::cerr << "Could not add enemy to Battle.\n";
            return false;
        }

        usedIDs.insertEnd(id);
        lastArrival = arrivalTime;
    }

    if (!markerFound)
    {
        std::cerr << "Input file must end with -1.\n";
        return false;
    }

    battle.SetTowersData(towerHealth, maxEnemies, towerPower);
    return true;
}

