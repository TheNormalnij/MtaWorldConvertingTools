
#include "CGuiTextBrowsertLogger.h"

void CGuiTextBrowsertLogger::Log(ELogLevel level, const char* text)
{
    QString msg;
    switch (level) {
        case ELogLevel::VERBOSE:
            msg.append("[V] ");
            break;
        case ELogLevel::INFO:
            msg.append("[I] ");
            break;
        case ELogLevel::WARNING:
            msg.append("[W] ");
            break;
        case ELogLevel::ERROR:
            msg.append("[E] ");
            break;
        default:
            return;
    }

    msg.append(text);
    m_out->append(msg);
}
