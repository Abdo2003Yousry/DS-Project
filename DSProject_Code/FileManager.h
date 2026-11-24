#pragma once

#include <string>
#include "Data_structure/LinkedList.h"
#include "Defs.h"

class Battle;

// Reads tower settings and creates the correctly derived enemy objects.
class FileManager
{
private:
    int CharToRegion(char regionChar) const;

public:
    bool LoadInputFile(const std::string& filename, Battle& battle);
};
