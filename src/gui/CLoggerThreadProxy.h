#pragma once

#include "../logic/CAbstractLogger.h"
#include <QObject>
#include <string>

class CLoggerThreadProxy: public QObject, public CAbstractLogger {
Q_OBJECT
public:
    CLoggerThreadProxy(CAbstractLogger *logger): m_logger(logger) {
        connect(this, SIGNAL(LogSig(ELogLevel,std::string)), SLOT(OnLog(ELogLevel,std::string)));
    };

    void Log(ELogLevel level, const char* text) override { emit LogSig(level, std::string(text)); };

private:
    CAbstractLogger *m_logger;

private slots:
    void OnLog(ELogLevel level, std::string info) { m_logger->Log(level, info.c_str()); };

signals:
    void LogSig(ELogLevel level, std::string info);
};
