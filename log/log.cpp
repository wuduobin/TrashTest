#include "log.h"

#include <QDir>
#include <QEventLoop>
#include <QMutexLocker>
#include <QSettings>
#include <QApplication>

Log::Log() : QObject(NULL)
{
    m_iWriteLevel = 0;
    m_sLogPath = QApplication::applicationDirPath() + "/Log/";
    logCfg();
    logDir();

    connect(this, SIGNAL(sigWriteLog()), SLOT(on_WriteLog()), Qt::QueuedConnection);

    m_thread.start();
    this->moveToThread(&m_thread);
}

Log::Log(QString sFileName)
{
    m_sFileName = sFileName;

    m_iWriteLevel = 0;
    m_sLogPath = QDir::currentPath() + "/Log/";
    logCfg();
    logDir();

    m_thread.start();
    this->moveToThread(&m_thread);
}

Log::~Log()
{
    m_thread.quit();
    m_thread.wait(2000);

    writeFile();
}

QString Log::logQStrLevel(LOGLEVEL level)
{
    QString str = " ";
    switch (level)
    {
    case TRACE:
        str = "TRACE";
        break;
    case DEBUG:
        str = "DEBUG";
        break;
    case INFO:
        str = "INFO";
        break;
    case WARN:
        str = "WARN";
        break;
    case MYERROR:
        str = "ERROR";
        break;
    case NUL:
        str = " ";
        break;
    }

    return str;
}

void Log::writeFile()
{
    QMap<QString, QList<QString> > log;
    {
        QMutexLocker locker(&m_aMutex);
        log.swap(m_qmLogMsg);
    }

	for (QMap<QString, QList<QString> >::iterator it = log.begin(); it != log.end(); ++it)
    {
        QString fileName = it.key();
        writeLogFile(fileName, it.value());
    }
}

void Log::writeLogFile(QString fileName, QList<QString>& logMsg)
{
    QFile log(fileName);
    if (false == log.open(QIODevice::Append)) return ;

    while (false == logMsg.empty())
    {
        log.write(logMsg.front().toLocal8Bit().data(), logMsg.front().toLocal8Bit().length());
        logMsg.pop_front();
    }
    log.close();
}

void Log::logDir()
{
    QDir dir(m_sLogPath);
    if (false == dir.exists())
    {
        QDir dircur;
        dircur.mkpath(m_sLogPath);
    }
}

void Log::logCfg()
{
    QString cfg = "./log.ini";
    if (false == QFile::exists(cfg)) return ;

    QSettings setCfg(cfg, QSettings::IniFormat);
    int level = setCfg.value("Log/Level").toInt();
    Log::setWriteLevel(level);
}

void Log::setPrefab(QString prefab)
{
    m_sPrefix = prefab;
}

void Log::setLogPath(QString logPath)
{
    m_sLogPath = logPath;
    if (false == m_sLogPath.endsWith("/") || m_sLogPath.endsWith("\\"))
        m_sLogPath += "/";

    logDir();
}

void Log::setWriteLevel(int level)
{
    m_iWriteLevel = level;
}

Log& Log::getInstance()
{
    static Log ins;
    return ins;
}

void Log::WriteLogLevel(LOGLEVEL loglevel, const QString& msg)
{
    switch (loglevel)
    {
    case TRACE:
        Log::getInstance().log(TRACE, __FILE__, __LINE__, msg);
        break;
    case DEBUG:
        Log::getInstance().log(DEBUG, __FILE__, __LINE__, msg);
        break;
    case INFO:
        Log::getInstance().log(INFO, __FILE__, __LINE__, msg);
        break;
    case WARN:
        Log::getInstance().log(WARN, __FILE__, __LINE__, msg);
        break;
    case MYERROR:
        Log::getInstance().log(MYERROR, __FILE__, __LINE__, msg);
        break;
    case NUL:
        break;
    default:
        break;
    }
}

void Log::WriteLog(const char* msg)
{
    Log::getInstance().log(NUL, __FILE__, __LINE__, msg);
}

void Log::SetPath(const char* path)
{
    Log::getInstance().setLogPath(path);
}

void Log::logPrefab(const char* prefix)
{
    Log::getInstance().setPrefab(prefix);
}

void Log::logWriteLevel(LOGLEVEL level)
{
    Log::getInstance().setWriteLevel(level);
}

void Log::writeLogLevel(LOGLEVEL loglevel, const char* msg)
{
    switch (loglevel)
    {
    case TRACE:
        log(TRACE, __FILE__, __LINE__, msg);
        break;
    case DEBUG:
        log(DEBUG, __FILE__, __LINE__, msg);
        break;
    case INFO:
        log(INFO, __FILE__, __LINE__, msg);
        break;
    case WARN:
        log(WARN, __FILE__, __LINE__, msg);
        break;
    case MYERROR:
        log(MYERROR, __FILE__, __LINE__, msg);
        break;
    case NUL:
        break;
    default:
        break;
    }
}

void Log::writeLog(const char* msg)
{
    log(NUL, __FILE__, __LINE__, msg);
}

void Log::log(LOGLEVEL logLevel, const char* fileName, int lineNo, const QString &msg)
{
    Q_UNUSED(fileName) Q_UNUSED(lineNo)
    if(logLevel < m_iWriteLevel) return;

    QString sFileName = m_sLogPath;
    QDateTime now = QDateTime::currentDateTime();
    if(m_sPrefix.length() > 0)
        sFileName = sFileName + m_sPrefix + "_" + now.toString("yyyyMMdd") + ".txt";
    else
        sFileName = sFileName + "Log_" + now.toString("yyyyMMdd") + ".txt";

    QString logMsg = now.toString("hh:mm:ss.zzz");
    logMsg += " " + msg;
    logMsg += QString(" [%1]").arg(logQStrLevel(logLevel));
    QString threadName = QThread::currentThread()->objectName();
    if(threadName.length() <= 0)
    {
        threadName = QString("0x%1").arg((quintptr)QThread::currentThreadId(), 8, 16, QLatin1Char('0'));
    }
    logMsg += QString("[thread=%1] ").arg(threadName);
    logMsg += "\r\n";

    QMutexLocker locker(&m_aMutex);
    m_qmLogMsg[sFileName].push_back(logMsg);
    emit sigWriteLog();
}

void Log::on_WriteLog()
{
    writeFile();
}

QString Log::getLogPath()
{
    return getInstance().m_sLogPath;
}

bool Log::init(const QString& prefab)
{
    Log::getInstance().logPrefab(prefab.toLocal8Bit().data());
    //qInstallMessageHandler(outputMessage);

    return true;
}

void Log::debug(const QString& rMessage)
{
    logDebug(rMessage);
}

void Log::debug(const char* pMessage)
{
    logDebug(QString::fromLocal8Bit(pMessage));
}

void Log::error(const QString& rMessage)
{
    logError(rMessage);
}

void Log::error(const char* pMessage)
{
    logError(QString::fromLocal8Bit(pMessage));
}

void Log::info(const QString& rMessage)
{
    logInfo(rMessage);
}

void Log::info(const char* pMessage)
{
    logInfo(QString::fromLocal8Bit(pMessage));
}

void Log::trace(const QString& rMessage)
{
    logDebug(rMessage);
}

void Log::trace(const char* pMessage)
{
    logDebug(QString::fromLocal8Bit(pMessage));
}

void Log::warn(const QString& rMessage)
{
    logWarn(rMessage);
}

void Log::warn(const char* pMessage)
{
    logWarn(QString::fromLocal8Bit(pMessage));
}

void logDebug(const QString& msg)
{
    Log::WriteLogLevel(DEBUG, msg);
}

void logDebug(const char* msg, ...)
{
    char sLog[MAXLEN] = {0};
    va_list ap;
    va_start(ap, msg);
    vsnprintf(sLog, MAXLEN - 1, msg, ap);
    QString sLogMsg = QString::fromLocal8Bit(sLog);
    va_end(ap);

    logDebug(sLogMsg);
}

void logInfo(const QString& msg)
{
    Log::WriteLogLevel(INFO, msg);
}

void logInfo(const char* msg, ...)
{
    char sLog[MAXLEN] = {0};
    va_list ap;
    va_start(ap,msg);
    vsnprintf(sLog, MAXLEN - 1, msg,ap);
    QString sLogMsg = QString::fromLocal8Bit(sLog);
    va_end(ap);

    logInfo(sLogMsg);
}

void logWarn(const QString& msg)
{
    Log::WriteLogLevel(WARN,msg);
}

void logWarn(const char* msg, ...)
{
    char sLog[MAXLEN] = {0};
    va_list ap;
    va_start(ap, msg);
    vsnprintf(sLog, MAXLEN - 1, msg, ap);
    QString sLogMsg = QString::fromLocal8Bit(sLog);
    va_end(ap);

    logWarn(sLogMsg);
}

void logError(const QString& msg)
{
    Log::WriteLogLevel(MYERROR, msg);
}

void logError(const char* msg, ...)
{
    char sLog[MAXLEN] = {0};
    va_list ap;
    va_start(ap, msg);
    vsnprintf(sLog, MAXLEN - 1, msg, ap);
    QString sLogMsg = QString::fromLocal8Bit(sLog);
    va_end(ap);

    logError(sLogMsg);
}

//void outputMessage(QtMsgType type, const QMessageLogContext& context, const QString& msg)
//{
//    Q_UNUSED(context)
//
//    switch (type)
//    {
//    case QtDebugMsg:
//        Log::WriteLogLevel(DEBUG, msg);
//        break;
//    case QtInfoMsg:
//        Log::WriteLogLevel(INFO, msg);
//        break;
//    case QtWarningMsg:
//        Log::WriteLogLevel(WARN, msg);
//        break;
//    case QtCriticalMsg:
//    case QtFatalMsg:
//        Log::WriteLogLevel(MYERROR, msg);
//        break;
//    default:
//        Log::WriteLogLevel(TRACE, msg);
//        break;
//    }
//}
