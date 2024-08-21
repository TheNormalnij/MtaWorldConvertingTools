#pragma once

#include <vector>

#include "CWriter.h"

class CMetaWriter : public CWriter
{
public:
    CMetaWriter(std::filesystem::path path): CWriter(std::move(path)) {};

    void Write();
    void AddFile(const char* name);

private:
    std::vector<std::string> m_files;
};
