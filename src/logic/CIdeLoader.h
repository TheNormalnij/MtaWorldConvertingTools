#pragma once

#include "CLoader.h"
#include <vector>

struct SAtomicModelDef {
    uint32_t modelId;
    char modelName[20];
    char texDictName[20];
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
    CIdeLoader(std::filesystem::path &&path): CLoader(std::move(path)) {};

    void Read(std::vector<SAtomicModelDef> &atomic, std::vector<STimeModelDef> &timed, std::vector<SClumpModelDef> &clump);
};
