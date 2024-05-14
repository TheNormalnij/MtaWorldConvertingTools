#include "CIplLoader.h"

enum class IPL_READ_MODE {
    NONE,
    INST,
};

void CIplLoader::Read(std::vector<SIplInfo> &out)
{
    const size_t lodOffset = out.size();
    IPL_READ_MODE mode = IPL_READ_MODE::NONE;
    std::string buff;
    while(!m_stream.eof()) {
        ReadLineWithoutCommas(buff);
        if (buff[0] == '#') {
            continue;
        }
        if (buff.starts_with("end")) {
            mode = IPL_READ_MODE::NONE;
            continue;
        } else {
            if (mode != IPL_READ_MODE::NONE) {
                switch (mode) {
                case IPL_READ_MODE::INST: {
                    SIplInfo def{};
                    const auto scanned = sscanf(buff.c_str(),
                                                "%d %s %d %f %f %f %f %f %f %f %d",
                                                &def.modelId,
                                                def.modelName.data(),
                                                &def.interior,
                                                &def.x, &def.y, &def.z,
                                                &def.rx, &def.ry, &def.rz, &def.rw,
                                                &def.lod);
                    if (scanned != 11) {
                        continue;
                    }

                    def.rw = -def.rw;

                    if (def.lod != -1) {
                        def.lod += lodOffset;
                    }

                    out.emplace_back(std::move(def));
                    break;
                }
                default:
                    continue;
                }

            } else {
                if (buff.starts_with("inst")) {
                    mode = IPL_READ_MODE::INST;
                }
            }
        }
    }
}
