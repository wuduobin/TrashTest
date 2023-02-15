#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QHeaderView>
#include <QSpacerItem>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //setWindowState(Qt::WindowMaximized);
    ui->listWidget->setResizeMode(QListView::Adjust);
    ui->listWidget->setAutoScroll(true);

	initTable(mTable1, mBtn1, 5, 18, QString::fromLocal8Bit("原始电压")); // DATADB
	initTable(mTable2, mBtn2, 5, 18, QString::fromLocal8Bit("浓度"));
    initTable(mTable3, mBtn3, 5, 18, QString::fromLocal8Bit("温度补偿")); // GASDB
    initTable(mTable4, mBtn4, 5, 18, QString::fromLocal8Bit("二次拟合"));
	initTable(mTable5, mBtn5, 5, 18, QString::fromLocal8Bit("漂移处理"));
	initTable(mTable6, mBtn6, 5, 18, QString::fromLocal8Bit("二点波动率")); 
    initTable(mTable7, mBtn7, 5, 38, QString::fromLocal8Bit("平滑处理")); //GL1500

	connect(&mBtn1, SIGNAL(clicked()), SLOT(on_mBtn1_clicked()));
	connect(&mBtn2, SIGNAL(clicked()), SLOT(on_mBtn1_clicked()));
	connect(&mBtn3, SIGNAL(clicked()), SLOT(on_mBtn1_clicked()));
	connect(&mBtn4, SIGNAL(clicked()), SLOT(on_mBtn1_clicked()));
	connect(&mBtn5, SIGNAL(clicked()), SLOT(on_mBtn1_clicked()));
	connect(&mBtn6, SIGNAL(clicked()), SLOT(on_mBtn1_clicked()));
	connect(&mBtn7, SIGNAL(clicked()), SLOT(on_mBtn1_clicked()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initTable(QTableWidget& table, QPushButton& btn, int row, int column, const QString& btnText)
{
    table.setRowCount(row);
    table.verticalHeader()->setDefaultSectionSize(40);
    table.setFocusPolicy(Qt::NoFocus);

    QFont font =  table.horizontalHeader()->font();
    font.setBold(true); //表头字体加粗
    font.setPointSize(10); //表头字体大小
    table.horizontalHeader()->setFont(font);

	table.setColumnCount(column);
	if (&table != &mTable7)
    {
        table.setHorizontalHeaderLabels(QStringList() << QString::fromLocal8Bit("时间") << QString::fromLocal8Bit("A\nCO2浓度")
			<< QString::fromLocal8Bit("B\nN2O浓度") << QString::fromLocal8Bit("C\nSF6浓度") 
			<< QString::fromLocal8Bit("D\nCH4浓度") << QString::fromLocal8Bit("E\n待定气体") 
			<< QString::fromLocal8Bit("W\n水浓度") << QString::fromLocal8Bit("F\n待定气体") 
			<< QString::fromLocal8Bit("SENSOR1\n柜外温度") << QString::fromLocal8Bit("SENSOR2\n柜内温度") 
			<< QString::fromLocal8Bit("SENSOR3\n气罐温度") << QString::fromLocal8Bit("TEMPERATURE\n温度") 
			<< QString::fromLocal8Bit("PRESSURE\n压力") << QString::fromLocal8Bit("ChamberTemperature\n光声腔温度") 
			<< QString::fromLocal8Bit("IRTemperature\n光源温度") << QString::fromLocal8Bit("AirTemperature\n空气温度") 
			<< QString::fromLocal8Bit("ChamberPress\n光声腔压力") << QString::fromLocal8Bit("WashAirPress\n洗气压力"));				
	}
    else
    {
		table.setHorizontalHeaderLabels(QStringList() << QString::fromLocal8Bit("时间") << QString::fromLocal8Bit("A\nCO2浓度") 
			<< QString::fromLocal8Bit("B\nN2O浓度") << QString::fromLocal8Bit("C\nSF6浓度") << QString::fromLocal8Bit("D\nCH4浓度")
            << QString::fromLocal8Bit("E\n待定气体") << QString::fromLocal8Bit("W\n水浓度") << QString::fromLocal8Bit("F\n待定气体") 
			<< QString::fromLocal8Bit("SENSOR1\n柜外温度") << QString::fromLocal8Bit("SENSOR2\n柜内温度") 
			<< QString::fromLocal8Bit("SENSOR3\n气罐温度") << QString::fromLocal8Bit("TEMPERATURE\n温度") 
			<< QString::fromLocal8Bit("PRESSURE\n压力") << QString::fromLocal8Bit("SendFlag\n发送标志") 
			<< "A_1" << "B_1" << "C_1" << "D_1" << "E_1" << "F_1" << "W_1" << "SENSOR1_1"<< "SENSOR2_1" << "SENSOR3_1" << "TEMPERATURE_1" << "PRESSURE_1" 
			<< "A_2" << "B_2" << "C_2" << "D_2" << "E_2" << "E_2" << "W_2" << "SENSOR1_2" << "SENSOR2_2" << "SENSOR3_2" << "TEMPERATURE_2" << "PRESSURE_2");
    }

    for (int i = 0; i < column; ++i)
    {
        table.horizontalHeader()->setResizeMode(i, QHeaderView::ResizeToContents);
        if (i)
        {
            QLabel* laName = new QLabel("test" + QString::number(i));
            laName->setStyleSheet("border-width:0;color: rgb(51, 51, 51);font: 12pt \"微软雅黑\";border-style:outset;");
            laName->setAlignment(Qt::AlignCenter);
            table.setCellWidget(4, i, laName);
        }
        else
        {
            QLabel* laName = new QLabel("2021/12/12 00:00:00");
            laName->setStyleSheet("border-width:0;color: rgb(51, 51, 51);font: 12pt \"微软雅黑\";border-style:outset;");
            laName->setAlignment(Qt::AlignCenter);
            table.setCellWidget(4, i, laName);
        }
    }
    table.horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);
    table.horizontalHeader()->setHighlightSections(false);
    table.setStyleSheet("QHeaderView::section {background-color: qlineargradient(x1:0, y1:0, x2:0, y2:1,\
    stop:1 #f2f2f2);color:rgba(0, 0, 0, 0.85);border-width:0;border-style:outset;height:60px;border-top:0px; \
    border-left:0px;border-right:1px solid #cccccc;border-bottom: 0px;}");

    btn.setText(btnText);
    btn.setMinimumSize(QSize(100, 30));
    btn.setStyleSheet("QPushButton{ \
                          border-radius: 8px; \
                          border: 1px solid #1890ff; \
                          font-size: 14px; \
                          color: #1890ff; \
                          } \
                          QPushButton:hover{ \
                          border-radius: 8px; \
                          border: 1px solid #1890ff; \
                          background: #1890ff; \
                          font-size: 14px; \
                          color: #ffffff; \
                          } \
                          QPushButton:pressed{ \
                          border-radius: 8px; \
                          border: 1px solid #1890ff; \
                          background: #006ccf; \
                          font-size: 14px; \
                          color: #ffffff; \
                          }");

    QHBoxLayout* layout = new QHBoxLayout();
    QWidget* widget = new QWidget;
    layout->addWidget(&table);
    layout->addSpacing(30);
    layout->addWidget(&btn);
    layout->setDirection(QBoxLayout::LeftToRight);
    widget->setLayout(layout);
    widget->setStyleSheet("QWidget{border: 0px}");

    QListWidgetItem* item = new QListWidgetItem;
    item->setSizeHint(QSize(0, 300));
    ui->listWidget->addItem(item);
    ui->listWidget->setItemWidget(item, widget);
}

void MainWindow::on_mBtn1_clicked()
{
	qDebug() << "Btn1 clicked";
}

void MainWindow::on_mBtn2_clicked()
{
	qDebug() << "Btn2 clicked";
}

void MainWindow::on_mBtn3_clicked()
{
	qDebug() << "Btn3 clicked";
}

void MainWindow::on_mBtn4_clicked()
{
	qDebug() << "Btn4 clicked";
}

void MainWindow::on_mBtn5_clicked()
{
	qDebug() << "Btn5 clicked";
}

void MainWindow::on_mBtn6_clicked()
{
	qDebug() << "Btn6 clicked";
}

void MainWindow::on_mBtn7_clicked()
{
	qDebug() << "Btn7 clicked";
}