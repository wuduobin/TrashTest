#ifndef LOG_H
#define LOG_H

#include <QFile>
#include <QFileInfo>
#include <QThread>
#include <QList>
#include <QMap>
#include <QDateTime>
#include <QMutex>
#include <QObject>
#include <QTimer>

enum LOGLEVEL { TRACE = 0, DEBUG, INFO, WARN, MYERROR, NUL };
static const int MAXLEN = 1 << 16;

class Log :public QObject
{
    Q_OBJECT
    static QString logQStrLevel(LOGLEVEL level);
    void writeFile();
    void writeLogFile(QString fileName, QList<QString>& logMsg);
    void logDir();
    void logCfg();
    void setPrefab(QString prefab);
    void setLogPath(QString logPath);
    void setWriteLevel(int level);

public:
    Log();
    explicit Log(QString sFileName);
    ~Log();

    static Log& getInstance();
    static void WriteLogLevel(LOGLEVEL loglevel, const QString& msg);
    static void WriteLog(const char* msg);
    static void SetPath(const char* path);
    static void logPrefab(const char* prefab);
    static void logWriteLevel(LOGLEVEL level);

    void writeLogLevel(LOGLEVEL level, const char* msg);
    void writeLog(const char* msg);
    void log(LOGLEVEL logLevel, const char* fileName, int lineNo, const QString &msg);

    static QString getLogPath();

    static bool init(const QString& prefab);

    static void debug(const QString& rMessage);
    static void debug(const char* pMessage);

    static void error(const QString& rMessage);
    static void error(const char* pMessage);

    static void info(const QString& rMessage);
    static void info(const char* pMessage);

    static void trace(const QString& rMessage);
    static void trace(const char* pMessage);

    static void warn(const QString& rMessage);
    static void warn(const char* pMessage);

signals:
    void sigWriteLog();

private slots:
    void on_WriteLog();

private:
    QThread m_thread;

    int m_iWriteLevel;
    QString m_sLogPath;
    QString m_sFileName;
    QString m_sPrefix;

    QMutex m_aMutex;
    QMap<QString,QList<QString> > m_qmLogMsg;
};

extern void logDebug(const QString &msg);
extern void logDebug(const char* msg,...);
extern void logInfo(const QString &msg);
extern void logInfo(const char* msg,...);
extern void logWarn(const QString &msg);
extern void logWarn(const char* msg,...);
extern void logError(const QString &msg);
extern void logError(const char* msg,...);
//extern void outputMessage(QtMsgType type, const QMessageLogContext &context, const QString &msg);

#endif // LOG_H
