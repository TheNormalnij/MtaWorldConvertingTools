#pragma once

#include "CWriter.h"
#include <unordered_map>

#include "CIplLoader.h"

class CMapDataWriter : public CWriter
{
public:
    CMapDataWriter(std::filesystem::path path): CWriter(std::move(path)) {};

    void Write();

    void SetIplInfo(std::vector<SIplInfo> *ipl) { m_pIpl = ipl; };
    void SetColMap(std::unordered_map<std::string, std::pair<uint32_t, uint32_t>> *colMap) { m_pColMap = colMap; };
private:
    std::filesystem::path  m_path;
    std::vector<SIplInfo> *m_pIpl;
    std::unordered_map<std::string, std::pair<uint32_t, uint32_t>> *m_pColMap;
};
