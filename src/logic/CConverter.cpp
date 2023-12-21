#include "CConverter.h"

#include "CMetaWriter.h"
#include "CGtaDatLoader.h"

namespace fs = std::filesystem;

CConverter::CConverter(ILogger *logger, SConverterParams &settings): m_log(logger), m_settings(settings) {
    m_usedImgs = 0;
}

void CConverter::Convert()
{
    CGtaDatLoader datLoader(std::move(m_settings.modPath.append("data/gta.dat")));

    if (!datLoader.Open()) {
        m_log->Error("Can not read gta.dat from mod");
        return;
    }

    datLoader.Read();
    datLoader.Close();

    if (m_settings.genMeta) {
        WriteMeta();
    }
}

bool CConverter::LoadModModelDefs()
{
    m_log->Info("Load mod IDE's");

    for (const auto &idePath : m_modIdes) {
        CIdeLoader ideLoader(std::move(m_settings.modPath.append(idePath)), m_log);

        if (!ideLoader.Open()) {
            std::string err = "Can not open mod IDE file: " + idePath;
            m_log->Error(err.c_str());
            continue;
        }

        ideLoader.Read(m_atomic, m_timed, m_clump);

        ideLoader.Close();
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
