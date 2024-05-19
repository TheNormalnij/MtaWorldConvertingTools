#include "CObjectDatLoader.h"

void CObjectsDatLoader::Read(std::vector<SObjectsInfo> &out) {

    std::string buff;
    while(!m_stream.eof()) {
        ReadLineWithoutCommas(buff);
        if (buff[0] == ';') {
            continue;
        }

        SObjectsInfo info{};
        const auto scanned = sscanf(buff.c_str(),
            "%s %f %f %f %f %f %f %f %d %d %d %d %d %f %f %f %s %f %f %f %f %f %d %d",
            info.modelName.data(),
            &info.mass,
            &info.turnMass,
            &info.airResistance,
            &info.elasticity,
            &info.percentSubmerget,
            &info.uprootLimit,
            &info.colDamMultiplier,
            &info.colDamageEffect,
            &info.specialColResponseCases,
            &info.avoidCamera,
            &info.causesExplosion,
            &info.fxType,
            &info.fxOffsetX,
            &info.fxOffsetY,
            &info.fxOffsetZ,
            info.fxEffect.data(),
            &info.smashMultiplier,
            &info.breakVelX,
            &info.breakVelY,
            &info.breakVelZ,
            &info.breakVelRandom,
            &info.gunBreakMode,
            &info.produceSparks);

        if (scanned != 17 && scanned != 24) {
            continue;
        }

        info.hasBreakableInfo = scanned == 24;

        out.emplace_back(std::move(info));
    }
}
