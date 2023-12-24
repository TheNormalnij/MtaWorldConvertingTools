
#pragma once

#include <vector>
#include <cstdint>

struct SColHeader {
    char version[4]; // COL3...
    uint32_t size;
    char name[20];
};

typedef std::vector<char> col_buff_t;

class CColLib
{
public:
    CColLib() {};
    CColLib(col_buff_t &&buff): m_data(std::move(buff)){};

    void Add(CColLib &another) {m_data.insert(m_data.end(), another.m_data.begin(), another.m_data.end()); };

    std::size_t GetSize() const noexcept { return m_data.size(); };
    const auto &GetName() { return (reinterpret_cast<SColHeader&>(m_data.at(0))).name; };

    // Can do it better
    bool Unpack(CColLib &out, std::size_t offset);

    const col_buff_t &GetData() const noexcept { return m_data; };
private:
    col_buff_t m_data;
};
