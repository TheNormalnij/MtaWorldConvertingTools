#pragma once

#include <filesystem>
#include <fstream>

class CWriter
{
public:
    CWriter(std::filesystem::path path): m_path(std::move(path)) {};

    bool Create();
    void Close();

protected:
    std::ofstream m_stream;
private:
    std::filesystem::path m_path;
};
