
#include "CGuiTextBrowsertLogger.h"

void CGuiTextBrowsertLogger::Verbose(const char *text)
{
    AddLine(ELogLevel::VERBOSE, text);
}

void CGuiTextBrowsertLogger::Info(const char* text) {
    AddLine(ELogLevel::INFO, text);
}

void CGuiTextBrowsertLogger::Warning(const char* text) {
    AddLine(ELogLevel::WARNING, text);
}

void CGuiTextBrowsertLogger::Error(const char* text) {
    AddLine(ELogLevel::ERROR, text);
}

void CGuiTextBrowsertLogger::AddLine(ELogLevel level, const char* text)
{
    switch (level) {
        case ELogLevel::VERBOSE:
            m_all.append("[V] ");
            break;
        case ELogLevel::INFO:
            m_all.append("[I] ");
            break;
        case ELogLevel::WARNING:
            m_all.append("[W] ");
            break;
        case ELogLevel::ERROR:
            m_all.append("[E] ");
            break;
        default:
            return;
    }

    m_all.append(text);
    m_all.append('\n');

    m_out->setText(m_all);
}
