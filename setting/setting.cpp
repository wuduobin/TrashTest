#include "setting.h"
#include <QFile>
#include <QDir>
#include <memory.h>
#include <QRegExp>
#include <memory>

Setting::Setting(QString file) :
    mCfgFile(file)
{
    init();
    LoadCfg();
}

Setting::~Setting()
{
}

void Setting::init()
{
    QDir dir;
    QString path = "./Config/";
    if (false == dir.exists(path)) dir.mkdir(path);
    QFile* file = new QFile;
    bool isExists = true;
    if (false == file->exists(mCfgFile))
    {
        isExists = false;
        file->setFileName(mCfgFile);
    }
    file->open(QIODevice::WriteOnly | QIODevice::Text);
    file->close();

    if (false == isExists)
    {
        //setCfgValue("BASE", "httpHost", "https://cto.angeek.com.cn/angeek");
        //setCfgValue("BASE", "machineCode", "");
        //setCfgValue("BASE", "MD5", "");
        //setCfgValue("BASE", "authUrl", "https://slo.angeek.com.cn");
        //setCfgValue("BASE", "knockDoorPort", "62201");
        setCfgValue("BASE", "gatewayAddress", "183.193.107.78:53201");
    }
}

void Setting::LoadCfg()
{
    QSettings setting(mCfgFile, QSettings::IniFormat);
    setting.setIniCodec("GBK");
    mSections = setting.childGroups();
    QStringList allKeys = setting.allKeys();
    for (int i = 0; i < allKeys.size(); ++i)
    {
        QString key = allKeys.at(i);
        QString keyData = toString(setting.value(key, ""));

        if (keyData.isEmpty()) continue;

        int pos = keyData.indexOf("//");
        if (pos >= 0)
        {
            mKeyValue[key.toLower()] = keyData.left(pos).trimmed();
            mCommentKey[key] = keyData.mid(pos + 2);
        }
        else
            mKeyValue[key.toLower()] = keyData.trimmed();

        mSensitiveKey[key.toLower()] = key;
    }
}

QString Setting::toString(const QVariant& value)
{
    QString string;
    if (value.type() == QVariant::StringList)
        string = value.toStringList().join(",");
    else
        string = value.toString();

    return string;
}

void Setting::saveCfgValue(const QString& section, const QString& key, const QString& value, const QString comment)
{
    if (section.length() <= 0 || key.length() <= 0)
        return;

    QString valueData = key + "=" + value;
    if (comment.length() > 0)
        valueData += "        //" + comment;
    valueData += "\r\n";

    QString sectionStr = "[" + section + "]\r\n";
    qint64 pos = -1;
    qint64 nextPos = -1;
    if (false == getInsertPos(section, key, pos, nextPos)) return;

    updateCfgFile(pos, nextPos, sectionStr, valueData);
}

/*!
 * \brief 查找更新配置的位置，找到对应key时填写非0位置，否则填写位置为0
 */
bool Setting::getInsertPos(const QString& section, const QString& key, qint64& pos, qint64& nextPos)
{
    QFile file(mCfgFile);
    if (false == file.exists()) return false;
    if (false == file.open(QIODevice::ReadOnly)) return false;

    QString sectionStr = "[" + section + "]";
    QString keyStr = key + "=";
    bool bFindKey = false;
    bool bFindSection = false;
    QByteArray line = file.readLine();
    while (line.length() > 0)
    {
        QString sLine = line;
        sLine = sLine.replace(' ', "");
        sLine = sLine.replace('\t', "");

        if (false == bFindSection && sLine.startsWith(sectionStr, Qt::CaseInsensitive))
        {
            bFindSection = true;
            pos = file.pos();
            nextPos = pos;
            line = file.readLine();
            bFindKey = true;
            continue;
        }

        if (bFindKey && sLine.startsWith("[", Qt::CaseInsensitive))
        {
            bFindKey = false;
            continue;
        }

        if (bFindKey && sLine.startsWith(keyStr, Qt::CaseInsensitive))
        {
            nextPos = file.pos();
            pos = file.pos() - line.length();
            break;
        }

        line = file.readLine();
    }
    file.close();

    if (false == bFindSection)
    {
        pos = -1;
        nextPos = -1;
    }

    return true;
}

/*!
 * \brief 获取删除的位置
 */
bool Setting::getDeletePos(const QString& section, qint64& pos, qint64& nextPos)
{
    QFile file(mCfgFile);
    if (false == file.exists()) return false;
    if (false == file.open(QIODevice::ReadOnly)) return false;

    QString sectionStr = "[" + section + "]";
    bool bFindSection = false;
    bool bFindNextSection = false;
    QByteArray line = file.readLine();

    while (line.length() > 0)
    {
        QString sLine = QString(line);

        if (false == bFindSection && sLine.startsWith(sectionStr, Qt::CaseInsensitive))
        {
            bFindSection = true;
            pos = file.pos() - line.length();
            nextPos = file.pos();
            bFindNextSection = true;
            line = file.readLine();
            continue;
        }

        if (bFindNextSection)
        {
            if (sLine.startsWith("[", Qt::CaseInsensitive))
            {
                nextPos = file.pos() - line.length();
                break;
            }
            else
                nextPos = file.pos();
        }
        line = file.readLine();
    }
    file.close();

    if (false == bFindSection)
    {
        pos = -1;
        nextPos = -1;
    }

    return true;
}

bool Setting::updateCfgFile(qint64 pos, qint64 nextPos, const QString section, const QString& line)
{
    QFile file(mCfgFile);
    if (false == file.exists()) return false;

    if (-1 == pos)
    {
        if (false == file.open(QIODevice::Append)) return false;

        file.write("\r\n", 2);
        file.write(section.toLocal8Bit().data(), section.toLocal8Bit().length());
        file.write(line.toLocal8Bit().data(), line.toLocal8Bit().length());
        file.close();
        return true;
    }

    if (false == file.open(QIODevice::ReadOnly)) return false;

    QByteArray baIniData = file.readAll();
    file.close();

    if (false == file.open(QIODevice::WriteOnly)) return false;

    file.write(baIniData.data(), pos);
    file.write(line.toLocal8Bit().data(), line.toLocal8Bit().length());
    file.write(baIniData.data() + nextPos, baIniData.length() - nextPos);
    file.close();
    return true;
}


QVariant Setting::value(const QString& key, const QVariant& defValue)
{
    QString keyLower = key.toLower();
    if (mKeyValue.count(keyLower) <= 0) return defValue;

    return mKeyValue[keyLower];
}

QString Setting::getCfgValue(const QString& section, const QString& key, const QString def)
{
    QString sectionKey = QString(section + "/" + key).toLower();

    if (mKeyValue.count(sectionKey) <= 0)
        return def;
    return mKeyValue[sectionKey].toString();
}

QStringList Setting::getSectionsKey(const QString& section) const
{
    QString sectionKey = QString(section + "+").toLower();
    QRegExp regExp(sectionKey, Qt::CaseInsensitive);
    QStringList list;

	for (QMap<QString, QVariant>::const_iterator i = mKeyValue.begin(); i != mKeyValue.end(); ++i)
        if (0 == regExp.indexIn(i.key()))
            list << i.key();

    return list;
}

QStringList Setting::getSectionsValue(const QString& section) const
{
    QString sectionKey = QString(section + "+").toLower();
    QRegExp regExp(sectionKey, Qt::CaseInsensitive);
    QStringList list;

    for (QMap<QString, QVariant>::const_iterator i = mKeyValue.begin(); i != mKeyValue.end(); ++i)
        if (0 == regExp.indexIn(i.key()))
            list << i.value().toString();

    return list;
}

QString Setting::getCftComment(const QString& section, const QString& key, const QString def)
{
    QString sSectionKey = section + "/" + key;

    if (mCommentKey.count(sSectionKey) <= 0) return def;
    return mCommentKey[sSectionKey].trimmed().replace(QRegExp("^//"), "");
}

bool Setting::setCfgValue(const QString& section, const QString& key, const QString& value, const QString comment)
{
    QString sectionKey = section + "/" + key;
    mKeyValue[sectionKey.toLower()] = value.trimmed();
    mSensitiveKey[sectionKey.toLower()] = sectionKey;
    if (comment.length() > 0)
        mCommentKey[sectionKey] = "\t//" + comment;

    saveCfgValue(section, key, value, comment);

    if (!mSections.contains(section, Qt::CaseInsensitive))
    {
        mSections.push_back(section);
    }

    return true;
}

/*!
 * \brief 删除section配置
 */
void Setting::delSection(const QString& section)
{
    if (section.length() <= 0) return;

    QString sectionLower = section.toLower();
    QStringList delKey, delLowerKey;
	for (QMap<QString, QString>::iterator it = mSensitiveKey.begin(); it != mSensitiveKey.end(); ++it)
    {
        if (it.key().startsWith(sectionLower + "/"))
        {
            delLowerKey << it.key();
            delKey << it.value();
        }
    }

    mSections.removeOne(section);
    foreach(QString key, delKey)
        mCommentKey.remove(key);
    foreach(QString key, delLowerKey)
    {
        mKeyValue.remove(key);
        mSensitiveKey.remove(key);
    }

    QString sectionStr = "[" + section + "]\r\n";
    qint64 pos = -1;
    qint64 nextPos = -1;
    if (false == getDeletePos(section, pos, nextPos)) return;

    updateCfgFile(pos, nextPos, sectionStr, "");
}

void Setting::delSectionKey(const QString& section, const QString& key)
{
    if (section.length() <= 0 || key.length() <= 0) return;

    QString sectionStr = "[" + section + "]\r\n";
    qint64 pos = -1;
    qint64 nextPos = -1;
    if (false == getInsertPos(section, key, pos, nextPos)) return;

    updateCfgFile(pos, nextPos, sectionStr, "");
}

int Setting::getSectionCfg(QString section, QMap<QString, QString>& cfgData)
{
    int cnt = 0;
    QString sectionKey = section.toLower() + "/";
    for (QMap<QString, QVariant>::iterator it = mKeyValue.begin(); it != mKeyValue.end(); ++it)
    {
        if (it.key().startsWith(sectionKey))
        {
            QString key = mSensitiveKey[it.key()];
            key = key.right(key.toLocal8Bit().length() - sectionKey.toLocal8Bit().length());
            cfgData[key] = it.value().toString();
            ++cnt;
        }
    }

    return cnt;
}

QStringList Setting::getSections() const
{
    return mSections;
}

QString Setting::getCfgFileName() const
{
    return mCfgFile;
}
