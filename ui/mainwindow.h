#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QTableWidget>
#include <QPushButton>

namespace Ui {
class MainWindow;
}
class QTableWidget;

class MainWindow final : public QWidget
{
    Q_OBJECT
    void initTable(std::unique_ptr<QTableWidget>& table,
                   std::unique_ptr<QPushButton>& btn,
                   int row, int column, const QString& btnText);

public:
    explicit MainWindow(QWidget *parent = Q_NULLPTR);
    ~MainWindow() = default;

private:
    std::shared_ptr<Ui::MainWindow> ui;
    std::unique_ptr<QTableWidget> mTable1 = std::make_unique<QTableWidget>();
    std::unique_ptr<QTableWidget> mTable2 = std::make_unique<QTableWidget>();
    std::unique_ptr<QTableWidget> mTable3 = std::make_unique<QTableWidget>();
    std::unique_ptr<QTableWidget> mTable4 = std::make_unique<QTableWidget>();
    std::unique_ptr<QTableWidget> mTable5 = std::make_unique<QTableWidget>();
    std::unique_ptr<QTableWidget> mTable6 = std::make_unique<QTableWidget>();
    std::unique_ptr<QTableWidget> mTable7 = std::make_unique<QTableWidget>();
    std::unique_ptr<QPushButton> mBtn1 = std::make_unique<QPushButton>();
    std::unique_ptr<QPushButton> mBtn2 = std::make_unique<QPushButton>();
    std::unique_ptr<QPushButton> mBtn3 = std::make_unique<QPushButton>();
    std::unique_ptr<QPushButton> mBtn4 = std::make_unique<QPushButton>();
    std::unique_ptr<QPushButton> mBtn5 = std::make_unique<QPushButton>();
    std::unique_ptr<QPushButton> mBtn6 = std::make_unique<QPushButton>();
    std::unique_ptr<QPushButton> mBtn7 = std::make_unique<QPushButton>();
};

#endif // MAINWINDOW_H
