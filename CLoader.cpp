#include "CLoader.h"

bool CLoader::Open()
{
    m_stream.open(m_path);

    if (!m_stream.is_open()) {
        return false;
    }

    return true;
}

void CLoader::Close()
{
    m_stream.close();
}
