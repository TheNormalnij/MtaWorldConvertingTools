#pragma once

#include "CLoader.h"
#include <vector>
#include "CArrayStr.h"

struct SAtomicModelDef {
    uint32_t modelId;
    CArrayStr<20> modelName;
    CArrayStr<20> texDictName;
    float drawDist;
    uint32_t flags;
};

struct STimeModelDef : SAtomicModelDef {
    uint32_t on;
    uint32_t off;
};

struct SClumpModelDef : SAtomicModelDef {
    CArrayStr<20> animName;
};

class CIdeLoader : public CLoader
{
public:
    CIdeLoader(std::filesystem::path &&path): CLoader(std::move(path)) {};

    void Read(std::vector<SAtomicModelDef> &atomic, std::vector<STimeModelDef> &timed, std::vector<SClumpModelDef> &clump);
};
