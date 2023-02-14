#ifndef SETTING_H
#define SETTING_H

#include <QObject>
#include <QStringList>
#include <QMap>
#include <QVariant>
#include <QSettings>

class Setting
{
    void init();
    void LoadCfg();
    QString toString( const QVariant& value );
    void saveCfgValue(const QString& section, const QString& key, const QString& value,const QString comment = "");
    bool getInsertPos(const QString& section, const QString& key, qint64& pos, qint64& nextPos);
    bool getDeletePos(const QString& section, qint64& pos, qint64& nextPos);
    bool updateCfgFile(qint64 pos, qint64 nextPos, const QString section, const QString& line);

public:
    explicit Setting(QString file = "./Config/config.ini");
    ~Setting();

    QVariant value(const QString& key, const QVariant& defValue= QVariant());
    QString getCfgValue(const QString& section, const QString& key, const QString def = "");
    QStringList getSectionsKey(const QString& section) const;
    QStringList getSectionsValue(const QString& section) const;
    QString getCftComment(const QString& section, const QString& key, const QString def = "");
    bool setCfgValue(const QString& section, const QString& key, const QString& value, const QString comment = "");
	void delSection(const QString& section);
	void delSectionKey(const QString& section, const QString& key);
    int getSectionCfg(QString section, QMap<QString, QString>& cfgData);
    QStringList getSections() const;
	QString getCfgFileName() const;

protected:
    QString mCfgFile;
    QStringList mSections;
    QMap<QString, QVariant> mKeyValue;
    QMap<QString, QString> mSensitiveKey;
    QMap<QString, QString> mCommentKey;
};

#endif // SETTING_H
