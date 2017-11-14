#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QInputDialog>
#include <QMessageBox>
#include <QTimer>
#include <QDebug>
#include <QUrl>
#include <QLabel>
#include <QFileDialog>
#include <QFile>
#include <QProgressBar>
#include <View/mpsocbox.h>
#include <View/newmpsocdialog.h>
#include <View/simulationtab.h>
#include <View/applicationstab.h>
#include <View/heuristicstab.h>
#include <Core/applicationcontroller.h>
#include <Core/mpsoccontroller.h>
#include <Core/heuristiccontroller.h>

namespace Ui {
class MainWindow;
}

class ApplicationLoader : public QThread {
    Q_OBJECT

public:
    ApplicationLoader(){}
    virtual ~ApplicationLoader(){}

public slots:
    void run() override;

signals:
    void status(QString, int);
};

class MPSoC_Simulator : public QMainWindow
{
    Q_OBJECT
public:
    explicit MPSoC_Simulator(QWidget *parent = 0);
    ~MPSoC_Simulator();

private slots:
    void loadingDone();

    void closeEvent(QCloseEvent *event) override;
    void openLogFile();

    void on_applicationsPushButton_clicked();
    void on_simulationPushButton_clicked();
    void on_heuristicsPushButton_clicked();

private:
    QThread *thread;
    QTimer *timer;
    ApplicationLoader *worker;

    Ui::MainWindow *ui;

    QProgressBar *statusProgress;
    QLabel *statusLabel;

    View::SimulationTab *simulationTab;
    View::ApplicationsTab *applicationsTab;
    View::HeuristicsTab *heuristicsTab;
};

#endif // MAINWINDOW_H
