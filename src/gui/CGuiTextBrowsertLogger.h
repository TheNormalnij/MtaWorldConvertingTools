
#include "../logic/ILogger.h"
#include <QTextBrowser>

enum class ELogLevel {
    VERBOSE,
    INFO,
    WARNING,
    ERROR,
};

class CGuiTextBrowsertLogger : public ILogger {
public:
    CGuiTextBrowsertLogger(QTextBrowser *out): m_out(out) {};

    // ILogger interface
public:
    void Verbose(const char *) override;
    void Info(const char *) override;
    void Warning(const char *) override;
    void Error(const char *) override;
private:
    void AddLine(ELogLevel level, const char* text);

private:
    QString       m_all;
    QTextBrowser *m_out;
};
