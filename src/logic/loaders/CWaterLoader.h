#pragma once

#include "CLoader.h"
#include <vector>

enum class EWaterPointType {
    TRIAGLE,
    RECTANGLE
};

struct SWaterPoint {
    float x;
    float y;
    float z;
    float w1;
    float w2;
    float w3;
    float w4;
};

struct SWaterInfo {
    EWaterPointType type;
    SWaterPoint vertices[4];
    uint8_t flags;

    bool IsShallow() const noexcept {
        //return flags & 2;
        // Disabled, because makes water element invisible
        return false;;
    }
};

class CWaterLoader : public CLoader
{
public:
    CWaterLoader(std::filesystem::path &&path): CLoader(std::move(path)) {};

    void Read(std::vector<SWaterInfo> &out);
};
