#pragma once

#include <filesystem>
#include <fstream>

class CLoader
{
public:
    CLoader(std::filesystem::path path): m_path(std::move(path)) {};

    bool Open();
    void Close();

protected:
    void ReadLine(std::string &buff) { std::getline(m_stream, buff); };
    void ReadLineWithoutCommas(std::string &buff);
    std::ifstream m_stream;
private:
    std::filesystem::path m_path;
};
