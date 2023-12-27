#pragma once

#include "CLoader.h"
#include <vector>

struct SIplInfo {
    uint32_t modelId;
    char modelName[20];
    uint32_t interior;
    float x;
    float y;
    float z;
    float rx;
    float ry;
    float rz;
    float rw;
    int32_t lod;
};

class CIplLoader : public CLoader
{
public:
    CIplLoader(std::filesystem::path &&path): CLoader(std::move(path)) {};

    void Read(std::vector<SIplInfo> &out);
};
