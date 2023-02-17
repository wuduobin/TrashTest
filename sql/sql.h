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
	bool selectTable1();

private:
	QSqlDatabase mDb;
};

#endif // SQL_H
