#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QHeaderView>
#include <QSpacerItem>

#include "log/log.h"
#include "sql/sql.h"

namespace {
	const unsigned int ROW = 5;
	const unsigned int COLUMN = 18;
}

MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowState(Qt::WindowMaximized);
    ui->listWidget->setResizeMode(QListView::Adjust);
    ui->listWidget->setAutoScroll(true);

	initTable(mTable1, mBtn1, ROW, COLUMN, QString::fromLocal8Bit("原始电压")); // DATADB
	initTable(mTable2, mBtn2, ROW, COLUMN, QString::fromLocal8Bit("浓度"));
    initTable(mTable3, mBtn3, ROW, COLUMN, QString::fromLocal8Bit("温度补偿")); // GASDB
    initTable(mTable4, mBtn4, ROW, COLUMN, QString::fromLocal8Bit("二次拟合"));
	initTable(mTable5, mBtn5, ROW, COLUMN, QString::fromLocal8Bit("漂移处理"));
	initTable(mTable6, mBtn6, ROW, COLUMN, QString::fromLocal8Bit("二点波动率")); 
    initTable(mTable7, mBtn7, ROW, COLUMN, QString::fromLocal8Bit("平滑处理")); //GL1500

	connect(&mBtn1, SIGNAL(clicked(bool)), SLOT(on_mBtn1_clicked()));
	connect(&mBtn2, SIGNAL(clicked(bool)), SLOT(on_mBtn2_clicked()));
	connect(&mBtn3, SIGNAL(clicked(bool)), SLOT(on_mBtn3_clicked()));
	connect(&mBtn4, SIGNAL(clicked(bool)), SLOT(on_mBtn4_clicked()));
	connect(&mBtn5, SIGNAL(clicked(bool)), SLOT(on_mBtn5_clicked()));
	connect(&mBtn6, SIGNAL(clicked(bool)), SLOT(on_mBtn6_clicked()));
	connect(&mBtn7, SIGNAL(clicked(bool)), SLOT(on_mBtn7_clicked()));
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
	table.setHorizontalHeaderLabels(QStringList() << QString::fromLocal8Bit("时间") << QString::fromLocal8Bit("A\nCO2浓度")
			<< QString::fromLocal8Bit("B\nN2O浓度") << QString::fromLocal8Bit("C\nSF6浓度") 
			<< QString::fromLocal8Bit("D\nCH4浓度") << QString::fromLocal8Bit("E\n待定气体") 
			<< QString::fromLocal8Bit("W\n水浓度") << QString::fromLocal8Bit("F\n待定气体") 
			<< QString::fromLocal8Bit("SENSOR1\n柜外温度") << QString::fromLocal8Bit("SENSOR2\n柜内温度") 
			<< QString::fromLocal8Bit("SENSOR3\n气罐温度") << QString::fromLocal8Bit("TEMPERATURE\n温度") 
			<< QString::fromLocal8Bit("PRESSURE\n压力") << QString::fromLocal8Bit("ChamberTemperature\n光声腔温度") 
			<< QString::fromLocal8Bit("IRTemperature\n光源温度") << QString::fromLocal8Bit("AirTemperature\n空气温度") 
			<< QString::fromLocal8Bit("ChamberPress\n光声腔压力") << QString::fromLocal8Bit("WashAirPress\n洗气压力"));	

    for (int i = 0; i < column; ++i)
    {
        table.horizontalHeader()->setResizeMode(i, QHeaderView::Interactive);
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
	Log::info("Btn1 clicked");
	mTable1.setRowCount(0);
	SQL::Instance().selectTable1();
}

void MainWindow::on_mBtn2_clicked()
{
	Log::info("Btn2 clicked");
}

void MainWindow::on_mBtn3_clicked()
{
	Log::info("Btn3 clicked");
}

void MainWindow::on_mBtn4_clicked()
{
	Log::info("Btn4 clicked");
}

void MainWindow::on_mBtn5_clicked()
{
	Log::info("Btn5 clicked");
}

void MainWindow::on_mBtn6_clicked()
{
	Log::info("Btn6 clicked");
}

void MainWindow::on_mBtn7_clicked()
{
	Log::info("Btn7 clicked");
}

void MainWindow::insertTable1(QString column)
{
	QStringList list = column.split("|");
	int row = mTable1.rowCount();
	mTable1.insertRow(row);

	for (int i = 0; i < COLUMN; ++i)
	{
		QLabel* laName = new QLabel(list.at(i));
		laName->setStyleSheet("border-width:0;color: rgb(51, 51, 51);font: 12pt \"微软雅黑\";border-style:outset;");
		laName->setAlignment(Qt::AlignCenter);
		mTable1.setCellWidget(row, i, laName);
	}
}
