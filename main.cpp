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

    QTextCodec::setCodecForLocale(QTextCodec::codecForName("utf-8"));
    std::unique_ptr<QTranslator> translator = std::make_unique<QTranslator>();
    translator->load(":/res/languages/qt_zh_CN.qm") && app.installTranslator(translator.get());

    QSharedMemory singleApp("QML");
    if (singleApp.attach())
    {
        QMessageBox::warning(Q_NULLPTR, QStringLiteral("警告"), QStringLiteral("客户端程序不允许同时打开两份!"));
        return 0;
    }
    singleApp.create(1);

    Log::init("温度补偿");
    Log::info(QApplication::applicationDirPath() + QStringLiteral(" version:") + QApplication::applicationVersion());

    MainWindow m;
    m.show();

    return app.exec();
}
