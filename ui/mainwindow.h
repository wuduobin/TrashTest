#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QTableWidget>
#include <QPushButton>

namespace Ui {
class MainWindow;
}

class MainWindow : public QWidget
{
    Q_OBJECT
    void initTable(QTableWidget& table, QPushButton& btn, int row, int column, const QString& btnText);

public:
    explicit MainWindow(QWidget *parent = NULL);
    ~MainWindow();

public slots:
	void on_mBtn1_clicked();
	void on_mBtn2_clicked();
	void on_mBtn3_clicked();
	void on_mBtn4_clicked();
	void on_mBtn5_clicked();
	void on_mBtn6_clicked();
	void on_mBtn7_clicked();

private:
    Ui::MainWindow* ui;
    QTableWidget mTable1;
    QTableWidget mTable2;
    QTableWidget mTable3;
    QTableWidget mTable4;
    QTableWidget mTable5;
    QTableWidget mTable6;
    QTableWidget mTable7;
    QPushButton mBtn1;
    QPushButton mBtn2;
    QPushButton mBtn3;
    QPushButton mBtn4;
    QPushButton mBtn5;
    QPushButton mBtn6;
    QPushButton mBtn7;
};

#endif // MAINWINDOW_H
