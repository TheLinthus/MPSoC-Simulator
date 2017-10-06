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
#include <QProgressBar>
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

private slots:
    //void increment();

    void on_applicationsList_changed(const QItemSelection &selection);
    void on_runningList_changed(const QItemSelection &selection);

    void closeEvent(QCloseEvent *) override;

    void on_timerSpinBox_valueChanged(int val);
    void on_stepSlider_valueChanged(int val);
    void on_addMPSoCButton_clicked();
    void on_applicationsPushButton_clicked();
    void on_emulationPushButton_clicked();

    void on_heuristicsPushButton_clicked();

private:
    QTimer *timer;

    Ui::MainWindow *ui;
    QStringListModel *applicationsList;

    QProgressBar *statusProgress;
};

#endif // MAINWINDOW_H
