#include "CLoader.h"

bool CLoader::Open()
{
    if (m_stream.is_open())
        return false;

    if (m_path.empty())
        return false;

    if (!std::filesystem::exists(m_path))
        return false;

    m_stream = std::ifstream(m_path);

    // Open the file
    if (m_stream.fail())
    {
        m_stream.close();
        return false;
    }

    //m_stream.open(m_path);

    if (!m_stream.is_open()) {
        return false;
    }

    return true;
}

void CLoader::Close()
{
    m_stream.close();
}

void CLoader::ReadLineWithoutCommas(std::string &buff)
{
    buff.clear();
    std::getline(m_stream, buff);
    for (auto &c : buff) {
        c = c == ',' ? ' ' : c;
    }
}
