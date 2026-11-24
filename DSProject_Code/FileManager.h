#pragma once
#include <fstream>
#include <string>
#include "GameStatus.h"

// Forward declarations (so this header does not depend on the full classes)
class Battle;
class Enemy;

// FileManager
// Responsible ONLY for reading the input file and writing the output file.
// It does not contain any simulation/fighting logic.
class FileManager
{
private:
    std::ifstream inFile;        // input file stream
    std::ofstream outFile;       // output file stream
    bool endMarkerFound;         // true when the "-1" line was read
    bool lastReadFailed;         // true when the last enemy line was malformed

    // ---------------------------------------------------------------
    // Private helpers
    // ---------------------------------------------------------------

    // Converts a region letter ('A'..'D', upper or lower case) to an index
    // (0..3). Returns -1 if the letter is invalid.
    int CharToRegion(char regionChar) const;

    // Creates (with "new") the correct enemy object according to its type:
    // 1 = Fighter, 2 = Healer, 3 = Freezer. Returns nullptr if type is invalid.
    // Each enemy is allocated ONLY ONCE here; all lists store its pointer.
    Enemy* CreateEnemy(int type, int id, int arrivalTime, double health,
                       int power, int reloadPeriod, int region) const;

    // Checks that an enemy line has valid values (id >= 0, health > 0,
    // power >= 0, arrival time >= 0, reload >= 0, valid region).
    // Prints the reason to std::cerr and returns false if something is wrong.
    bool ValidateEnemyData(int id, int arrivalTime, double health,
                           int power, int reloadPeriod, int region) const;

    // Returns the text written in the output file for a game status:
    // "WIN", "LOSS" or "DRAWN".
    static std::string GetStatusText(GameStatus status);

    // Returns sum / count, or 0 when count is 0 (protects from division by 0).
    static double CalcAverage(double sum, int count);

public:
    FileManager();
    ~FileManager();   // closes any file that is still open

    // ---------------------------------------------------------------
    // INPUT (Phase 1 and Phase 2)
    // ---------------------------------------------------------------

    // Opens the input file. Returns false if the file does not exist
    // or cannot be opened.
    bool OpenInputFile(const std::string& filename);

    // Reads the first line of the file: TH (tower health), N (max enemies
    // a tower attacks per time step) and TP (tower power).
    // Returns false if the line is missing or invalid.
    bool ReadTowerData(double& towerHealth, int& maxEnemies, int& towerPower);

    // Reads ONE enemy line: "S TYP T H POW RLD REG".
    // Returns false when it reaches the "-1" line (end of input), the end of
    // the file, or a malformed line (use HasReachedEnd() to know which).
    bool ReadNextEnemyLine(int& id, int& type, int& arrivalTime, double& health,
                           int& power, int& reloadPeriod, char& regionChar);

    // True if the last call of ReadNextEnemyLine stopped because it
    // found the "-1" end marker (not because of an error).
    bool HasReachedEnd() const;

    // Main input function. Opens the file, reads the tower data, then reads
    // all enemies, creates them and puts them in Battle's inactive list.
    // Also checks that IDs are unique and arrival times are sorted.
    // Prints a warning if healers are more than 20% of all enemies.
    // Returns false if anything fails (nothing is added to Battle then).
    bool LoadInputFile(const std::string& filename, Battle& battle);

    // Closes the input file.
    void CloseInputFile();

    // ---------------------------------------------------------------
    // OUTPUT (Phase 2 only)
    // ---------------------------------------------------------------

    // Opens (creates) the output file for writing. Returns false on failure.
    bool OpenOutputFile(const std::string& filename);

    // Writes the header line of the killed-enemies table: "KTS S FD KD LT".
    void WriteKilledHeader();

    // Writes one line for a killed enemy: kill time step, ID, first-shot
    // delay, kill delay and lifetime.
    void WriteKilledEnemy(int kts, int id, int fd, int kd, int lt);

    // Writes the total damage done to each of the 4 towers
    // (header line + values line).
    void WriteTowersDamage(double t1, double t2, double t3, double t4);

    // Writes "Game is WIN", "Game is LOSS" or "Game is DRAWN".
    void WriteGameStatus(const std::string& status);

    // Writes the final statistics when the game is WON:
    // total number of enemies, average FD and average KD.
    void WriteWinStatistics(int totalEnemies, double avgFD, double avgKD);

    // Writes the final statistics when the game is LOST or DRAWN:
    // killed count, alive count (active + inactive), average FD and KD
    // (calculated for killed enemies only).
    void WriteLossDrawStatistics(int killedCount, int aliveCount,
                                 double avgFD, double avgKD);

    // Main output function. Takes everything it needs from Battle and writes
    // the whole file in the correct order: killed list, towers damage,
    // game status, then statistics. Returns false if the file can't be written.
    bool WriteOutputFile(const std::string& filename, Battle& battle);

    // Closes the output file.
    void CloseOutputFile();
};
