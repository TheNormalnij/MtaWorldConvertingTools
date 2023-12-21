#pragma once

#include <filesystem>
#include <fstream>

class CLoader
{
public:
    CLoader(std::filesystem::path &&path): m_path(path) {};

    bool Open();
    void Read() {};
    void Close();

protected:
    std::ifstream m_stream;
private:
    std::filesystem::path m_path;
};
