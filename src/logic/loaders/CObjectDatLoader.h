#pragma once

#include "CLoader.h"
#include "../CArrayStr.h"
#include <vector>

struct SObjectsInfo {
    CArrayStr<20> modelName;
    float mass;
    float turnMass;
    float airResistance;
    float elasticity;
    float percentSubmerget;
    float uprootLimit;
    float colDamMultiplier;
    int32_t colDamageEffect;
    int32_t specialColResponseCases;
    int32_t avoidCamera;
    int32_t causesExplosion;
    int32_t fxType;
    float fxOffsetX;
    float fxOffsetY;
    float fxOffsetZ;
    CArrayStr<20> fxEffect;
    bool hasBreakableInfo;
    float smashMultiplier;
    float breakVelX;
    float breakVelY;
    float breakVelZ;
    float breakVelRandom;
    int32_t gunBreakMode;
    int32_t produceSparks;
};

class CObjectsDatLoader : public CLoader {
public:
    CObjectsDatLoader(std::filesystem::path &&path): CLoader(std::move(path)) {};

    void Read(std::vector<SObjectsInfo> &out);
};
