#include "CConverter.h"

#include "CMetaWriter.h"

#ifndef _WIN32
    #include "CPathResolver.h"
#endif

namespace fs = std::filesystem;

CConverter::CConverter(ILogger *logger, SConverterParams &settings): m_log(logger), m_settings(settings) {
    m_usedImgs = 0;
}

void CConverter::Convert()
{
    m_log->Info("Start converting...");

    try {
        // This fixes float values reading in sscanf
        setlocale(LC_NUMERIC, "C");

        if (!LoadModGtaDat()) {
            m_log->Error("Stopped");
            return;
        }

        if (!LoadModModelDefs()) {
            m_log->Error("Stopped");
            return;
        }

        if (!LoadModIpls()) {
            m_log->Error("Stopped");
            return;
        }

        if (m_settings.genMeta) {
            WriteMeta();
        }
    } catch (...) {
        m_log->Error("Runtime error");
    }

    m_log->Info("Finished");
}

bool CConverter::LoadModGtaDat()
{
    m_log->Info("Load mod gta.dat");

    fs::path path = m_settings.modPath / "data/gta.dat";
    CGtaDatLoader datLoader(std::move(path));

    if (!datLoader.Open()) {
        m_log->Error("Can not read gta.dat from mod");
        return false;
    }

    datLoader.Read();
    datLoader.Close();

    m_modFiles.swap(datLoader.GetFiles());

    std::string msg = "Readed " + std::to_string(m_modFiles.size()) + " lines from mod gta.dat";
    m_log->Verbose(msg.c_str());

    return true;
}

bool CConverter::LoadModModelDefs()
{
    m_log->Info("Load mod IDE's");

    for (const auto &filePathData : m_modFiles) {
        if (filePathData.type != EDatType::IDE) {
            continue;
        }

        fs::path fullPath;
        MakePath(m_settings.modPath, filePathData.realtivePath, fullPath);

        CIdeLoader ideLoader(std::move(fullPath));

        if (!ideLoader.Open()) {
            std::string err = "Can not open mod IDE file: " + filePathData.realtivePath;
            m_log->Error(err.c_str());
            return false;
        }

        ideLoader.Read(m_atomic, m_timed, m_clump);

        ideLoader.Close();
    }

    return true;
}

bool CConverter::LoadModIpls()
{
    m_log->Info("Load mod IPL's");

    for (const auto &filePathData : m_modFiles) {
        if (filePathData.type != EDatType::IPL) {
            continue;
        }

        fs::path fullPath;
        MakePath(m_settings.modPath, filePathData.realtivePath, fullPath);

        CIplLoader iplLoader(std::move(fullPath));

        if (!iplLoader.Open()) {
            std::string err = "Can not open mod IPL file: " + filePathData.realtivePath;
            m_log->Error(err.c_str());
            return false;
        }

        iplLoader.Read(m_modIpl);

        iplLoader.Close();
    }

    return true;
}

void CConverter::WriteMeta()
{
    m_log->Info("Write meta.xml");

    CMetaWriter metaWriter( fs::path(m_settings.outputPath).append("meta.xml"));

    if (!metaWriter.Create()) {
        m_log->Error("Can not create meta.xml file");
        return;
    }

    metaWriter.AddFile("world1.img");
    metaWriter.AddFile("world.lua");

    metaWriter.Write();

    metaWriter.Close();
}

void CConverter::MakePath(const fs::path &root, const std::string &add, fs::path &out)
{
#ifndef _WIN32
    static CPathResolver resolver{};
    resolver.Resolve(root, add, out);
#else
    out = root/add;
#endif
}
