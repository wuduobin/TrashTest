#include "sql.h"

#include <QStringList>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QVariant>

#include "log/log.h"

SQL::SQL()
{
	QStringList drivers = QSqlDatabase::drivers(); 
	QString driver = "Available drivers:";
	foreach(QString dri, drivers)
	{
		driver.append(" ").append(dri);
	}
	Log::info(driver);
}

SQL::~SQL()
{
}

SQL SQL::Instance()
{
	static SQL sql;
	return sql;
}

bool SQL::connect(QString name)
{
	mDb = QSqlDatabase::addDatabase("QODBC", name);
	//连接.mdb文件，注意空格，数据库用绝对路径 TODO路径获取改为打开系统文件夹选择相应的文件
	QString path = QString("DRIVER={Microsoft Access Driver (*.mdb, *.accdb)};FIL={MS Access};DBQ=F:/bin_debug/DATADB.mdb");
	mDb.setDatabaseName(path);
	bool ret = mDb.open();
	if (false == ret) 
	{
		QSqlError error = mDb.lastError();
		if (error.isValid()) 
		{
			Log::error(QString("open database failed! error=").append(QString::number(static_cast<int>(error.type())))
				.append(" driverText=").append(error.driverText())
				.append(" databaseText=").append(error.databaseText())
				.append(" text=").append(error.text()));
		}
	}
	Log::info(QString("driverName=").append(mDb.driverName())
		.append(" databaseName=").append(mDb.databaseName())
		.append(" hostName=").append(mDb.hostName())
		.append(" userName=").append(mDb.userName())
		.append(" password=").append(mDb.password())
		.append(" connectionName=").append(mDb.connectionName())
		.append(" connectOptions=").append(mDb.connectOptions()));

	return ret;
}

QStringList SQL::select()
{
	QStringList ret;
	if (false == connect())
	{
		return ret;
	}
	
	QStringList tables = mDb.tables(QSql::Tables);
	if (tables.size() < 1)
	{
		Log::error("query select error: tables size < 1 ");
		return ret;
	}

	QSqlQuery query("SELECT * FROM " + tables.at(0), mDb);

	QSqlRecord record = query.record();
	int cnt = record.count(); //列数

	QString fieldName;
	for(int j = 0; j < cnt; ++j)
	{
		fieldName = record.fieldName(j);
	}

	while (query.next())
	{
		QString row;
		QVariant var;
		for (int i = 0; i < cnt; ++i)
		{
			if (false == (var = query.value(i)).isValid())
			{
				row.append("|");
			}
			else
			{
				row.append(var.toDateTime().toString("yyyy-MM-dd hh:mm:ss")).append("|");
			}
		}
		if (row.size())
		{
			row.chop(1);
		}
	}

	mDb.close();
	return ret;
}
