#include "CIMG.h"

#include <algorithm>
#include <cstring>

constexpr size_t BLOCK_SIZE = 2048;

struct SImgHeader
{
    char         szMagic[4];
    unsigned int uiFilesCount;
};

bool CIMG::Create()
{
    if (!m_filesInfo.empty())
        return false;

    if (m_stream.is_open())
        return false;

    if (m_path.empty())
        return false;

    m_stream = std::fstream(m_path, std::ios::binary | std::ios::in | std::ios::out | std::ios::trunc);

    // Open the file
    if (m_stream.fail())
    {
        m_stream.close();
        return false;
    }

    m_readOnly = false;

    // Write empty header
    m_stream.write("VER2", 4);
    {
        std::vector<char> buff(BLOCK_SIZE - 4, '\000');
        m_stream.write(buff.data(), buff.size());
    }

    m_startDataOffset = BLOCK_SIZE;

    return true;
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

    m_readOnly = true;

    m_stream = std::fstream(m_path, std::ios_base::binary | std::ios_base::in);

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

bool CIMG::Close()
{
    if (!m_readOnly) {
        UpdateHeader();
        m_stream.flush();
    }
    m_stream.close();
    return true;
}

bool CIMG::UnpackFile(const SImgFileInfo *info, std::vector<char> &buff)
{
    const size_t size = info->usSize * BLOCK_SIZE;

    buff.resize(size);

    m_stream.seekg(info->uiOffset * BLOCK_SIZE);

    m_stream.read(buff.data(), size);

    return true;
}

bool CIMG::UnpackFile(size_t pos, std::vector<char> &buff)
{
    if (m_filesInfo.size() < pos) {
        return false;
    }

    return UnpackFile(&m_filesInfo[pos], buff);
}

bool CIMG::AddFile(std::string_view fileName, const char *content, size_t count)
{
    if (fileName.size() > 24) {
        return false;
    }

    if (!HasHeaderSizeForNextElement()) {
        ExtentHeader();
    }

    // Get offset with alingment
    size_t pos = GetSize();
    pos += pos % BLOCK_SIZE;

    m_stream.seekp(pos);

    m_stream.write(content, count);

    const size_t addZeroButes = count % BLOCK_SIZE;

    if (addZeroButes != 0) {
        std::vector<char> zeroBuff(addZeroButes, '\000');
        m_stream.write(zeroBuff.data(), addZeroButes);
        count += addZeroButes;
    }

    SImgFileInfo fileInfo{};
    fileInfo.uiOffset = pos / BLOCK_SIZE;
    fileInfo.usSize = count / BLOCK_SIZE;
    fileInfo.usUnpackedSize = fileInfo.usSize;

    std::memcpy(fileInfo.szFileName, fileName.data(), fileName.size());

    m_filesInfo.emplace_back(std::move(fileInfo));

    return true;
}

size_t CIMG::GetSize()
{
    m_stream.seekp (0, std::ios::end);
    return m_stream.tellp();
}

void CIMG::PrepareFileMap()
{
    for (auto &item : m_filesInfo) {
        m_fileMap[item.szFileName] = &item;
    }
}

bool CIMG::HasHeaderSizeForNextElement()
{
    const size_t estimatedSize = sizeof(SImgHeader) + sizeof(SImgFileInfo) * (m_filesInfo.size() + 1);
    return estimatedSize < m_startDataOffset;
}

void CIMG::ExtentHeader()
{
    // Find file with minimal size
    auto minFile = std::min_element(m_filesInfo.begin(), m_filesInfo.end(), [](const auto &a, const auto &b) { return a.uiOffset < b.uiOffset; });
    SImgFileInfo* file = minFile.base();

    // Unpack this file
    std::vector<char> buff;
    UnpackFile(file, buff);

    // Move file
    const size_t size = GetSize();
    m_stream.seekp(size);
    m_stream.write(buff.data(), buff.size());

    // Zero old file
    m_stream.seekp(file->uiOffset * BLOCK_SIZE);
    std::fill(buff.begin(), buff.end(), '\000');
    m_stream.write(buff.data(), buff.size());

    // Update file info
    file->uiOffset = size / BLOCK_SIZE;

    // Update start offset
    m_startDataOffset += minFile->usSize * BLOCK_SIZE;
}

void CIMG::UpdateHeader()
{
    m_stream.seekp(4);
    const uint32_t filesCount = m_filesInfo.size();
    m_stream.write(reinterpret_cast<const char*>(&filesCount), 4);

    m_stream.write(reinterpret_cast<const char*>(m_filesInfo.data()), sizeof(SImgFileInfo) * filesCount);
}
