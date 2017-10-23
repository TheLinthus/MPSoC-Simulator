#include "appnode.h"

namespace Core {

AppNode::AppNode(int lifespan, QObject *parent)
    : lifespan(lifespan)
    , cycles(0)
    , QObject(parent)
{

}

AppNode::~AppNode() {
    emit finished(this);
}

bool AppNode::isDone() {
    return cycles == lifespan;
}

int AppNode::getLifespan() const {
    return lifespan;
}

int AppNode::getCycles() const {
    return cycles;
}

void AppNode::setLifespan(const int lifespan) {
    this->lifespan = lifespan;
}

void AppNode::tick() {
    if (cycles < lifespan) {
        cycles++;
        emit changed();
        if (isDone()) {
            emit finished(this);
        }
    }
}

} // namespace Core
