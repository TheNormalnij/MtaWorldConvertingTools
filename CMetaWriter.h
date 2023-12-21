#pragma once

#include <filesystem>
#include <string>
#include <vector>
#include <fstream>

class CMetaWriter
{
public:
    CMetaWriter(std::filesystem::path &path): m_path(path) {};

    bool Create();
    void Write();
    void Close();
    void AddFile(const char* name);

private:
    std::filesystem::path  m_path;
    std::vector<std::string> m_files;
    std::ofstream m_stream;
};
