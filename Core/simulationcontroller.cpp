#include "simulationcontroller.h"
#include "singleton.h"

namespace Core {

SimulationController::SimulationController(QObject *parent) :
    QObject(parent),
    timer(this),
    pointer(-1),
    started(false),
    processingMPSoCs(0),
    error(false)
{
    timer.setInterval(500);
    connect(&worker, SIGNAL(processed(int)), this, SLOT(processingDone(int)));
    connect(&worker, SIGNAL(failed(int)), this, SLOT(fail(int)));
    connect(&timer, SIGNAL(timeout()), this, SLOT(autoStep()));
    connect(mpsocs, SIGNAL(processed(int)), this, SLOT(processingDone()));
}

SimulationController *SimulationController::createInstance() {
    return new SimulationController();
}

void SimulationController::autoStep() {
    if (isStepEnable()) {
        this->stepFoward();
    } else {
        qWarning() << "Step skiped, processing is to slow for this tick rate";
    }
}

SimulationController::~SimulationController() {

}

SimulationController *SimulationController::instance() {
    return Singleton<Core::SimulationController>::instance(Core::SimulationController::createInstance);
}

bool SimulationController::isStarted() {
    return started;
}

bool SimulationController::isRunning() {
    return timer.isActive();
}

bool SimulationController::isRunEnabled() {
    if (!error && (steps.value(pointer, -1) == 0 || !started)) {
        return true;
    } else {
        return false;
    }
}

bool SimulationController::isKillEnabled() {
    if (!error && (steps.value(pointer, -1) == 0)) {
        return true;
    } else {
        return false;
    }
}

bool SimulationController::isStepEnable() {
    return !error && processingMPSoCs == 0 && mpsocs->count() > 0;
}

int SimulationController::currentStep() {
    return pointer;
}

int SimulationController::stepsCount() {
    return steps.count();
}

void SimulationController::setInterval(int ms) {
    timer.setInterval(ms);
}

void SimulationController::setAutoStep(bool enabled) {
    if (enabled) {
        timer.start();
    } else {
        timer.stop();
    }
    emit notify();
}

void SimulationController::reset() {
    steps.clear();
    processList.clear();
    started = false;
    error = false;
    pointer = -1;
    processingMPSoCs = 0;
    timer.stop();
    apps->killAll();
    emit notify();
}

Core::AppNode* SimulationController::dequeueRunList() {
    if (!processList.isEmpty()) {
        return processList.takeFirst();
    }
    return 0;
}

void SimulationController::newStep() {
    started = true;

    pointer++;
    steps.append(0);
}

void SimulationController::processingDone(int index) {
    processingMPSoCs--;
    if (processingMPSoCs == 0) {
        emit notify();
    }
}

void SimulationController::stepFoward() {
    if (started && pointer + 1 < steps.count()) {
        pointer++;
        // TODO - Redo step from history
    } else {
        newStep();
    }
    if (!processList.isEmpty()) {
        worker.start();
    }
}

void SimulationController::stepBackward() {
    if (pointer > 0) {
        pointer--;
        // TODO - Undo step from history
    }
    emit notify();
}

void SimulationController::runAction(Application *app) {
    if (pointer + 1 == steps.size()) {
        newStep();
    }
    if (steps.value(pointer, -1) == 0) {
        steps[pointer] = 1;
        processList += app->getAllNodes();
        emit notify();
    }
    if (!processList.isEmpty()) {
        worker.start();
    }
}

void SimulationController::killAction(Application *app) {
    if (steps.value(pointer, -1) == 0) {
        steps[pointer] = 2;
        emit notify();
    }
}

void SimulationController::fail(int e){
    if (e == 0) /*if (e.fatal())*/ {
        error = true;
        timer.stop();
    }
    if (e == 1) {
        processingMPSoCs = 0;
    }
    emit failed(e);
    emit notify();
}

void SimulationWorker::run() {
    Core::Heuristic *heuristic;
    Core::MPSoC *mpsoc;
    Core::AppNode *node = simulator->dequeueRunList();
    if (node == 0) {
        emit failed(1); // Nothing to sim
        return;
    }
    for (int i = 0; i < mpsocs->count(); i++) {
        mpsoc = mpsocs->get(i);
        heuristic = mpsoc->getHeuristic();

        QPoint point;
        int thread = 0;

        QScriptValueList args;

        args << mpsoc->toScriptObject();
        args << node->getN();
        args << ((Core::Application *) node->parent())->toScriptObject(heuristic->getEngine());
        int result = heuristic->selectCore(point, args); // Thread number not considered yet (future version)
        if (result != 1) {
            emit failed(result);
        }
        if (!mpsoc->getCore(point.x(),point.y())->run(node, thread)) {
            emit failed(0); // Core not empty
            return;
        }

        emit processed(i);
    }
}

} // namespace Core
