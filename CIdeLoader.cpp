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
    std::string buff(101, '\000');
    while(m_stream.getline(buff.data(), 100)) {
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
                        auto &def = atomic.emplace_back();
                        int modelId;
                        int drawDist;
                        int flags;
                        scanf("%d %s %s %d %d", &modelId, &def.modelName, &def.texDictName, &drawDist, &flags);
                        def.modelId = static_cast<uint16_t>(modelId);
                        def.drawDist = static_cast<uint16_t>(drawDist);
                        def.flags = static_cast<uint16_t>(flags);
                        break;
                    }
                    case IDE_READ_MODE::TOBJ: {
                        auto &def = timed.emplace_back();
                        int modelId;
                        int drawDist;
                        int flags;
                        int on;
                        int off;
                        scanf("%d %s %s %d %d %d %d", &modelId, &def.modelName, &def.texDictName, &drawDist, &flags, &on, &off);
                        def.modelId = static_cast<uint16_t>(modelId);
                        def.drawDist = static_cast<uint16_t>(drawDist);
                        def.flags = static_cast<uint16_t>(flags);
                        def.on = static_cast<uint8_t>(on);
                        def.off = static_cast<uint8_t>(off);
                        break;
                    }
                    case IDE_READ_MODE::ANIM: {
                        auto &def = clump.emplace_back();
                        int modelId;
                        int drawDist;
                        int flags;
                        scanf("%d %s %s %d %d %s", &modelId, &def.modelName, &def.texDictName, &drawDist, &flags, &def.animName);
                        def.modelId = static_cast<uint16_t>(modelId);
                        def.drawDist = static_cast<uint16_t>(drawDist);
                        def.flags = static_cast<uint16_t>(flags);
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
                } else if (buff.starts_with("tobj")) {
                    mode = IDE_READ_MODE::ANIM;
                }
            }
        }
    }
}
