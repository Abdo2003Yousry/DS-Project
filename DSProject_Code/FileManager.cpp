#include "FileManager.h"
#include "Battle.h"
#include "../DSProject_Code/Enemies/Enemy.h"
#include "../DSProject_Code/Data_structure/LinkedQueue.h"
#include"../DSProject_Code/Data_structure/LinkedList.h"

#include <iostream>
#include <iomanip>

// -------------------------------------------------------------------
// Local helper (visible only in this file): checks whether an enemy ID
// was already used. Walks the list of IDs, so it is O(n) per check.
// -------------------------------------------------------------------
static bool ContainsID(const LinkedList<int>& ids, int id)
{
    for (Node<int>* p = ids.getHead(); p != nullptr; p = p->getNext())
        if (p->getItem() == id)
            return true;
    return false;
}

// ===================================================================
// Construction / destruction
// ===================================================================

FileManager::FileManager()
    : endMarkerFound(false), lastReadFailed(false)
{
}

FileManager::~FileManager()
{
    CloseInputFile();
    CloseOutputFile();
}

// ===================================================================
// Private helpers
// ===================================================================

// Converts 'A'..'D' (or 'a'..'d') to 0..3, otherwise -1.
int FileManager::CharToRegion(char regionChar) const
{
    switch (regionChar)
    {
    case 'A': case 'a': return 0;
    case 'B': case 'b': return 1;
    case 'C': case 'c': return 2;
    case 'D': case 'd': return 3;
    default:            return -1;
    }
}

// Creates the right enemy subclass. The returned pointer is the ONLY
// allocation of this enemy; it is passed around, never copied.
Enemy* FileManager::CreateEnemy(int type, int id, int arrivalTime,
                                double health, int power,
                                int reloadPeriod, int region) const
{
    switch (type)
    {
    case 1: return new Fighter(id, arrivalTime, health, power, reloadPeriod, region);
    case 2: return new Healer (id, arrivalTime, health, power, reloadPeriod, region);
    case 3: return new Freezer(id, arrivalTime, health, power, reloadPeriod, region);
    default: return nullptr;   // unknown type
    }
}

// Checks the numeric fields of one enemy line.
bool FileManager::ValidateEnemyData(int id, int arrivalTime, double health,
                                    int power, int reloadPeriod, int region) const
{
    if (id < 0)          { std::cerr << "Input error: negative enemy ID\n";                 return false; }
    if (arrivalTime < 0) { std::cerr << "Input error: enemy " << id << " has negative arrival time\n"; return false; }
    if (health <= 0)     { std::cerr << "Input error: enemy " << id << " has health <= 0\n"; return false; }
    if (power < 0)       { std::cerr << "Input error: enemy " << id << " has negative power\n"; return false; }
    if (reloadPeriod < 0){ std::cerr << "Input error: enemy " << id << " has negative reload period\n"; return false; }
    if (region < 0)      { std::cerr << "Input error: enemy " << id << " has an invalid region (use A-D)\n"; return false; }
    return true;
}

// "WIN" / "LOSS" / "DRAWN"
std::string FileManager::GetStatusText(GameStatus status)
{
    switch (status)
    {
    case GAME_WIN:  return "WIN";
    case GAME_LOSS: return "LOSS";
    default:        return "DRAWN";
    }
}

// Average that never divides by zero.
double FileManager::CalcAverage(double sum, int count)
{
    return (count > 0) ? sum / count : 0.0;
}

// ===================================================================
// INPUT
// ===================================================================

bool FileManager::OpenInputFile(const std::string& filename)
{
    CloseInputFile();                 // in case a file is already open
    inFile.clear();
    inFile.open(filename.c_str());
    endMarkerFound = false;
    lastReadFailed = false;
    return inFile.is_open();
}

// First line: TH N TP
bool FileManager::ReadTowerData(double& towerHealth, int& maxEnemies, int& towerPower)
{
    if (!(inFile >> towerHealth >> maxEnemies >> towerPower))
    {
        std::cerr << "Input error: the first line must contain: TH N TP\n";
        return false;
    }
    if (towerHealth <= 0 || maxEnemies <= 0 || towerPower <= 0)
    {
        std::cerr << "Input error: TH, N and TP must be greater than 0\n";
        return false;
    }
    return true;
}

// One enemy line: S TYP T H POW RLD REG
bool FileManager::ReadNextEnemyLine(int& id, int& type, int& arrivalTime,
                                    double& health, int& power,
                                    int& reloadPeriod, char& regionChar)
{
    endMarkerFound = false;
    lastReadFailed = false;

    // Read the first token. It is either an ID or the "-1" end marker.
    if (!(inFile >> id))
    {
        // Clean end of file (no "-1") is not a malformed line.
        lastReadFailed = !inFile.eof();
        return false;
    }
    if (id == -1)
    {
        endMarkerFound = true;
        return false;
    }

    // Read the other six fields.
    if (!(inFile >> type >> arrivalTime >> health >> power >> reloadPeriod >> regionChar))
    {
        std::cerr << "Input error: malformed enemy line (ID " << id << ")\n";
        lastReadFailed = true;
        return false;
    }
    return true;
}

bool FileManager::HasReachedEnd() const
{
    return endMarkerFound;
}

// Reads everything first into a temporary queue, validates it, and only
// then gives the enemies to Battle. This way a bad file leaves Battle clean.
bool FileManager::LoadInputFile(const std::string& filename, Battle& battle)
{
    if (!OpenInputFile(filename))
    {
        std::cerr << "Cannot open input file: " << filename << "\n";
        return false;
    }

    double towerHealth;
    int    maxEnemies, towerPower;
    if (!ReadTowerData(towerHealth, maxEnemies, towerPower))
    {
        CloseInputFile();
        return false;
    }

    LinkedQueue<Enemy*> pending;     // enemies read so far (not yet in Battle)
    LinkedList<int>     usedIDs;     // to detect duplicated IDs
    int  total = 0, healers = 0;
    int  lastArrival = 0;
    bool ok = true;

    int id, type, arrivalTime, power, reloadPeriod;
    double health;
    char regionChar;

    while (ReadNextEnemyLine(id, type, arrivalTime, health, power, reloadPeriod, regionChar))
    {
        int region = CharToRegion(regionChar);

        if (!ValidateEnemyData(id, arrivalTime, health, power, reloadPeriod, region))
        { ok = false; break; }

        if (ContainsID(usedIDs, id))
        { std::cerr << "Input error: duplicated enemy ID " << id << "\n"; ok = false; break; }

        // The inactive queue relies on arrival times being sorted.
        if (arrivalTime < lastArrival)
        { std::cerr << "Input error: enemy " << id << " is not sorted by arrival time\n"; ok = false; break; }

        Enemy* e = CreateEnemy(type, id, arrivalTime, health, power, reloadPeriod, region);
        if (e == nullptr)
        { std::cerr << "Input error: enemy " << id << " has an unknown type (use 1, 2 or 3)\n"; ok = false; break; }

        pending.enqueue(e);
        usedIDs.insertEnd(id);
        lastArrival = arrivalTime;
        total++;
        if (type == 2) healers++;
    }

    if (lastReadFailed) ok = false;                       // malformed line

    if (ok && !endMarkerFound)
        std::cerr << "Warning: the input file does not end with -1\n";

    if (ok && healers * 5 > total)                        // healers > 20%
        std::cerr << "Warning: healers are more than 20% of all enemies\n";

    if (!ok)
    {
        // Delete what we created so there is no memory leak.
        Enemy* e;
        while (pending.dequeue(e)) delete e;
        CloseInputFile();
        return false;
    }

    // Everything is valid: hand the data over to Battle.
    battle.SetTowersData(towerHealth, maxEnemies, towerPower);
    Enemy* e;
    while (pending.dequeue(e))
        battle.AddInactiveEnemy(e);

    CloseInputFile();
    return true;
}

void FileManager::CloseInputFile()
{
    if (inFile.is_open())
        inFile.close();
}

// ===================================================================
// OUTPUT
// ===================================================================

bool FileManager::OpenOutputFile(const std::string& filename)
{
    CloseOutputFile();
    outFile.clear();
    outFile.open(filename.c_str());
    return outFile.is_open();
}

void FileManager::WriteKilledHeader()
{
    outFile << "KTS S FD KD LT\n";
}

void FileManager::WriteKilledEnemy(int kts, int id, int fd, int kd, int lt)
{
    outFile << kts << " " << id << " " << fd << " " << kd << " " << lt << "\n";
}

void FileManager::WriteTowersDamage(double t1, double t2, double t3, double t4)
{
    outFile << "T1_Total_Damage T2_Total_Damage T3_Total_Damage T4_Total_Damage\n";
    outFile << t1 << " " << t2 << " " << t3 << " " << t4 << "\n";
}

void FileManager::WriteGameStatus(const std::string& status)
{
    outFile << "Game is " << status << "\n";
}

void FileManager::WriteWinStatistics(int totalEnemies, double avgFD, double avgKD)
{
    outFile << std::fixed << std::setprecision(2);       // 2 digits after the point
    outFile << "Total Enemies = " << totalEnemies << "\n";
    outFile << "Average First-Shot Delay = " << avgFD << "\n";
    outFile << "Average Kill Delay = " << avgKD << "\n";
    outFile.unsetf(std::ios::floatfield);                // back to default format
    outFile << std::setprecision(6);
}

void FileManager::WriteLossDrawStatistics(int killedCount, int aliveCount,
                                          double avgFD, double avgKD)
{
    outFile << std::fixed << std::setprecision(2);
    outFile << "Killed Enemies = " << killedCount << "\n";
    outFile << "Alive Enemies = " << aliveCount << "\n";
    outFile << "Average First-Shot Delay = " << avgFD << "\n";
    outFile << "Average Kill Delay = " << avgKD << "\n";
    outFile.unsetf(std::ios::floatfield);
    outFile << std::setprecision(6);
}

// Writes the whole output file in the order required by the project.
bool FileManager::WriteOutputFile(const std::string& filename, Battle& battle)
{
    if (!OpenOutputFile(filename))
    {
        std::cerr << "Cannot create output file: " << filename << "\n";
        return false;
    }

    // 1) Killed enemies, already sorted by (KTS, FD) by Battle.
    int killedCount = 0;
    Enemy** killed = battle.GetKilledEnemiesSorted(killedCount);

    WriteKilledHeader();
    double sumFD = 0, sumKD = 0;
    for (int i = 0; i < killedCount; i++)
    {
        Enemy* e = killed[i];
        WriteKilledEnemy(e->GetKillTime(), e->GetID(), e->GetFD(), e->GetKD(), e->GetLT());
        sumFD += e->GetFD();
        sumKD += e->GetKD();
    }
    delete[] killed;     // only the array; the enemies belong to Battle

    // 2) Total damage of each tower.
    WriteTowersDamage(battle.GetTowerDamage(0), battle.GetTowerDamage(1),
                      battle.GetTowerDamage(2), battle.GetTowerDamage(3));

    // 3) Game result.
    GameStatus status = battle.GetGameStatus();
    WriteGameStatus(GetStatusText(status));

    // 4) Statistics (averages use killed enemies only).
    double avgFD = CalcAverage(sumFD, killedCount);
    double avgKD = CalcAverage(sumKD, killedCount);

    if (status == GAME_WIN)
        WriteWinStatistics(battle.GetTotalEnemies(), avgFD, avgKD);
    else
        WriteLossDrawStatistics(killedCount, battle.GetAliveCount(), avgFD, avgKD);

    bool good = outFile.good();   // did every write succeed?
    CloseOutputFile();
    return good;
}

void FileManager::CloseOutputFile()
{
    if (outFile.is_open())
        outFile.close();
}
