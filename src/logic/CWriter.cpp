#include "CWriter.h"

bool CWriter::Create()
{
    m_stream.open(m_path);

    if (!m_stream.is_open()) {
        return false;
    }

    m_stream.clear();

    return true;
}

void CWriter::Close()
{
    m_stream.flush();
    m_stream.close();
}
