#include "processor.h"

namespace Core {

Processor::Processor(int x, int y, int threads, QObject *parent)
    : QObject(parent)
    , north(nullptr)
    , west(nullptr)
    , south(nullptr)
    , east(nullptr)
    , threads(threads)
    , x(x)
    , y(y)
{
    cores = new AppNode*[threads]();
}

Channel *Processor::getChannel(Direction direction) {
    switch (direction) {
        case North : return north;
        case West : return west;
        case South : return south;
        case East : return east;
        default : return nullptr;
    }
}

int Processor::firstIdle() {
    for (int i = 0; i < threads; i++) {
        if (cores[i] != nullptr)
            return i;
    }
    return -1;
}

int Processor::isIdle(int i)
{
    return cores[i] == nullptr;
}

bool Processor::run(AppNode *node, int thread) {
    if (thread < 0 || thread >= threads) {
        return false;
    }
    if (cores[thread] == nullptr) {
        node->setThread(thread);
        connect(node, SIGNAL(finished(int)), this, SLOT(kill(int)));
        cores[thread] = node;
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

void Processor::kill(int thread) {
    if (thread < 0 || thread >= threads) {
        return;
    }
    if (cores[thread] != nullptr) {
        delete cores[thread];
        cores[thread] = nullptr;
        emit changed();
    }
}

void Processor::setChannel(Direction direction, Channel *channel)
{
    switch (direction) {
        case North : north = channel; break;
        case West : west = channel; break;
        case South : south = channel; break;
        case East : east = channel; break;
    }
}

} // namespace Core
