#include "processor.h"

namespace Core {

Processor::Processor(int x, int y, Type type, int threads, QObject *parent)
    : QObject(parent)
    , inNorth(0)
    , inWest(0)
    , inSouth(0)
    , inEast(0)
    , outNorth(0)
    , outWest(0)
    , outSouth(0)
    , outEast(0)
    , threads(threads)
    , cores(threads)
    , x(x)
    , y(y)
    , type(type)
{
    if (isMaster()) {
        MasterApplication *masterApplication = new MasterApplication();
        connect(this, SIGNAL(destroyed(QObject*)), masterApplication, SLOT(deleteLater()));
        masterApplication->setRootNode(0,-1);
        cores[0] = masterApplication->getNode(0);
    }
}

int Processor::firstIdle() {
    for (int i = 0; i < threads; i++) {
        if (cores[i] != 0)
            return i;
    }
    return -1;
}

int Processor::isIdle(int i) {
    return cores[i] == 0;
}

bool Processor::run(AppNode *node, int thread) {
    if (thread < 0 || thread >= threads) {
        return false;
    }
    if (cores[thread] == 0) {
        connect(node, SIGNAL(finished(AppNode*)), this, SLOT(kill(AppNode*)));
        cores[thread] = node;
        node->setRunning(true);
        node->setInCoreX(x);
        node->setInCoreY(y);
        node->setInThread(thread);
        emit changed();
        return true;
    }
    return false;
}

AppNode* Processor::runningNode(int thread) {
    return cores[thread];
}

int Processor::nOfThreads() const {
    return threads;
}
int Processor::getX() const {
    return x;
}

int Processor::getY() const {
    return y;
}

int Processor::getType() const {
    return type;
}

bool Processor::isMaster() {
    return type == Master;
}

int Processor::getThreadOf(const AppNode *appNode) {
    for (int i = 0; i < threads; i++) {
        if (appNode == cores[i]) {
            return i;
        }
    }
    return -1;
}

void Processor::kill(QObject *appNode) {
    kill((AppNode *) appNode);
}

void Processor::kill(AppNode *appNode) {
    kill(getThreadOf(appNode));
}

void Processor::kill(int thread) {
    if (thread < 0 || thread >= threads) {
        return;
    }
    if (cores[thread] != 0) {
        cores[thread] = 0;
        emit changed();
    }
}

void Processor::setChannel(DataDirection inout, Direction direction, Channel *channel)
{
    switch (direction) {
        case North : if (inout == In) inNorth = channel; else outNorth = channel; break;
        case West : if (inout == In) inWest = channel; else outWest = channel; break;
        case South : if (inout == In) inSouth = channel; else outSouth = channel; break;
        case East : if (inout == In) inEast = channel; else outEast = channel; break;
    }
}

Channel *Processor::getChannel(DataDirection inout, Direction direction)
{
    switch (direction) {
        case North : return inout == In ? inNorth : outNorth;
        case West : return inout == In ? inWest : outWest;
        case South : return inout == In ? inSouth : outSouth;
        case East : return inout == In ? inEast : outEast;
        default : return 0;
    }
}

} // namespace Core
