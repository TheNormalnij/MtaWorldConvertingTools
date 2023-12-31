#include "CMetaWriter.h"

void CMetaWriter::Write()
{
    m_stream << "<meta>\n";
    m_stream << "\t<info type=\"script\" name=\"WorldName\" author=\"YourName\" version=\"1.0\" />\n";

    m_stream << "\t<include resource='Streamer' />\n\n";
    m_stream << "\t<script src=\"register.lua\" type=\"shared\" />\n\n";

    for (auto &path : m_files) {
        m_stream << "\t<file src=\"" << path.c_str() << "\" />\n";
    }

    m_stream << "</meta>\n";
}

void CMetaWriter::AddFile(const char *name)
{
    m_files.emplace_back(name);
}
