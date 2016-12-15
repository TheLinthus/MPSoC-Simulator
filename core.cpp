#include "core.h"

namespace Core {

Core::Core(int x, int y, int threads, QObject *parent)
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

Channel *Core::getChannel(Direction direction) {
    switch (direction) {
        case North : return north;
        case West : return west;
        case South : return south;
        case East : return east;
        default : return nullptr;
    }
}

int Core::firstIdle() {
    for (int i = 0; i < threads; i++) {
        if (cores[i] != nullptr)
            return i;
    }
    return -1;
}

int Core::isIdle(int i)
{
    return cores[i] == nullptr;
}

bool Core::run(AppNode *node, int thread) {
    if (thread < 0 || thread >= threads) {
        return false;
    }
    if (cores[thread] == nullptr) {
        cores[thread] = node;
        emit changed();
        return true;
    }
    return false;
}

AppNode* Core::runningNode(int thread) {
    return cores[thread];
}

int Core::nOfThreads() {
    return threads;
}
int Core::getX() {
    return x;
}

int Core::getY() {
    return y;
}

void Core::setChannel(Direction direction, Channel *channel)
{
    switch (direction) {
        case North : north = channel; break;
        case West : west = channel; break;
        case South : south = channel; break;
        case East : east = channel; break;
    }
}

} // namespace Core
