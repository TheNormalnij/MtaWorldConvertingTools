#pragma once

#include <filesystem>
#include <vector>
#include <list>
#include <unordered_set>

#include "CIdeLoader.h"
#include "CIplLoader.h"
#include "CGtaDatLoader.h"
#include "CIMG.h"

#include "CColLib.h"

#include "ILogger.h"

namespace fs = std::filesystem;

struct SConverterParams {
    fs::path gtaPath;
    fs::path modPath;
    fs::path outputPath;
    bool genMeta;
    bool genRegister;
    bool packOneCol;
    bool removeLods;
    bool useLowerCase;
    bool optimiseModelsCount;
    bool checkCompability; // For engineLoadDFF
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

    void RemoveLods();
    void FilterUnusedModels();

    void OpenModIMGs();
    void CloseModIMGs();

    void GenerateColLib();

    void GetUsedTxd(std::unordered_set<std::string> &out);

    void WriteIMGs();
    size_t WriteIMG();
    void WriteMapInfo();
    void WriteMeta();

    void MakePath(const fs::path &root, const std::string &add, fs::path &out);

private:
    ILogger *m_log;
    const SConverterParams m_settings;

    std::vector<SGtaDatSection> m_modFiles;

    std::vector<SAtomicModelDef> m_atomic;
    std::vector<STimeModelDef> m_timed;
    std::vector<SClumpModelDef> m_clump;

    std::vector<SIplInfo> m_modMap;
    std::list<CIMG> m_modImgs;

    CColLib m_cols;

    std::unordered_set<std::string> m_usedModels;
};
