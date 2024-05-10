#include "convertors.h"
#include "math.h"

constexpr double radToDeg = (180.0/3.141592653589793238463);

void quaternionToEulerAngles(float qx, float qy, float qz, float qw, float &ex, float &ey, float &ez) {
    ex = radToDeg * std::atan2(-2 * (qx*qz - qw*qy), qw *qw - qx*qx -qy*qy + qz*qz);
    ey = radToDeg * std::asin(std::clamp(2 * (qy * qz + qw * qx), -1.0f, 1.0f));
    ez = radToDeg * std::atan2( -2 * (qx * qy - qw * qz), (qw * qw - qx *qx + qy * qy - qz * qz));
}

void convertPosForMta(SIplInfo &from, SMapObject &to) {
    to.x = from.x;
    to.y = from.y;
    to.z = from.z;
    to.isLod = false;
    to.modelId = from.modelId;
    to.modelDef = 0;
    to.interior = from.interior;

    quaternionToEulerAngles(from.rx, from.ry, from.rz, from.rw, to.rx, to.ry, to.rz);
}
