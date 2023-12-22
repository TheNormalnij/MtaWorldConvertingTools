#pragma once

#include <filesystem>
#include <vector>

#include "CIdeLoader.h"
#include "CGtaDatLoader.h"

#include "ILogger.h"

namespace fs = std::filesystem;

struct SConverterParams {
    fs::path gtaPath;
    fs::path modPath;
    fs::path outputPath;
    bool genMeta;
    bool genRegister;
    bool packOneCol;
    bool deleteLods;
    bool useLowerCase;
    bool optimiseModelsCount;
    bool checkCompability; // For engineLoadDFF
};

class CConverter
{
public:
    CConverter(ILogger *logger, SConverterParams &settings);

    void Convert();


private:

    bool LoadModGtaDat();
    bool LoadModModelDefs();
    bool LoadModIpls();

    size_t WriteIMG();
    void WriteMapInfo();
    void WriteMeta();

    void MakePath(const fs::path &root, const std::string &add, fs::path &out);

private:
    ILogger *m_log;
    const SConverterParams m_settings;

    std::vector<SGtaDatSection> m_modFiles;

    std::vector<std::string> m_modIdes;
    std::vector<std::string> m_modeIpls;

    std::vector<SAtomicModelDef> m_atomic;
    std::vector<STimeModelDef> m_timed;
    std::vector<SClumpModelDef> m_clump;

    size_t m_usedImgs;
};
