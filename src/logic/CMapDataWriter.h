#pragma once

#include "CWriter.h"

#include "CIplLoader.h"

class CMapDataWriter : public CWriter
{
public:
    CMapDataWriter(std::filesystem::path path): CWriter(std::move(path)) {};

    void Write(std::vector<SIplInfo> &ipl);
private:
    std::filesystem::path  m_path;
};
