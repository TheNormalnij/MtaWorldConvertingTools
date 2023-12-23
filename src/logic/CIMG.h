
#pragma once

#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <filesystem>

namespace fs = std::filesystem;

struct SImgFileInfo
{
    unsigned int   uiOffset;
    unsigned short usSize;
    unsigned short usUnpackedSize;
    char           szFileName[24];
};

class CIMG
{
public:
    CIMG(fs::path &&path): m_path(std::move(path)){};

    bool Create();
    bool Open();
    bool Close();

    bool UnpackFile(size_t pos, std::vector<char> &buff);
    bool UnpackFile(const SImgFileInfo &info, std::vector<char> &buff);

    bool AddFile(std::string_view fileName, uint8_t *content, size_t count);

    const char* GetContentFileName(size_t pos) const noexcept { return m_filesInfo[pos].szFileName; };
    size_t GetFilesCount() const noexcept { return m_filesCount; };

    size_t GetSize() const noexcept;

    void PrepareHeader(size_t elementsCount);

    auto &GetFilesInfo() const noexcept { return m_filesInfo; };
private:
    void PrepareFileMap();

private:
    std::ifstream m_stream;
    fs::path m_path;
    size_t m_filesCount;
    size_t m_startOffset;
    std::vector<SImgFileInfo> m_filesInfo;
    std::unordered_map<std::string, SImgFileInfo*> m_fileMap;
};

