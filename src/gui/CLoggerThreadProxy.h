#pragma once

#include "../logic/ILogger.h"
#include <QString>
#include <QObject>

class CLoggerThreadProxy: public QObject, public ILogger {
Q_OBJECT
public:
    CLoggerThreadProxy(ILogger *logger): m_logger(logger) {
        connect(this, SIGNAL(LogSig(ELogLevel,QString)), SLOT(OnLog(ELogLevel,QString)));
    };

    void Log(ELogLevel level, const char* text) override { emit LogSig(level, QString(text)); };

private:
    ILogger *m_logger;

private slots:
    void OnLog(ELogLevel level, QString info) { m_logger->Log(level, info.toStdString().c_str()); };

signals:
    void LogSig(ELogLevel level, QString info);
};
