
#pragma once

#include <algorithm>
#include <string>

template<int size>
class CArrayStr {
public:
    CArrayStr(): m_data{} {};
    ~CArrayStr() = default;

    size_t GetSize() const noexcept {
        auto end = std::find(m_data, &m_data[size], '\000');
        return end - m_data;
    };

    std::string GetLowerString() const noexcept {
        char buff[size];
        size_t i;
        for (i = 0; i < size; i++) {
            buff[i] = std::tolower(m_data[i]);
            if (buff[i] == '\000') {
                break;
            }
        }

        return std::string(buff, i);
    };

    std::string GetString() const noexcept {
        return std::string(data, std::find(m_data, &m_data[size], '\000'));
    };

    char* data() {
        return m_data;
    }

private:
    char m_data[size];
};
