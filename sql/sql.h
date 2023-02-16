#ifndef SQL_H
#define SQL_H

#include <QString>
#include <QSqlDatabase>

class SQL
{
public:
	explicit SQL();
	~SQL();
	static SQL Instance();
	bool connect(QString name = "DATADB");


private:
	QSqlDatabase mDb;
};

#endif // SQL_H
