
#include "CWaterLoader.h"

enum class WATER_READ_MODE {
    NONE,
    PROCESSED,
};


void CWaterLoader::Read(std::vector<SWaterInfo> &out) {
    WATER_READ_MODE mode = WATER_READ_MODE::NONE;
    std::string buff;
    while(!m_stream.eof()) {
        ReadLineWithoutCommas(buff);
        if (buff[0] == '#') {
            continue;
        }
        if (buff.starts_with("end")) {
            mode = WATER_READ_MODE::NONE;
            continue;
        } else {
            if (mode != WATER_READ_MODE::NONE) {
                switch (mode) {
                case WATER_READ_MODE::PROCESSED: {
                    SWaterInfo def{};
                    int32_t flags;
                    const auto scanned = sscanf(buff.c_str(),
                                                "%f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %d",
                                                &def.vertices[0].x,
                                                &def.vertices[0].y,
                                                &def.vertices[0].z,
                                                &def.vertices[0].w1,
                                                &def.vertices[0].w2,
                                                &def.vertices[0].w3,
                                                &def.vertices[0].w4,
                                                &def.vertices[1].x,
                                                &def.vertices[1].y,
                                                &def.vertices[1].z,
                                                &def.vertices[1].w1,
                                                &def.vertices[1].w2,
                                                &def.vertices[1].w3,
                                                &def.vertices[1].w4,
                                                &def.vertices[2].x,
                                                &def.vertices[2].y,
                                                &def.vertices[2].z,
                                                &def.vertices[2].w1,
                                                &def.vertices[2].w2,
                                                &def.vertices[2].w3,
                                                &def.vertices[2].w4,
                                                &def.vertices[3].x,
                                                &def.vertices[3].y,
                                                &def.vertices[3].z,
                                                &def.vertices[3].w1,
                                                &def.vertices[3].w2,
                                                &def.vertices[3].w3,
                                                &def.vertices[3].w4,
                                                &flags);
                    if (scanned == 29) {
                        def.type = EWaterPointType::RECTANGLE;
                    } else if (scanned == 22) {
                        def.type = EWaterPointType::TRIAGLE;
                        flags = (uint8_t)def.vertices[3].x;
                    } else if (scanned != 11) {
                        continue;
                    }

                    def.flags = (uint8_t)flags;

                    out.emplace_back(std::move(def));
                    break;
                }
                default:
                    continue;
                }

            } else {
                if (buff.starts_with("processed")) {
                    mode = WATER_READ_MODE::PROCESSED;
                }
            }
        }
    }
}
