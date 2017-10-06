#include "appnode.h"

namespace Core {

AppNode::AppNode(qreal lifespan, QObject *parent)
    : lifespan(lifespan)
    , cycles(0)
    , QObject(parent)
{

}

AppNode::~AppNode() {

}

QColor AppNode::getColor() {
    return color;
}

void AppNode::setColor(QColor color) {
    this->color = color;
}

int AppNode::getThread() const {
    return thread;
}

void AppNode::setThread(int value) {
    thread = value;
}

bool AppNode::isDone() {
    return cycles >= lifespan;
}

qreal AppNode::getLifespan() const {
    return lifespan;
}

qreal AppNode::getCycles() const {
    return cycles;
}

void AppNode::tick() {
    cycles++;
    emit changed();
    if (isDone()) {
        emit finished(thread);
    }
}

} // namespace Core
