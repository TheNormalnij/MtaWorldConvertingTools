#include "CMapDataWriter.h"

void CMapDataWriter::Write()
{
    Printf("-- Generated with converter version 3.0\nreturn {\n");
    WriteModelsInfo();

    WriteMapInfo();
    WriteWaterInfo();
    WritePhysicalInfo();

    Printf("}\n");
}

void CMapDataWriter::WriteWaterInfo() {
    // Water
    Printf("\twater = {\n");
    for (const SWaterInfo &info : *m_water) {
        if (info.type == EWaterPointType::TRIAGLE) {
            Printf("\t\t{ %f, %f, %f, %f, %f, %f, %f, %f, %f, %s };\n",
                   info.vertices[0].x,
                   info.vertices[0].y,
                   info.vertices[0].z,
                   info.vertices[1].x,
                   info.vertices[1].y,
                   info.vertices[1].z,
                   info.vertices[2].x,
                   info.vertices[2].y,
                   info.vertices[2].z,
                   info.IsShallow() ? "true" : "false"
                   );
        } else {
            Printf("\t\t{ %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %s };\n",
                   info.vertices[0].x,
                   info.vertices[0].y,
                   info.vertices[0].z,
                   info.vertices[1].x,
                   info.vertices[1].y,
                   info.vertices[1].z,
                   info.vertices[2].x,
                   info.vertices[2].y,
                   info.vertices[2].z,
                   info.vertices[3].x,
                   info.vertices[3].y,
                   info.vertices[3].z,
                   info.IsShallow() ? "true" : "false"
                   );
        }
    }

    Printf("\t};\n");
}

void CMapDataWriter::WriteMapInfo() {
    // Map
    Printf("\tmap = {\n");
    // { 10226, 1654, 0, -2798.16, -2799.47, -73.4996, -0.0, 0.0, -0.0, false,  };
    // { modelId, ?, interior, x, y, z, rx, ry, rz, lod? };

    for (const SMapObject& info : *m_pMapObjects) {
        Printf("\t\t{ %d, %d, %d, %f, %f, %f, %f, %f, %f, %s, %d };\n",
               info.modelId,
               info.modelDef,
               info.interior,
               info.x,
               info.y,
               info.z,
               info.rx,
               info.ry,
               info.rz,
               info.isLod ? "true" : "false",
               info.lod + 1
               );
    }
    Printf("\t};\n");
}

void CMapDataWriter::WritePhysicalInfo() {
    Printf("\tphysical = {\n");

    Printf("\t\tgroups = {\n");
    for (const auto &group : m_physicalInfo->groups) {
        Printf("\t\t\t{ %f, %f, %f, %f, %f, %f, %f, %d, %d, %s, %s, %d, %f, %f, %f, %d, %f, %f, %f, %f, %f, %d, %s };\n",
               group.mass,
               group.turnMass,
               group.airResistance,
               group.elasticity,
               group.percentSubmerge,
               group.uprootLimit,
               group.colDamMultiplier,
               group.colDamageEffect,
               group.specialColResponseCases,
               group.avoidCamera ? "true" : "false",
               group.causesExplosion ? "true" : "false",
               group.fxType,
               group.fxOffsetX,
               group.fxOffsetY,
               group.fxOffsetZ,
               group.fxEffect,
               group.hasBreakableInfo ? group.smashMultiplier : 0.0f,
               group.hasBreakableInfo ? group.breakVelX : 0.0f,
               group.hasBreakableInfo ? group.breakVelY : 0.0f,
               group.hasBreakableInfo ? group.breakVelZ : 0.0f,
               group.hasBreakableInfo ? group.breakVelRandom : 0.0f,
               group.hasBreakableInfo ? group.gunBreakMode : 1,
               (group.hasBreakableInfo && group.produceSparks) ? "true" : "false"
        );
    }
    Printf("\t\t};\n");

    Printf("\t\tmodels = {\n");
    for (const auto &defs : m_physicalInfo->defs) {
        Printf("\t\t\t{");

        for (const auto &def : defs) {
            Printf(" %d;", def);
        }

        Printf(" };\n");

    }
    Printf("\t\t};\n");

    Printf("\t};\n");
}

void CMapDataWriter::WriteModelsInfo()
{

    // Col map
    Printf("\tcolmap = {\n");
    std::unordered_map<std::string, size_t> colPos;

    for (size_t i = 1; const auto &map : *m_pColMap) {
        colPos[map.first] = i;
        Printf("\t\t{ %d, %d };\n", map.second.first, map.second.second);
        i++;
    }
    Printf("\t};\n");

    // Defs
    Printf("\tdefs = {\n");

    std::unordered_map<std::string, size_t> fileMap;

    size_t imgFilesCount = m_pIMG->GetFilesCount();
    for (size_t index = 0; index < imgFilesCount; index++) {
        fileMap[m_pIMG->GetFileInfo(index)->szFileName.GetLowerString()] = index;
    }

    Printf("\t\tatomic = {\n");

    // { id, dffPos, txdPos, colPos, drawDistance, flags }
    for (const auto &def : *m_atomic) {
        const std::string modelName = def.modelName.GetLowerString();
        const std::string txdName = def.texDictName.GetLowerString();
        const bool hasCol = colPos.contains(modelName);
        const std::string colStr = hasCol ? std::to_string(colPos[modelName]) : "nil";

        Printf("\t\t\t{ %d, %d, %d, %s, %d, %d };\n",
               def.modelId,
               fileMap[modelName + ".dff"],
               fileMap[txdName + ".txd"],
               colStr.c_str(),
               (uint32_t)def.drawDist,
               (uint32_t)def.flags
               );
    }

    Printf("\t\t};\n");

    Printf("\t\ttimed = {\n");

    // { id, dffPos, txdPos, colPos, drawDistance, flags, timeOn, timeOff }
    for (const auto &def : *m_timed) {
        const std::string modelName = def.modelName.GetLowerString();
        const std::string txdName = def.texDictName.GetLowerString();
        const bool hasCol = colPos.contains(modelName);
        const std::string colStr = hasCol ? std::to_string(colPos[modelName]) : "nil";

        Printf("\t\t\t{ %d, %d, %d, %s, %d, %d, %d, %d };\n",
               def.modelId,
               fileMap[modelName + ".dff"],
               fileMap[txdName + ".txd"],
               colStr.c_str(),
               (uint32_t)def.drawDist,
               (uint32_t)def.flags,
               def.on,
               def.off
               );
    }

    Printf("\t\t};\n");

    Printf("\t\tclump = {\n");

    // { id, dffPos, txdPos, colPos, drawDistance, flags }
    for (const auto &def : *m_clump) {
        const std::string modelName = def.modelName.GetLowerString();
        const std::string txdName = def.texDictName.GetLowerString();
        const bool hasCol = colPos.contains(modelName);
        const std::string colStr = hasCol ? std::to_string(colPos[modelName]) : "nil";

        Printf("\t\t\t{ %d, %d, %d, %s, %d, %d };\n",
               def.modelId,
               fileMap[modelName + ".dff"],
               fileMap[txdName + ".txd"],
               colStr.c_str(),
               (uint32_t)def.drawDist,
               (uint32_t)def.flags
               );
    }

    Printf("\t\t};\n");
    Printf("\t};\n");
}
