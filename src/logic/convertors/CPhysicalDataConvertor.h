
#include <cstring>
#include <vector>
#include <unordered_set>

#pragma once

#include <cstdint>

#include "../loaders/CObjectDatLoader.h"

struct SMtaPhysicalGroup
{
    float mass;
    float turnMass;
    float airResistance;
    float elasticity;
    float percentSubmerge;
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
    int32_t fxEffect;
    bool hasBreakableInfo;
    float smashMultiplier;
    float breakVelX;
    float breakVelY;
    float breakVelZ;
    float breakVelRandom;
    int32_t gunBreakMode;
    int32_t produceSparks;

    bool operator==(const SMtaPhysicalGroup &o) const {
        return std::memcmp(this, &o, sizeof(SMtaPhysicalGroup)) == 0;
    };
};

template<> struct std::hash<SMtaPhysicalGroup> {
    std::size_t operator()(const SMtaPhysicalGroup& obj) const {
        std::size_t seed = 0;
        seed ^= std::hash<float>()(obj.mass) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        seed ^= std::hash<float>()(obj.turnMass) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        seed ^= std::hash<float>()(obj.airResistance) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        seed ^= std::hash<float>()(obj.elasticity) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        seed ^= std::hash<float>()(obj.percentSubmerge) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        seed ^= std::hash<float>()(obj.uprootLimit) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        seed ^= std::hash<float>()(obj.colDamMultiplier) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        seed ^= std::hash<uint32_t>()(obj.colDamageEffect) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        seed ^= std::hash<uint32_t>()(obj.specialColResponseCases) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        seed ^= std::hash<uint32_t>()(obj.avoidCamera) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        seed ^= std::hash<uint32_t>()(obj.causesExplosion) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        seed ^= std::hash<uint32_t>()(obj.fxType) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        seed ^= std::hash<float>()(obj.fxOffsetX) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        seed ^= std::hash<float>()(obj.fxOffsetY) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        seed ^= std::hash<float>()(obj.fxOffsetZ) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        seed ^= std::hash<uint32_t>()(obj.fxEffect) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        seed ^= std::hash<bool>()(obj.hasBreakableInfo) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        seed ^= std::hash<float>()(obj.smashMultiplier) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        seed ^= std::hash<float>()(obj.breakVelX) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        seed ^= std::hash<float>()(obj.breakVelY) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        seed ^= std::hash<float>()(obj.breakVelZ) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        seed ^= std::hash<float>()(obj.breakVelRandom) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        seed ^= std::hash<uint32_t>()(obj.gunBreakMode) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        seed ^= std::hash<uint32_t>()(obj.produceSparks) + 0x9e3779b9 + (seed << 6) + (seed >> 2);

        return seed;
    }
};

struct SMTAPhysicalInfo {
    std::vector<SMtaPhysicalGroup> groups;
    std::vector<std::vector<std::string> > models;
    std::vector<std::vector<uint32_t> > defs;
};

class CPhysicalDataConvertor {
public:
    CPhysicalDataConvertor(std::vector<SObjectsInfo> *gtaObjectInfo, std::unordered_set<std::string> *usedModels): m_gtaInfo(gtaObjectInfo), m_usedModels(usedModels) {};
    ~CPhysicalDataConvertor() = default;

    void Convert(SMTAPhysicalInfo &out);

private:
    std::vector<SObjectsInfo> *m_gtaInfo;
    std::unordered_set<std::string> *m_usedModels;
};
