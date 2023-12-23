#include "CIMG.h"

#include <cstring>

struct SImgHeader
{
    char         szMagic[4];
    unsigned int uiFilesCount;
};

bool CIMG::Create()
{
    return false;
}

bool CIMG::Open()
{

    if (!m_filesInfo.empty())
        return false;

    if (m_stream.is_open())
        return false;

    if (m_path.empty())
        return false;

    if (!fs::exists(m_path))
        return false;

    m_stream = std::ifstream(m_path, std::ios::binary);

    // Open the file
    if (m_stream.fail())
    {
        m_stream.close();
        return false;
    }

    SImgHeader fileHeader;

    // Read header
    m_stream.read(reinterpret_cast<char*>(&fileHeader), sizeof(SImgHeader));

    if (m_stream.fail() || m_stream.eof() || std::memcmp(&fileHeader.szMagic, "VER2", 4) != 0)
    {
        m_stream.close();
        return false;
    }

    // Read content info
    try
    {
        m_filesInfo.resize(fileHeader.uiFilesCount);
    }
    catch (const std::bad_alloc&)
    {
        m_stream.close();
        return false;
    }

    m_stream.read(reinterpret_cast<char*>(m_filesInfo.data()), sizeof(SImgFileInfo) * (std::streampos)fileHeader.uiFilesCount);
    if (m_stream.fail() || m_stream.eof())
    {
        m_stream.close();
        return false;
    }

    PrepareFileMap();
    return true;
}

bool CIMG::UnpackFile(const SImgFileInfo &info, std::vector<char> &buff)
{
    const size_t size = info.usSize * 2048;

    buff.resize(size);

    m_stream.seekg(info.uiOffset * 2048);

    m_stream.read(buff.data(), size);

    return true;
}

bool CIMG::UnpackFile(size_t pos, std::vector<char> &buff)
{
    if (m_filesInfo.size() < pos) {
        return false;
    }

    return UnpackFile(m_filesInfo[pos], buff);
}

bool CIMG::AddFile(std::string_view fileName, uint8_t *content, size_t count)
{
    if (fileName.size() > 24) {
        return false;
    }

    return false;
}

size_t CIMG::GetSize() const noexcept
{
    return 0;
}

void CIMG::PrepareHeader(size_t elementsCount)
{
    m_filesInfo.reserve(elementsCount);
    //m_startOffset =
}

void CIMG::PrepareFileMap()
{
    for (auto &item : m_filesInfo) {
        m_fileMap[item.szFileName] = &item;
    }
}
