#pragma once

#include "CWriter.h"
#include <unordered_map>

#include "../loaders/CIdeLoader.h"
#include "../loaders/CWaterLoader.h"
#include "../convertors/convertors.h"
#include "../convertors/CPhysicalDataConvertor.h"
#include "../CIMG.h"

class CMapDataWriter : public CWriter
{
public:
    CMapDataWriter(std::filesystem::path path): CWriter(std::move(path)) {};

    void Write();

    void SetIplInfo(std::vector<SMapObject> *map) { m_pMapObjects = map; };
    void SetColMap(std::unordered_map<std::string, std::pair<uint32_t, uint32_t>> *colMap) { m_pColMap = colMap; };

    void SetIMG(CIMG *img) { m_pIMG = img; };
    void SetAtomic(std::vector<SAtomicModelDef> *defs) { m_atomic = defs; };
    void SetTimed(std::vector<STimeModelDef> *defs) { m_timed = defs; };
    void SetClump(std::vector<SClumpModelDef> *defs) { m_clump = defs; };
    void SetDamageable(std::vector<SAtomicModelDef> *defs) { m_damageable = defs; };
    void SetWater(std::vector<SWaterInfo> *water) { m_water = water; };
    void SetPhysicalInfo(SMTAPhysicalInfo *info) { m_physicalInfo = info; };
private:
    void WriteWaterInfo();
    void WriteMapInfo();
    void WritePhysicalInfo();
    void WriteModelsInfo();
    void WriteModelInfoMap();
private:
    std::filesystem::path    m_path;
    std::vector<SMapObject> *m_pMapObjects;
    SMTAPhysicalInfo        *m_physicalInfo;
    std::unordered_map<std::string, std::pair<uint32_t, uint32_t>> *m_pColMap;

    std::vector<SAtomicModelDef> *m_atomic;
    std::vector<STimeModelDef>   *m_timed;
    std::vector<SClumpModelDef>  *m_clump;
    std::vector<SAtomicModelDef> *m_damageable;

    std::vector<SWaterInfo> *m_water;

    CIMG *m_pIMG;
};
