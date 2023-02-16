#ifndef SQL_H
#define SQL_H

#include <QString>
#include <QSqlDatabase>

class SQL
{
	bool connect(QString name = "DATADB");

public:
	explicit SQL();
	~SQL();
	static SQL Instance();
	QStringList select();

private:
	QSqlDatabase mDb;
};

#endif // SQL_H
