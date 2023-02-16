/******************************************************************************************************************
* Copyright (C): 本代码所有版权为和智电气所有，任何个人或者组织不得以任何方式复制、转载以挪作他用，否则将构成对和智电气的产权知识侵权
* @autor      : Robin 吴多宾
* @emil:      :
* @date       : 2023/2/7
******************************************************************************************************************/

#include <QApplication>
#include <QTextCodec>
#include <QTranslator>
#include <QSharedMemory>
#include <QMessageBox>

#include "log/log.h"
#include "ui/mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
	QApplication::setApplicationVersion("0.0.0.1");

    QTextCodec::setCodecForLocale(QTextCodec::codecForName("GB18030")); // UTF-8

    QTranslator translator;
    if (translator.load(":/res/languages/qt_zh_CN.qm")) app.installTranslator(&translator);

    QSharedMemory singleApp("QML");
    if (singleApp.attach())
    {
		QMessageBox::warning(NULL, QString::fromLocal8Bit("警告"), QString::fromLocal8Bit("客户端程序不允许同时打开两份!"));
        return 0;
    }
    singleApp.create(1);

	Log::init("TC");
    Log::info(QApplication::applicationDirPath() + QString::fromLocal8Bit(" version:") + QApplication::applicationVersion());

    MainWindow m;
    m.show();

    return app.exec();
}
