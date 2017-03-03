#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QInputDialog>
#include <QMessageBox>
#include <QTimer>
#include <QDesktopServices>
#include <QVector>
#include <QDebug>
#include <QUrl>
#include "Core/mpsoc.h"
#include "View/mpsocbox.h"
#include "graphicalmpsoc.h"

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
    void on_addMPSoCButton_clicked();

    //void increment();

    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
