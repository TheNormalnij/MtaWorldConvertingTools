#include "CDFF.h"

uint32_t CDFF::GetVersion() const {
    auto* header = reinterpret_cast<const SDFFHeader*>(m_data.data());
    return header->version;
}

uint32_t CDFF::GetSize() const {
    auto* header = reinterpret_cast<const SDFFHeader*>(m_data.data());
    return header->size;
}

uint32_t CDFF::GetType() const {
    auto* header = reinterpret_cast<const SDFFHeader*>(m_data.data());
    return header->type;
}
