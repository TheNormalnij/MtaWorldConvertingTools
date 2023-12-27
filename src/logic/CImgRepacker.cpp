#include "CImgRepacker.h"

CImgRepacker::CImgRepacker(std::filesystem::path path): m_outputPath(std::move(path)), m_currentOutput(nullptr) {

}

CImgRepacker::~CImgRepacker()
{
    Close();
}

bool CImgRepacker::Create()
{
    if (m_currentOutput) {
        return false;
    }

    m_currentOutput = new CIMG(std::move(m_outputPath / "world1.img"));

    return m_currentOutput->Create();
}

void CImgRepacker::Close()
{
    if (m_currentOutput) {
        m_currentOutput->Close();
        m_currentOutput = nullptr;
    }
}

bool CImgRepacker::AddFile(const char* name, const std::vector<char> &buff)
{
    return m_currentOutput->AddFile(name, buff.data(), buff.size());
}

bool CImgRepacker::ExportFile(const char* name)
{
    const SImgFileInfo *fileInfo = nullptr;
    CIMG *sourceImg = nullptr;

    for (CIMG* img : m_import) {
        fileInfo = img->GetFileInfo(name);
        sourceImg = img;
        if (fileInfo) {
            break;
        }
    }

    if (!fileInfo)
        return false;

    std::vector<char> buff;
    buff.resize(fileInfo->usSize * 2048);
    sourceImg->UnpackFile(fileInfo, buff);

    return m_currentOutput->AddFile(fileInfo->szFileName, buff.data(), buff.size());
}
