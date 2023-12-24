#pragma once

enum class ELogLevel {
    VERBOSE,
    INFO,
    WARNING,
    ERROR,
};

class ILogger
{
public:
    void Verbose(const char* msg) { Log(ELogLevel::VERBOSE, msg); };
    void Info(const char* msg) { Log(ELogLevel::INFO, msg); };
    void Warning(const char* msg) { Log(ELogLevel::WARNING, msg); };
    void Error(const char* msg) { Log(ELogLevel::ERROR, msg); };

    virtual void Log(ELogLevel level, const char*) = 0;
};
