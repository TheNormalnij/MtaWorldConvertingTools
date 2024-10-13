#pragma once

#include <filesystem>
#include <vector>
#include <list>
#include <unordered_set>

#include "loaders/CIdeLoader.h"
#include "loaders/CIplLoader.h"
#include "loaders/CGtaDatLoader.h"
#include "loaders/CWaterLoader.h"
#include "convertors/convertors.h"
#include "convertors/CPhysicalDataConvertor.h"
#include "CIMG.h"

#include "CColLib.h"

#include "ILogger.h"

namespace fs = std::filesystem;

struct SConverterParams {
    fs::path gtaPath;
    fs::path modPath;
    fs::path outputPath;
    std::string worldName;
    bool removeLods;
    bool genMeta;
    bool genRegister;
};

class CConverter
{
public:
    CConverter(ILogger *logger, SConverterParams *settings);

    void Convert();
private:

    bool LoadModGtaDat();
    bool LoadModModelDefs();
    bool LoadModIpls();
    bool LoadModWaterData();
    bool LoadModPhysicalInfo();

    void RemoveBrokenModels();
    void RemoveLods();
    void ReorderLods();
    void FilterUnusedModels();

    bool OpenModIMGs();
    void CloseModIMGs();

    void GenerateColLib();

    void GetUsedTxd(std::unordered_set<std::string> &out);

    void WriteIMGs();
    size_t WriteIMG();
    void WriteMapInfo();
    void WriteMeta();
    void WriteRegister();

    void MakePath(const fs::path &root, const std::string &add, fs::path &out);

    void ConvetMapInfoToMTA();

private:
    ILogger *m_log;
    const SConverterParams m_settings;

    std::vector<SGtaDatSection> m_modFiles;

    std::vector<SAtomicModelDef> m_atomic;
    std::vector<STimeModelDef> m_timed;
    std::vector<SClumpModelDef> m_clump;

    std::vector<SWaterInfo> m_waterinfo;
    std::vector<SIplInfo> m_modMap;
    std::vector<SMapObject> m_mtaMap;
    std::unordered_map<std::string, std::pair<uint32_t, uint32_t>> m_colMap;
    std::list<CIMG> m_modImgs;

    CColLib m_cols;

    std::unordered_set<std::string> m_usedModels;
    SMTAPhysicalInfo m_physical;
};
