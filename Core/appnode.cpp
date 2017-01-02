#include "appnode.h"

namespace Core {

AppNode::AppNode(qreal lifespan, QColor *color, QObject *parent)
    : lifespan(lifespan)
    , color(color)
    , QObject(parent)
{

}

QColor *AppNode::getColor() {
    return color;
}

int AppNode::getThread() const
{
    return thread;
}

void AppNode::setThread(int value)
{
    thread = value;
}

qreal AppNode::getLifespan() const
{
    return lifespan;
}

void AppNode::tick()
{
    lifespan--;
    emit changed();
    if (lifespan == 0) {
        emit finished(thread);
    }
}

} // namespace Core
