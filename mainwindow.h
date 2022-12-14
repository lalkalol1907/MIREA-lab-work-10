#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QTableWidgetItem>
#include <QMainWindow>
#include <QMessageBox>
#include "./ui_mainwindow.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_clear_pressed();

    void clearOutput();

    bool confirmation();

    void changeButtonsStatus(bool b);

    void fillTable(double *arr, int n);

    void on_random_pressed();

    void on_elemFind_textEdited(const QString &arg1);

    void on_find_pressed();

    void on_size_textEdited(const QString &arg1);

    void on_sizeButton_pressed();

    void on_inputTable_itemChanged(QTableWidgetItem *item);

    void on_sortButton_pressed();

    void on_maxButton_pressed();

    void on_minButton_pressed();

    void on_avgButton_pressed();

    void on_deleteDup_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
