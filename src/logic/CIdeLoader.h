#pragma once

#include "CLoader.h"
#include "ILogger.h"
#include <vector>

struct SAtomicModelDef {
    uint32_t modelId;
    char modelName[24];
    char texDictName[24];
    float drawDist;
    uint32_t flags;
};

struct STimeModelDef : SAtomicModelDef {
    uint32_t on;
    uint32_t off;
};

struct SClumpModelDef : SAtomicModelDef {
    char animName[24];
};

class CIdeLoader : public CLoader
{
public:
    CIdeLoader(std::filesystem::path &&path, ILogger *log): CLoader(std::move(path)), m_log(log){};

    void Read(std::vector<SAtomicModelDef> &atomic, std::vector<STimeModelDef> &timed, std::vector<SClumpModelDef> &clump);
private:
    ILogger *m_log;
};
