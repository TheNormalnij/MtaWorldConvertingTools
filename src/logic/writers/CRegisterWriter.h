
#pragma once

#include "CWriter.h"

class CRegisterWriter : public CWriter
{
public:
    CRegisterWriter(std::filesystem::path path, std::string worldName): CWriter(std::move(path)), m_worldName(worldName) {};

    void Write();

private:
    std::string m_worldName;
};
