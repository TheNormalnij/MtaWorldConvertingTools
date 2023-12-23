#include "CColLib.h"

bool CColLib::Unpack(CColLib &out, std::size_t offset)
{
    if (m_data.size() < offset) {
        return false;
    }

    SColHeader &header = reinterpret_cast<SColHeader&>(m_data.at(offset));

    const std::size_t end = header.size + offset + sizeof(SColHeader::size) + sizeof(SColHeader::version);
    if (end > m_data.size()) {
        return false;
    }

    std::vector<char> buff(m_data.begin() + offset, m_data.begin() + end);

    out = CColLib(std::move(buff));

    return true;
}
