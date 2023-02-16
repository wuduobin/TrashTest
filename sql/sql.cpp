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
	mDb = QSqlDatabase::addDatabase("QODBC3", name);
	//连接.mdb文件，注意空格，数据库用绝对路径 TODO路径获取改为打开系统文件夹选择相应的文件
	QString path = QString("DRIVER={Microsoft Access Driver (*.mdb, *.accdb)};FIL={MS Access};DBQ=F:/bin_debug/DATADB.mdb");
	mDb.setDatabaseName(path);
	bool ret = mDb.open();
	if (ret) 
	{ 
		QStringList tables = mDb.tables(QSql::Tables);
	}
	else
	{
		QSqlError error = mDb.lastError();
		if (error.isValid()) 
		{
			Log::info(QString("open database failed! error=").append(QString::number(static_cast<int>(error.type())))
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