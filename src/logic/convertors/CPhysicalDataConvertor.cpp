#include "CPhysicalDataConvertor.h"
#include <unordered_map>

std::unordered_map<std::string, int32_t> FX_EFFECT {
    {"wallbust", 1},
    {"shootlight", 2},
    {"puke", 3},
    {"explosion_door", 4},
    {"explosion_crate", 5},
    {"explosion_barrel", 6},
    {"blood_heli", 7},
    {"tree_hit_palm", 8},
    {"tree_hit_fir", 9},
    {"water_swim", 10},
    {"water_splsh_sml", 11},
    {"water_splash_big", 12},
    {"water_splash", 13},
    {"water_hydrant", 14},
    {"tank_fire", 15},
    {"riot_smoke", 16},
    {"gunsmoke", 17},
    {"gunflash", 18},
    {"explosion_tiny", 19},
    {"explosion_small", 20},
    {"explosion_molotov", 21},
    {"explosion_medium", 22},
    {"explosion_large", 23},
    {"explosion_fuel_car", 24},
    {"exhale", 25},
    {"camflash", 26},
    {"prt_wake", 27},
    {"none", 28},
};

void CPhysicalDataConvertor::Convert(SMTAPhysicalInfo &out)
{
    std::unordered_map<SMtaPhysicalGroup, std::vector<std::string>> filtered{};

    for (const auto &entry : *m_gtaInfo) {
        const std::string lowerModelName = entry.modelName.GetLowerString();
        if (m_usedModels->contains(lowerModelName)) {
            SMtaPhysicalGroup mtaInfo {
                .mass = entry.mass,
                .turnMass = entry.turnMass,
                .airResistance = entry.airResistance,
                .elasticity = entry.elasticity,
                .percentSubmerge = entry.percentSubmerget,
                .uprootLimit = entry.uprootLimit,
                .colDamMultiplier = entry.colDamMultiplier,
                .avoidCamera = entry.avoidCamera,
                .causesExplosion = entry.causesExplosion,
                .fxOffsetX = entry.fxOffsetX,
                .fxOffsetY = entry.fxOffsetY,
                .fxOffsetZ = entry.fxOffsetZ,
                .hasBreakableInfo = entry.hasBreakableInfo,
                .smashMultiplier = entry.smashMultiplier,
                .breakVelX = entry.breakVelX,
                .breakVelY = entry.breakVelY,
                .breakVelZ = entry.breakVelZ,
                .breakVelRandom = entry.breakVelRandom,
                .produceSparks = entry.produceSparks
            };

            switch (entry.colDamageEffect) {
            case 1:
                mtaInfo.colDamageEffect = 2;
                break;
            case 20:
                mtaInfo.colDamageEffect = 3;
                break;
            case 21:
                mtaInfo.colDamageEffect = 4;
                break;
            case 200:
                mtaInfo.colDamageEffect = 5;
                break;
            case 201:
                mtaInfo.colDamageEffect = 6;
                break;
            default:
                mtaInfo.colDamageEffect = 1; // none
                break;
            }

            mtaInfo.fxType = entry.fxType + 1;
            mtaInfo.specialColResponseCases = entry.specialColResponseCases + 1;
            mtaInfo.fxEffect = FX_EFFECT[entry.fxEffect.GetLowerString()];

            if (mtaInfo.fxEffect == 0) {
                mtaInfo.fxEffect = 28;
                // LOG THIS
            }

            if (entry.hasBreakableInfo) {
                mtaInfo.gunBreakMode = entry.gunBreakMode + 1;
            } else {
                mtaInfo.gunBreakMode = 1;
            }

            auto list = filtered.find(mtaInfo);
            if (list == filtered.end()) {
                filtered[mtaInfo] = {lowerModelName};
            } else {
                list->second.push_back(lowerModelName);
            }
        }
    }

    for (const auto &info : filtered) {
        out.groups.push_back(info.first);
        out.models.push_back(info.second);
    }
}
