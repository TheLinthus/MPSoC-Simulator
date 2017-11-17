#ifndef CORE_SIMULATIONCONTROLLER_H
#define CORE_SIMULATIONCONTROLLER_H

#include <QObject>
#include <QDebug>
#include <heuristiccontroller.h>
#include <applicationcontroller.h>
#include <mpsoccontroller.h>
#include <scriptconverter.h>

#define simulator Core::SimulationController::instance()

namespace Core {

class SimulationWorker : public QThread
{
    Q_OBJECT
    void run() override;

signals:
    void processed(int);
    void failed(int);
};

class SimulationController : public QObject
{
    Q_OBJECT
private:
    SimulationController(QObject* parent = 0);
    static SimulationController* createInstance();

    bool error;
    int pointer;
    bool started;
    int processingMPSoCs;
    QVector<int> steps; // int as placeholder, it will be changed to a historic of actions

    QTimer timer;
    SimulationWorker worker;
    void newStep();

private slots:
    void workerFinished();
    void processingDone(int index);
    void autoStep();
    void fail(int e);

public:
    ~SimulationController();
    static SimulationController* instance();

    bool isStarted();
    bool isRunning();
    bool isRunEnabled();
    bool isKillEnabled();
    bool isStepEnable();
    int currentStep();
    int stepsCount();

    Core::AppNode *dequeueRunList();

    void stepFoward();
    void stepBackward();

public slots:
    void reset();
    void setInterval(int ms);
    void setAutoStep(bool enabled);

    void runAction(Core::Application *app);
    void killAction(Core::Application *app);

signals:
    void step();
    void reseted();
    void calculating(bool);
    void notify();
    void failed(int);
};

} // namespace Core

#endif // CORE_SIMULATIONCONTROLLER_H
