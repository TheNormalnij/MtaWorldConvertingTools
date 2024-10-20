
#pragma once

#include "../logic/CAbstractLogger.h"
#include <QTextBrowser>

class CGuiTextBrowsertLogger : public CAbstractLogger {
public:
    CGuiTextBrowsertLogger(QTextBrowser *out): m_out(out) {};

    // ILogger interface
    void Log(ELogLevel level, const char* text) override;

private:
    QTextBrowser *m_out;
};
