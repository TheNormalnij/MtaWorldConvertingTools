#include <cstdint>

struct SMapObject {
    uint32_t modelId;
    uint32_t modelDef;
    uint8_t interior;
    double x;
    double y;
    double z;
    float rx;
    float ry;
    float rz;
    bool isLod;
    uint32_t lod;
};
