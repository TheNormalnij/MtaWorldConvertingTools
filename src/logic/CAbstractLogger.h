#pragma once

enum class ELogLevel {
    VERBOSE,
    INFO,
    WARNING,
    ERROR,
};

// From some point this is not a real interface class
class CAbstractLogger
{
public:
    void Verbose(const char* msg) { Log(ELogLevel::VERBOSE, msg); };
    void Info(const char* msg) { Log(ELogLevel::INFO, msg); };
    void Warning(const char* msg) { Log(ELogLevel::WARNING, msg); };
    void Error(const char* msg) { Log(ELogLevel::ERROR, msg); };

    template<typename... Args>
    void Verbose(const char* msg, Args ...args) { Log(ELogLevel::VERBOSE, msg, args...); };
    template<typename... Args>
    void Info(const char* msg, Args ...args) { Log(ELogLevel::INFO, msg, args...); };
    template<typename... Args>
    void Warning(const char* msg, Args ...args) { Log(ELogLevel::WARNING, msg, args...); };
    template<typename... Args>
    void Error(const char* msg, Args ...args) { Log(ELogLevel::ERROR, msg, args...); };

    template<typename... Args>
    void Log(ELogLevel level, const char* format, Args ...args) {
        char msg[256];
        sprintf(msg, format, args...);
        Log(level, msg);
    };

    virtual void Log(ELogLevel level, const char*) = 0;
};
