#pragma once

#include <filesystem>
#include <fstream>

class CWriter
{
public:
    CWriter(std::filesystem::path path): m_path(std::move(path)) {};

    bool Create();
    void Close();

protected:
    template<typename... Args>
    void Printf(const char* fmt, Args ...params) {
        const int size = snprintf(m_buffer, 1024, fmt, params...);
        m_stream.write(m_buffer, size);
    }

    void Printf(const char* fmt);

protected:
    std::ofstream m_stream;
private:
    std::filesystem::path m_path;
    char m_buffer[1024];
};
