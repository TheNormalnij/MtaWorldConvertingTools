#include "CIdeLoader.h"

enum class IDE_READ_MODE {
    NONE,
    OBJ,
    TOBJ,
    ANIM,
};

void CIdeLoader::Read(std::vector<SAtomicModelDef> &atomic, std::vector<STimeModelDef> &timed, std::vector<SClumpModelDef> &clump)
{
    IDE_READ_MODE mode = IDE_READ_MODE::NONE;
    std::string buff;
    while(!m_stream.eof()) {
        ReadLineWithoutCommas(buff);
        if (buff[0] == '#') {
            continue;
        }
        if (buff.starts_with("end")) {
            mode = IDE_READ_MODE::NONE;
            continue;
        } else {
            if (mode != IDE_READ_MODE::NONE) {
                switch (mode) {
                    case IDE_READ_MODE::OBJ: {
                        SAtomicModelDef def{};
                        const auto scanned = sscanf(buff.c_str(), "%d %s %s %f %d", &def.modelId, def.modelName, def.texDictName, &def.drawDist, &def.flags);
                        if (scanned != 5) {
                            // TODO additional logic
                            continue;
                        }

                        atomic.emplace_back(std::move(def));
                        break;
                    }
                    case IDE_READ_MODE::TOBJ: {
                        STimeModelDef def{};
                        const auto scanned = sscanf(buff.c_str(), "%d %s %s %f %d %d %d", &def.modelId, def.modelName, def.texDictName, &def.drawDist, &def.flags, &def.on, &def.off);
                        if (scanned != 7) {
                            // TODO additional logic
                            continue;
                        }
                        timed.emplace_back(std::move(def));
                        break;
                    }
                    case IDE_READ_MODE::ANIM: {
                        SClumpModelDef def{};
                        const auto scanned = sscanf(buff.c_str(), "%d %s %s %s %f %d", &def.modelId, def.modelName, def.texDictName, def.animName, &def.drawDist, &def.flags);
                        if (scanned != 6) {
                            // TODO additional logic
                            continue;
                        }
                        clump.emplace_back(std::move(def));
                        break;
                    }
                    default:
                        continue;
                }

            } else {
                if (buff.starts_with("objs")) {
                    mode = IDE_READ_MODE::OBJ;
                } else if (buff.starts_with("tobj")) {
                    mode = IDE_READ_MODE::TOBJ;
                } else if (buff.starts_with("anim")) {
                    mode = IDE_READ_MODE::ANIM;
                }
            }
        }
    }
}
