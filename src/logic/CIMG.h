
#pragma once

#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <filesystem>
#include "CArrayStr.h"

namespace fs = std::filesystem;

struct SImgFileInfo
{
    unsigned int   uiOffset;
    unsigned short usSize;
    unsigned short usUnpackedSize;
    CArrayStr<24>  szFileName;
};

class CIMG
{
public:
    CIMG(fs::path path): m_path(std::move(path)){};

    bool Create();
    bool Open();
    bool Close();

    bool UnpackFile(size_t fileIndex, std::vector<char> &buff);
    bool UnpackFile(size_t offsetBlock, size_t sizeBlock, std::vector<char> &buff);
    bool UnpackFile(const SImgFileInfo *info, std::vector<char> &buff);

    bool AddFile(std::string_view fileName, const char *content, size_t count);
    size_t GetFilesCount() const noexcept { return m_filesInfo.size(); };

    const SImgFileInfo *GetFileInfo(size_t index) const { return &m_filesInfo[index]; };
    const SImgFileInfo *GetFileInfo(const char* name) { return m_fileMap[name]; };
    const SImgFileInfo *GetFileInfo(std::string &name) { return m_fileMap[name]; };

    size_t GetSize();

    auto &GetFilesInfo() const noexcept { return m_filesInfo; };
private:
    void PrepareFileMap();
    bool HasHeaderSizeForNextElement();
    void ExtentHeader();

    void UpdateHeader();

private:
    std::fstream m_stream;
    fs::path m_path;
    bool m_readOnly;

    size_t m_startDataOffset;

    std::vector<SImgFileInfo> m_filesInfo;
    std::unordered_map<std::string, SImgFileInfo*> m_fileMap;
};

