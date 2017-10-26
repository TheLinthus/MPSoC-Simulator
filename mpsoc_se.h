#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QInputDialog>
#include <QMessageBox>
#include <QTimer>
#include <QDebug>
#include <QUrl>
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
    //void increment();
    void loadingDone();

    void applicationButtonsCheckEnable();

    void closeEvent(QCloseEvent *event) override;

    void applicationsListModel_selectionChanged(const QItemSelection, const QItemSelection);
    void runningListModel_selectionChanged(const QItemSelection, const QItemSelection);
    void applicationsGroupListModel_selectionChanged(const QItemSelection s, const QItemSelection);

    void on_timerSpinBox_valueChanged(int val);
    void on_stepSlider_valueChanged(int val);
    void on_addMPSoCButton_clicked();
    void on_applicationsPushButton_clicked();
    void on_simulationPushButton_clicked();
    void on_heuristicsPushButton_clicked();
    void on_nextStepButton_clicked();
    void on_playTimerButton_clicked();
    void on_pauseTimerButton_clicked();
    void on_pushButtonCreateNewApplicationGroup_clicked();
    void on_pushButtonCreateApplicationGroupFromFile_clicked();
    void on_pushButtonSaveApplicationGroup_clicked();

    void on_pushButtonCancelApplicationGroup_clicked();

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

    QStringListModel *applicationsListModel;
    QStringListModel *applicationsGroupListModel;
    QStringListModel *runningListModel;
    Core::ApplicationGroup *editApplicationsGroup;
    void clearEditor();
};

#endif // MAINWINDOW_H
