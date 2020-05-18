#pragma once

#include <vector>
#include <string>
#include <fstream>

class FileReader
{
public:
    static std::vector<char> readFile(const std::string &filename);
};