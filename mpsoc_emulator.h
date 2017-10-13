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
#include "View/mpsocbox.h"
#include "View/newmpsocdialog.h"
#include "Core/applicationcontroller.h"
#include "Core/mpsoccontroller.h"
#include "Core/heuristiccontroller.h"

namespace Ui {
class MainWindow;
}

class MPSoC_Emulator : public QMainWindow
{
    Q_OBJECT

public:
    explicit MPSoC_Emulator(QWidget *parent = 0);
    ~MPSoC_Emulator();

private slots:
    //void increment();

    void parallelLoad();

    void applicationsListModel_selectionChanged(const QItemSelection, const QItemSelection);
    void runningListModel_selectionChanged(const QItemSelection, const QItemSelection);

    void applicationButtonsCheckEnable();

    void closeEvent(QCloseEvent *event) override;

    void on_timerSpinBox_valueChanged(int val);
    void on_stepSlider_valueChanged(int val);
    void on_addMPSoCButton_clicked();
    void on_applicationsPushButton_clicked();
    void on_emulationPushButton_clicked();
    void on_heuristicsPushButton_clicked();
private:
    QTimer *thread;

    Ui::MainWindow *ui;

    QProgressBar *statusProgress;
    QLabel *statusLabel;

    QStringListModel *applicationsListModel;
    QStringListModel *runningListModel;

    void loadApplications();
    void loadHeuristics();
};

#endif // MAINWINDOW_H
