#include "sql.h"

#include <QStringList>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QVariant>

#ifdef WIN32
#include <Windows.h>
#endif

#include "log/log.h"
#include "ui/mainwindow.h"

extern MainWindow* gMainWindow;

SQL::SQL()
{
#ifdef MYDEBUG
	QStringList drivers = QSqlDatabase::drivers(); 
	QString driver = "Available drivers:";
	foreach(QString dri, drivers)
	{
		driver.append(" ").append(dri);
	}
	Log::info(driver);
#endif
}

SQL::~SQL()
{
	if (mDb.isOpen())
	{
		mDb.close();
	}
}

SQL SQL::Instance()
{
	static SQL sql;
	return sql;
}

bool SQL::connect(QString name)
{
	mDb = QSqlDatabase::addDatabase("QODBC", name);
	// 连接.mdb文件，注意空格，数据库用绝对路径 TODO路径获取改为打开系统文件夹选择相应的文件
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
		Log::info(QString("driverName=").append(mDb.driverName())
			.append(" databaseName=").append(mDb.databaseName())
			.append(" hostName=").append(mDb.hostName())
			.append(" userName=").append(mDb.userName())
			.append(" password=").append(mDb.password())
			.append(" connectionName=").append(mDb.connectionName())
			.append(" connectOptions=").append(mDb.connectOptions()));
	}

	return ret;
}

bool SQL::selectTable1()
{
	if (false == connect())
	{
		return false;
	}
	
	QStringList tables = mDb.tables(QSql::Tables);
	if (tables.size() < 1)
	{
		Log::error("query select error: tables size < 1 ");
		return false;
	}

	QSqlQuery queryCol("SELECT COUNT(*) FROM " + tables.at(0), mDb);
	QSqlRecord recordCol = queryCol.record();
	if (queryCol.first())
	{	// 行数
		Log::info(tables.at(0) + " table column is: " + QString::number(queryCol.value(0).toInt()));
	}

	QSqlQuery query("SELECT * FROM " + tables.at(0), mDb);

	QSqlRecord record = query.record();
	int cnt = record.count(); // 列数
	if (cnt != 19)
	{	
		Log::info("table1 column != 19");
		return false;
	}

	QString fieldName = tables.at(0) + " table1 field: ";
	for (int j = 0; j < cnt; ++j)
	{
		fieldName.append(record.fieldName(j)).append("  ");
	}
	Log::info(fieldName);
	while (query.next())
	{	
		QString column; // 一开始用QStringList,但qt4定义的信号参数为QStringList编译不通过
		QVariant var;

		// 定义局部变量便于调试查看值
		// 表的第一个字段是自动增长的ID,跳过 
		QString a1 = query.value(1).toDateTime().toString("yyyy-MM-dd hh:mm:ss"); // DateTime
		QString a2 = QString::number(query.value(2).toFloat(), 'f', 6); // A
		QString a3 = QString::number(query.value(3).toFloat(), 'f', 6); // B
		QString a4 = QString::number(query.value(4).toFloat(), 'f', 6); // C
		QString a5 = QString::number(query.value(5).toFloat(), 'f', 6); // D
		QString a6 = QString::number(query.value(6).toFloat(), 'f', 6); // E
		QString a7 = QString::number(query.value(7).toFloat(), 'f', 6); // W
		QString a8 = QString::number(query.value(8).toFloat(), 'f', 6); // F
		QString a9 = QString::number(query.value(9).toFloat(), 'f', 6); // SENSOR1
		QString a10 = QString::number(query.value(10).toFloat(), 'f', 6); // SENSOR2
		QString a11 = QString::number(query.value(11).toLongLong()); // SENSOR3
		QString a12 = QString::number(query.value(12).toLongLong()); // TEMPERATURE
		QString a13 = QString::number(query.value(13).toLongLong()); // PRESSURE
		QString a14 = QString::number(query.value(14).toFloat(), 'f', 6); // ChamberTemperature
		QString a15 = QString::number(query.value(15).toFloat(), 'f', 6); // IRTemperature
		QString a16 = QString::number(query.value(16).toFloat(), 'f', 6); // AirTemperature
		QString a17 = QString::number(query.value(17).toFloat(), 'f', 6); // ChamberPress
		QString a18 = QString::number(query.value(18).toFloat(), 'f', 6); // WashAirPress

		column.append(a1).append("|").append(a2).append("|").append(a3).append("|")
			  .append(a4).append("|").append(a5).append("|").append(a6).append("|")
			  .append(a7).append("|").append(a8).append("|").append(a9).append("|")
			  .append(a10).append("|").append(a11).append("|").append(a12).append("|")
			  .append(a13).append("|").append(a14).append("|").append(a15).append("|")
			  .append(a16).append("|").append(a17).append("|").append(a18).append("|");
		if (column.size() > 1)
		{
			column.chop(1);
		}

		// 打算用QT的信号-槽机制，发射一个信号，然后在MainWindow类里执行对应的槽函数
		// 但是qt4的原因，老是走不到对应的槽函数，因此更换为了全局变量gMainWidow
		gMainWindow->insertTable1(column); // 定义局部变量column便于调试查看值
	}

	mDb.close();
	return true;
}
