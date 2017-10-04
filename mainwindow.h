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
#include <QFileDialog>
#include <QFile>
#include <QtScript/qscriptengine.h>
#include "Core/mpsoc.h"
#include "View/mpsocbox.h"

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
    void on_timerSpinBox_valueChanged(int val);
    void on_stepSlider_valueChanged(int value);
    void on_pushButton_2_clicked();

private:
    Ui::MainWindow *ui;
    QStringListModel *applicationsList;
};

#endif // MAINWINDOW_H
