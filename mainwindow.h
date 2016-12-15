#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <mpsoc.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    int ci = 0;
    QTimer *timer;

private slots:
    void on_pushButton_2_clicked();

    void on_addMPSoCButton_clicked();

    void increment();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
