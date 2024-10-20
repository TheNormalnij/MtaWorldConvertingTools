#include "CDFFChecker.h"

#include <array>

const std::array<uint32_t, 4> GTASA_DFF_VERSIONS = {
    0x1803FFFF,
    0x0324FFFF,
    0x1003FFFF,
    0x0800FFFF,
};

bool CDFFChecker::Check()
{
    return CheckEmpty() && CheckType() && CheckVersion() && CheckSize();
}

bool CDFFChecker::CheckEmpty()
{
    if (m_dff->GetDataSize() > sizeof(SDFFHeader)) {
        return true;
    }

    error = "DFF is empty";
    return false;
}

bool CDFFChecker::CheckVersion()
{
    const auto dffVersion = m_dff->GetVersion();

    for (auto possibleVersion : GTASA_DFF_VERSIONS) {
        if (possibleVersion == dffVersion) {
            return true;
        }
    }

    error = "Unsupported DFF version";
    return false;
}

bool CDFFChecker::CheckSize()
{
    if (m_dff->GetDataSize() >= m_dff->GetSize()) {
        return true;
    }

    error = "Invalid DFF size";
    return false;
}

bool CDFFChecker::CheckType()
{
    const uint32_t type = m_dff->GetType();
    if (type == 0x10 || type == 0x2b) {
        return true;
    }

    error = "Invalid DFF type " + std::to_string(type);
    return false;
}
