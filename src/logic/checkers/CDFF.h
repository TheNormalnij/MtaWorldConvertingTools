#pragma once

#include <vector>
#include <cstdint>

struct SDFFHeader {
    uint32_t type;
    uint32_t size;
    uint32_t version;
};

class CDFF
{
public:
    CDFF(std::vector<char> &&data): m_data{data} {};

    uint32_t GetVersion() const;
    uint32_t GetSize() const;
    uint32_t GetType() const;

    size_t GetDataSize() const noexcept { return m_data.size(); };
private:
    const std::vector<char> m_data;
};
